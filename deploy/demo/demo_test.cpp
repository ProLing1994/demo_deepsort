#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "../deepsort.h" 

int main(int argc, char** argv) {
	deepsort::DeepSORT deep_sort;
	deep_sort.init();
	int loop_time = 0;

	ObjInfo test_obj;
	std::vector<ObjInfo> obj_info;

	while (1) {
		clock_t begin, end;		
		begin = clock();

		test_obj.x1 = rand() % 100;
		test_obj.y1 = rand() % 100;
		test_obj.x2 = rand() % 100 + test_obj.x1;
		test_obj.y2 = rand() % 100 + test_obj.y1;
		test_obj.score = rand() % 10 / 10.;
		test_obj.area = 10;
		test_obj.label = 1;
		for(int i=0; i<128; i++)
			test_obj.ids_.push_back(rand() % 100 / 100.);

		obj_info.push_back(test_obj);

		deep_sort.load_detections(obj_info);
		deep_sort.update();

		std::vector<ObjInfo> obj_info_track;
		std::vector<int> track_id;
		deep_sort.get_results(obj_info_track, track_id);

		end = clock();
		std::cout << "loop_time: "<< loop_time << ", tracking time: " << 1.0*(end - begin) / CLOCKS_PER_SEC * 1000.0 << " ms." << std::endl;
		if(not track_id.empty()) {
			std::cout << "track id: " << track_id[0] << std::endl;
		}
		loop_time += 1;
		if(loop_time % 10 == 0) {
			obj_info.clear();
		}
	}
	return 0;
}
