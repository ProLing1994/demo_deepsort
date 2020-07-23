#include "ncnn_centernet.h"
#include <map>
#include <string>

#ifdef _DEBUG
#include <iostream>
#include <time.h>
#endif

using namespace std;

Centerobj::Centerobj()
{
}

Centerobj::~Centerobj()
{
}

int Centerobj::init(std::string model_path)
{
	// std::string param = model_path + "centernet_mbv2_voc.param";
	// std::string bin= model_path + "centernet_mbv2_voc.bin";
	// std::string param = model_path + "vgg_FPN_centernet_ncnn.param";
	// std::string bin= model_path + "vgg_FPN_centernet_ncnn.bin";
	std::string param = model_path + "vgg_FPN_0723_1_ncnn.param";
	std::string bin= model_path + "vgg_FPN_0723_1_ncnn.bin";
	net.load_param(param.data());
	net.load_model(bin.data());
	return 0;
}

void Centerobj::pretty_print(const ncnn::Mat& m)
{
    for (int q=0; q<m.c; q++)
    {
        const float* ptr = m.channel(q);
        for (int y=0; y<1; y++)
        {
            for (int x=0; x<1; x++)
            {
                printf("%f ", ptr[x]);
            }
            ptr += m.w;
			std::cout<<m.w<<std::endl;
            printf("\n");
        }
        printf("------------------------\n");
    }
}

int Centerobj::detect(ncnn::Mat & inblob, std::vector<ObjInfo>& objs, int resized_w, int resized_h, float scoreThresh, float nmsThresh)
{
	if (inblob.empty()) {
		 std::cout << "blob is empty ,please check!" << std::endl;
		 return -1;
	}

	#ifdef _DEBUG
		clock_t begin, end;	
		begin = clock();
	#endif

	image_h = inblob.h;
	image_w = inblob.w;

	scale_w = (float)image_w / (float)resized_w;
	scale_h = (float)image_h / (float)resized_h;

	ncnn::Mat in;
	dynamicScale(resized_w, resized_h);

	float mean_vals_1[3]  = {0, 0, 0} ;
	float norm_vals_1[3]  = {1.0/255, 1.0/255, 1.0/255} ;

	inblob.substract_mean_normalize(mean_vals_1, norm_vals_1);

	ncnn::Extractor ex = net.create_extractor();
	ex.input("data", inblob);
	ncnn::Mat heatmap, scale, offset, id_feature;
	ex.extract("Sigmoid_blob", heatmap);
	ex.extract("conv_blob27", scale);
	ex.extract("conv_blob29", offset);
	ex.extract("conv_blob28", id_feature);

	#ifdef _DEBUG
		end = clock();
		std::cout <<"ncnn forward time: " << 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0 << " ms." << std::endl;
		begin = clock();
	#endif

	decode(heatmap, scale, offset, id_feature, objs, scoreThresh, nmsThresh);

	#ifdef _DEBUG
		end = clock();
		std::cout <<"ncnn decode time: " << 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0 << " ms." << std::endl;
	#endif

	return 0;
}

void Centerobj::nms(std::vector<ObjInfo>& input, std::vector<ObjInfo>& output, float nmsthreshold,int type)
{
	if (input.empty()) {
		return;
	}
	std::sort(input.begin(), input.end(),
		[](const ObjInfo& a, const ObjInfo& b)
		{
			return a.score < b.score;
		});

	float IOU = 0;
	float maxX = 0;
	float maxY = 0;
	float minX = 0;
	float minY = 0;
	std::vector<int> vPick;
	int nPick = 0;
	std::multimap<float, int> vScores;
	const int num_boxes = input.size();
	vPick.resize(num_boxes);
	for (int i = 0; i < num_boxes; ++i) {
		vScores.insert(std::pair<float, int>(input[i].score, i));
	}
	while (vScores.size() > 0) {
		int last = vScores.rbegin()->second;
		vPick[nPick] = last;
		nPick += 1;
		for (std::multimap<float, int>::iterator it = vScores.begin(); it != vScores.end();) {
			int it_idx = it->second;
			maxX = max(input.at(it_idx).x1, input.at(last).x1);
			maxY = max(input.at(it_idx).y1, input.at(last).y1);
			minX = min(input.at(it_idx).x2, input.at(last).x2);
			minY = min(input.at(it_idx).y2, input.at(last).y2);
			//maxX1 and maxY1 reuse 
			maxX = ((minX - maxX + 1) > 0) ? (minX - maxX + 1) : 0;
			maxY = ((minY - maxY + 1) > 0) ? (minY - maxY + 1) : 0;
			//IOU reuse for the area of two bbox
			IOU = maxX * maxY;
			if (type==NMS_UNION)
				IOU = IOU / (input.at(it_idx).area + input.at(last).area - IOU);
			else if (type == NMS_MIN) {
				IOU = IOU / ((input.at(it_idx).area < input.at(last).area) ? input.at(it_idx).area : input.at(last).area);
			}
			if (IOU > nmsthreshold) {
				it = vScores.erase(it);
			}
			else {
				it++;
			}
		}
	}

	vPick.resize(nPick);
	output.resize(nPick);
	for (int i = 0; i < nPick; i++) {
		output[i] = input[vPick[i]];
	}
}

void Centerobj::decode(ncnn::Mat & heatmap  , ncnn::Mat & scale, ncnn::Mat & offset, ncnn::Mat & id_feature, std::vector<ObjInfo>& objs, float scoreThresh, float nmsThresh)
{
	int fea_h = heatmap.h;
	int fea_w = heatmap.w;
	int fea_c = heatmap.c;
	int spacial_size = fea_w*fea_h;

	float *heatmap_ = (float*)(heatmap.data);
	float *scale0 = (float*)(scale.data);
	float *scale1 = scale0 + spacial_size;

	std::vector<float*> idd(128);

	float *id0 = (float*)(id_feature.data);
	idd[0] = id0;
	for(int i = 1; i < 128; i++)
	{
		id0 += spacial_size;
		idd[i] = id0;
	}
	
	float *offset0 = (float*)(offset.data);
	float *offset1 = offset0 + spacial_size;

	std::vector<float> ids;
	genIds(heatmap_,fea_h, fea_w,fea_c, scoreThresh, ids);
	
	std::vector<ObjInfo> objs_tmp;
	for (int i = 0; i < ids.size() / 4; i++) {
		
		int id_h = ids[4 * i];
		int id_w = ids[4 * i + 1];
		int cate_id = ids[4 * i + 2];
		float score = ids[4 * i + 3];
		int index = id_h*fea_w + id_w;

		// float s0 = std::exp(scale0[index]) * 4;
		// float s1 = std::exp(scale1[index]) * 4;
		float s0 = scale0[index] * 4;
		float s1 = scale1[index] * 4;

		float o0 = offset0[index];
		float o1 = offset1[index];
		std::vector<float> id_f(128);
	
		for(int k = 0; k < 128; k++)
		{
			id_f[k] = idd[k][index];
		}

		float x1 =  (id_w + o1 + 0.5) * 4 - s0 / 2 > 0.f ? (id_w + o1 + 0.5) * 4 - s0 / 2 : 0;
		float y1 =  (id_h + o0 + 0.5) * 4 - s1 / 2 > 0 ? (id_h + o0 + 0.5) * 4 - s1 / 2 : 0;
		float x2 = 0, y2 = 0;
		x1 = x1 < (float)d_w ? x1 : (float)d_w;
		y1 = y1 < (float)d_h ? y1 : (float)d_h;
		x2 =  x1 + s0 < (float)d_w ? x1 + s0 : (float)d_w;
		y2 = y1 + s1 < (float)d_h ? y1 + s1 : (float)d_h;
       
		ObjInfo objbox;
		objbox.x1 = x1;
		objbox.y1 = y1;
		objbox.x2 = x2;
		objbox.y2 = y2;
		objbox.label = cate_id;
		objbox.score = score;
		objbox.ids_ = id_f;
		objbox.area=(objbox.x2-objbox.x1)*(objbox.y2-objbox.y1);


		float box_w = x2 - x1; //=s1?
		float box_h = y2 - y1; //=s0?

		//std::cout << objbox.x1 << " " << objbox.y1 << " " << objbox.x2 << " " << objbox.y2 << " " << objbox.label  << std::endl;
	
		objs_tmp.push_back(objbox);
	}

	nms(objs_tmp, objs, nmsThresh);
	
	for (int k = 0; k < objs.size(); k++) {
		objs[k].x1 *= d_scale_w*scale_w;
		objs[k].y1 *= d_scale_h*scale_h;
		objs[k].x2 *= d_scale_w*scale_w;
		objs[k].y2 *= d_scale_h*scale_h;

	}
}

void Centerobj::dynamicScale(float in_w, float in_h)
{
	d_h = (int)(std::ceil(in_h / 32) * 32);
	d_w = (int)(std::ceil(in_w / 32) * 32);

	d_scale_h = in_h / d_h;
	d_scale_w = in_w / d_w;
}


void Centerobj::genIds(float * heatmap, int h, int w, int c, float thresh, std::vector<float>& ids)
{
	if (heatmap==NULL)
	{
		std::cout << "heatmap is nullptr,please check! " << std::endl;
		return;
	}
	for (int id = 0; id < c ;id ++){
		float *temp_heatmap = heatmap + h*w *id;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {

				if (temp_heatmap[ i*w + j] > thresh  ) {
					ids.push_back(i);
					ids.push_back(j);
					ids.push_back(id);
					ids.push_back(temp_heatmap[ i*w + j]);
				}
			}
		}
	}
}
