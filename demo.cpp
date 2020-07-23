#include <iostream>
#include <opencv2/opencv.hpp>
#include "ncnn_centernet.h"
#include <sys/time.h>
#include "model.h"

float getElapse(struct timeval *tv1,struct timeval *tv2)
{
    float t = 0.0f;
    if (tv1->tv_sec == tv2->tv_sec)
        t = (tv2->tv_usec - tv1->tv_usec)/1000.0f;
    else
        t = ((tv2->tv_sec - tv1->tv_sec) * 1000 * 1000 + tv2->tv_usec - tv1->tv_usec)/1000.0f;
    return t;
}


static void draw_objects(const cv::Mat& bgr, const std::vector<ObjInfo> obj_info, float ratio, float ratio1, int i)
{
    static const char* class_names[] = {
        "people"};

    cv::Mat image = bgr.clone();
    for (size_t i = 0; i < obj_info.size(); i++)
    {
        std::cout<<obj_info[i].score<<" "<<obj_info[i].x1<<" "<<obj_info[i].y1<<" "<<obj_info[i].x2<<" "<<obj_info[i].y2<<" "<<std::endl;
    }


    for (size_t i = 0; i < obj_info.size(); i++)
    {
	
        if (obj_info[i].score > 0.2) 
			{
				const ObjInfo& obj = obj_info[i];

				fprintf(stderr, "%d = %.5f at %.2f %.2f %.2f x %.2f\n", obj.label, obj.score,
					obj.x1, obj.y1, obj.x2, obj.y2);



				cv::rectangle(image, cv::Point(obj.x1 /ratio, obj.y1 / ratio1), cv::Point(obj.x2 /ratio, obj.y2 /ratio1), cv::Scalar(255, 0, 0));

				char text[256];
				sprintf(text, "%s %.1f%%", class_names[obj.label], obj.score * 100);

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
    }

    cv::imwrite("test" + std::to_string(i)  + ".jpg",image);
}
const char* keys =
    "{help h usage ? | | Usage examples: \n\t\t./object_detection_yolo.out --image=dog.jpg \n\t\t./object_detection_yolo.out --video=run_sm.mp4}"
    "{image i        |<none>| input image   }"
    "{video v       |<none>| input video   }"
    ;

int main(int argc, char** argv) {
	// if (argc !=3)
	// {
	// 	std::cout << " .exe mode_path image_file" << std::endl;
	// 	return -1;
	// }
    struct timeval  tv1,tv2;
    struct timezone tz1,tz2;
	//cv::CommandLineParser parser(argc, argv, keys);
  	// parser.about("Use this script to run object detection using YOLO3 in OpenCV.");
  	// if (parser.has("help"))
    // {
    // 	parser.printMessage();
    //   	return 0;
    // }

	std::string model_path = argv[1];
	std::string str = argv[2];
	//std::string str, outputFile;
	//std::string model_path = "../models";
	cv::VideoCapture cap;
  	cv::VideoWriter video;
	cv::Mat frame;
	// try 
	// {
	// 	outputFile = "yolo_out_cpp.avi";
	// 	if (parser.has("image"))
	// 	{
	// 		// Open the image file
	// 		str = parser.get<cv::String>("image");
	// 		std::ifstream ifile(str);
	// 		if (!ifile) throw("error");
	// 		cap.open(str);
	// 		str.replace(str.end()-4, str.end(), "_yolo_out_cpp.jpg");
	// 		outputFile = str;
	// 	}
	// 	else if (parser.has("video"))
	// 	{
	// 		std::cout<<"ffff"<<std::endl;
	// 		// Open the video file
	// 		str = parser.get<cv::String>("video");
	// 		std::ifstream ifile(str);
	// 		if (!ifile) throw("error");
	// 		cap.open(str);
	// 		str.replace(str.end()-4, str.end(), "_yolo_out_cpp.avi");
	// 		outputFile = str;
	// 	}
	// 	else
	// 	{
	// 		cap.open(0);
	// 	}
    // // Open the webcaom
    // // else cap.open(parser.get<int>("device"));

  	// }
	// catch(...) {
	// 	std::cout << "Could not open the input image/video stream" <<  std::endl;
	// 	return 0;
  	// }
	cap.open(str);
	int i = 0;
	while (cv::waitKey(1) < 0)
    {
      // get frame from the video
      cap >> frame;

      // Stop the program if reached end of video
      if (frame.empty())
        {
          std::cout << "Done processing !!!" <<  std::endl;
          //std::cout << "Output file is stored as " << outputFile <<  std::endl;
          cv::waitKey(3000);
          break;
        }
		
		Centerobj centerobj;

		centerobj.init(model_path);

		//cv::Mat image = cv::imread(image_file);
		
		float ratio = 256 *1.0 / 1280;
		float ratio1 = 192 *1.0 / 720;
	

		cv::Mat img_resize;
		//letterbox(image, img_resize, 192, 256);

		cv::resize(frame, img_resize, cv::Size(256, 192), cv::INTER_LINEAR);
		std::vector<ObjInfo> obj_info;
		//DETECTIONS detections;
		ncnn::Mat inmat = ncnn::Mat::from_pixels(img_resize.data, ncnn::Mat::PIXEL_RGB2BGR, img_resize.cols, img_resize.rows);

		gettimeofday(&tv1,&tz1);
		centerobj.detect(inmat, obj_info, 256, 192);
		// std::cout<<"fff"<<std::endl;
		// for(int i = 0; i < obj_info.size(); i++)
		// {
		// 	std::cout<<"fff"<<std::endl;
		// 	DETECTION_ROW box;
		// 	box.tlwh = DETECTBOX(obj_info[i].x1, obj_info[i].y1, obj_info[i].x2 - obj_info[i].x1, obj_info[i].y2 - obj_info[i].y1);
			
		// 	std::cout<<"fff"<<std::endl;
		
		// 	for(int j = 0; j < 128; j++)
		// 	{
		// 		std::cout<<obj_info[i].ids_[j]<<std::endl;
		// 		box.feature[j] = obj_info[i].ids_[j];
		// 	}
		// 	//std::cout<<"fff"<<std::endl;
		// 	detections.push_back(box);
		// }
		gettimeofday(&tv2,&tz2);
		float tc = getElapse(&tv1, &tv2);
		std::cout << "耗时：" << tc << "ms" << std::endl;

		draw_objects(frame,obj_info,ratio, ratio1, i);
		i++;
		// cv::imshow("test", image);
		// cv::waitKey();
	}
	return 0;
}

// void get_detections(DETECTBOX box,float confidence,DETECTIONS& d)
// {
//   DETECTION_ROW tmpRow;
//   tmpRow.tlwh = box;//DETECTBOX(x, y, w, h);

//   tmpRow.confidence = confidence;
//   d.push_back(tmpRow);
// }