#include <iostream>
#include <time.h>

#include "opencv2/opencv.hpp"

#include "../deepsort.h" 
#include "../net/ncnn_centernet.h"
#include "../utils/model.h"


float getElapse(struct timeval *tv1,struct timeval *tv2)
{
	float t = 0.0f;
	if (tv1->tv_sec == tv2->tv_sec)
			t = (tv2->tv_usec - tv1->tv_usec)/1000.0f;
	else
			t = ((tv2->tv_sec - tv1->tv_sec) * 1000 * 1000 + tv2->tv_usec - tv1->tv_usec)/1000.0f;
	return t;
}


static void draw_objects(const cv::Mat& bgr, 
												const std::vector<ObjInfo> obj_info, 
												const std::vector<int> track_id, 
												float ratio, 
												float ratio1, 
												int i,
												std::string output_path) {
	static const char* class_names[] = {
			"people"};

	cv::Mat image = bgr.clone();
	for (size_t i = 0; i < obj_info.size(); i++) {
		const ObjInfo& obj = obj_info[i];
		int id = track_id[i];
		cv::rectangle(image, cv::Point(obj.x1 /ratio, obj.y1 / ratio1), cv::Point(obj.x2 /ratio, obj.y2 /ratio1), cv::Scalar(255, 0, 0));
		char text[256];
		sprintf(text, "%d", id);
		int baseLine = 0;
		cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
		int x = obj.x1 / ratio;
		int y = obj.y1 / ratio1 - label_size.height - baseLine;
		if (y < 0) y = 0;
		if (x + label_size.width > image.cols) x = image.cols - label_size.width;

		cv::rectangle(image, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
				cv::Scalar(255, 255, 255), -1);

		cv::putText(image, text, cv::Point(x, y + label_size.height),
				cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0));
	}

	cv::imwrite(output_path + "test" + std::to_string(i) + ".jpg", image);
	// cv::imshow("frame", image);
	// cv::waitKey(1);
}

int main(int argc, char** argv) {
	std::string model_path = argv[1];
	std::string video_path = argv[2];
	std::string output_path = argv[3];
	
	cv::VideoCapture cap;
	// cv::VideoWriter video;
	cv::Mat frame;
	int frame_id = 0;
	float ratio = 256 *1.0 / 1280;
	float ratio1 = 192 *1.0 / 720;

	// init 
	Centerobj centerobj;
	centerobj.init(model_path);
	deepsort::DeepSORT deep_sort;
	deep_sort.init();
	cap.open(video_path);
	
	while (1) {

		clock_t begin, end;	
		begin = clock();
		float total_time = 0.;

		cv::Mat img_resize;
		std::vector<ObjInfo> obj_info;

		// get frame from the video
		cap >> frame;

		// Stop the program if reached end of video
		if (frame.empty())
		{
			std::cout << "Done processing !!!" <<  std::endl;
			std::cout << "Output file is stored to " << output_path <<  std::endl;
			// cv::waitKey(3000);
			break;
		}

		cv::resize(frame, img_resize, cv::Size(256, 192), cv::INTER_LINEAR);
		ncnn::Mat inmat = ncnn::Mat::from_pixels(img_resize.data, ncnn::Mat::PIXEL_RGB2BGR, img_resize.cols, img_resize.rows);

		end = clock();
		std::cout <<"loading time: " << 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0 << " ms." << std::endl;
		total_time += 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0;
		begin = clock();

		centerobj.detect(inmat, obj_info, 256, 192);

		end = clock();
		std::cout <<"detecting time: " << 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0 << " ms." << std::endl;
		total_time += 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0;
		begin = clock();

		deep_sort.load_detections(obj_info);
		deep_sort.update();

		std::vector<ObjInfo> obj_info_track;
		std::vector<int> track_id;
		deep_sort.get_results(obj_info_track, track_id);

		end = clock();
		std::cout <<"tracking time: " << 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0 << " ms." << std::endl;
		total_time += 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0;

		draw_objects(frame, obj_info_track, track_id, ratio, ratio1, frame_id, output_path);

		std::cout << "total time:" << total_time << "ms" << std::endl;

		frame_id++;
	}
	return 0;
}
