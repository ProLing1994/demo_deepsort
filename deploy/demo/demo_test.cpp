#include <iostream>
#include <time.h>

#include "../deepsort.h" 

int main(int argc, char** argv) {
	deepsort::DeepSORT deep_sort;
	deep_sort.init();
	
	while (1) {
		clock_t begin, end;		
		begin = clock();

		std::vector<ObjInfo> obj_info;

		ObjInfo test_obj;

		test_obj.x1 = 10.0;
		test_obj.y1 = 10.0;
		test_obj.x2 = 100.0;
		test_obj.y2 = 100.0;
		test_obj.score = 0.9;
		test_obj.area = 10;
		test_obj.label = 1;
		float boneage_tw3carpal_male[128]={2.5,2.6,2.7,2.8,2.9,3.0,3.1,3.2,3.3,3.4,
                                       3.5,3.6,3.7,3.8,3.9,4.0,4.1,4.2,4.3,4.4,
                                       4.5,4.6,4.7,4.8,4.9,5.0,5.1,5.2,5.3,5.4,
                                       5.5,5.6,5.7,5.8,5.9,6.0,6.1,6.2,6.3,6.4,
                                       6.5,6.6,6.7,6.8,6.9,7.0,7.1,7.2,7.3,7.4,
                                       7.5,7.6,7.7,7.8,7.9,8.0,8.1,8.2,8.3,8.4,
                                       8.5,8.6,8.7,8.8,8.9,9.0,9.1,9.2,9.3,9.4,
                                       9.5,9.6,9.7,9.8,9.9,10.0,10.1,10.2,10.3,10.4,
                                       10.5,10.6,10.7,10.8,10.9,11.0,11.1,11.2,11.3,11.4,
                                       11.5,11.6,11.7,11.8,11.9,12.0,12.1,12.2,12.3,12.4,
                                       12.5,12.6,12.7,12.8,12.9,13.0,13.1,13.2,13.3,13.4,
                                       13.5,13.6,13.7,13.8,13.9,14.0,14.1,14.2,14.3,14.4,
                                       14.5,14.6,14.7,14.8,14.9,15.0,1.0,2.0};
		for(int i=0;i<128;i++)
			test_obj.ids_.push_back(boneage_tw3carpal_male[i]);
		obj_info.push_back(test_obj);

		deep_sort.load_detections(obj_info);
		deep_sort.update();

		std::vector<ObjInfo> obj_info_track;
		std::vector<int> track_id;
		deep_sort.get_results(obj_info_track, track_id);

		end = clock();
		std::cout <<"tracking time: " << 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0 << " ms." << std::endl;
	}
	return 0;
}
