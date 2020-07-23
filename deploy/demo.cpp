#include <iostream>
#include <sys/time.h>

#include "opencv2/opencv.hpp"
#include "net/ncnn_centernet.h"
#include "utils/model.h"
#include "deepsort.h" 

float getElapse(struct timeval *tv1,struct timeval *tv2)
{
    float t = 0.0f;
    if (tv1->tv_sec == tv2->tv_sec)
        t = (tv2->tv_usec - tv1->tv_usec)/1000.0f;
    else
        t = ((tv2->tv_sec - tv1->tv_sec) * 1000 * 1000 + tv2->tv_usec - tv1->tv_usec)/1000.0f;
    return t;
}


static void draw_objects(const cv::Mat& bgr, const std::vector<ObjInfo> obj_info, const std::vector<int> track_id,float ratio, float ratio1, int i)
{
    static const char* class_names[] = {
        "people"};

    cv::Mat image = bgr.clone();
    for (size_t i = 0; i < obj_info.size(); i++)
    {
		const ObjInfo& obj = obj_info[i];
		int id = track_id[i];
		cv::rectangle(image, cv::Point(obj.x1 /ratio, obj.y1 / ratio1), cv::Point(obj.x2 /ratio, obj.y2 /ratio1), cv::Scalar(255, 0, 0));
		char text[256];
		sprintf(text, "%d", id);
		int baseLine = 0;
		cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
		int x = obj.x1 / ratio;
		int y = obj.y1 / ratio1 - label_size.height - baseLine;
			if (y < 0)
			y = 0;
		if (x + label_size.width > image.cols)
		x = image.cols - label_size.width;

		cv::rectangle(image, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
				cv::Scalar(255, 255, 255), -1);

		cv::putText(image, text, cv::Point(x, y + label_size.height),
				cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
    }

    cv::imwrite("test" + std::to_string(i)  + ".jpg",image);
}

int main(int argc, char** argv) {
    struct timeval  tv1,tv2, tv3, tv4;
    struct timezone tz1,tz2, tz3, tz4;

	std::string model_path = argv[1];
	std::string str = argv[2];
	
	cv::VideoCapture cap;
  	cv::VideoWriter video;
	cv::Mat frame;
	
	cap.open(str);
	int i = 0;

	deepsort::DeepSORT deep_sort;
	deep_sort.init();
	while (cv::waitKey(1) < 0)
    {
      // get frame from the video
      cap >> frame;

      // Stop the program if reached end of video
      if (frame.empty())
        {
          std::cout << "Done processing !!!" <<  std::endl;
          cv::waitKey(3000);
          break;
        }

		// begin time
		gettimeofday(&tv1,&tz1);

		Centerobj centerobj;
		centerobj.init(model_path);

		float ratio = 256 *1.0 / 1280;
		float ratio1 = 192 *1.0 / 720;

		cv::Mat img_resize;
		cv::resize(frame, img_resize, cv::Size(256, 192), cv::INTER_LINEAR);
		std::vector<ObjInfo> obj_info;
		
		ncnn::Mat inmat = ncnn::Mat::from_pixels(img_resize.data, ncnn::Mat::PIXEL_RGB2BGR, img_resize.cols, img_resize.rows);
		gettimeofday(&tv2,&tz2);
		centerobj.detect(inmat, obj_info, 256, 192);

		// detect time
		gettimeofday(&tv3,&tz3);
		
		deep_sort.load_detections(obj_info);
		deep_sort.update();

		std::vector<ObjInfo> obj_info_track;
		std::vector<int> track_id;
		deep_sort.get_results(obj_info_track, track_id);

		//track time
		gettimeofday(&tv4,&tz4);

		draw_objects(frame,obj_info_track, track_id, ratio, ratio1, i);
		
		float tc_data = getElapse(&tv1, &tv2);
		float tc_detect = getElapse(&tv2, &tv3);
		float tc_track = getElapse(&tv3, &tv4);
		float tc_all = getElapse(&tv1, &tv4);
		std::cout << "数据耗时:" << tc_data << "ms" << std::endl;
		std::cout << "检测耗时:" << tc_detect << "ms" << std::endl;
		std::cout << "跟踪耗时:" << tc_track << "ms" << std::endl;
		std::cout << "总耗时:" << tc_all << "ms" << std::endl;
		i++;
	}
	return 0;
}
