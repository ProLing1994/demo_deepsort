#ifndef DEEPSORT_DEPLOY_DEEPSORT_H_
#define DEEPSORT_DEPLOY_DEEPSORT_H_

#include <memory>
#include <vector>

#include "common/tracker.h"
#include "utils/model.h"

typedef struct ObjInfo {
	float x1;
	float y1;
	float x2;
	float y2;
	float score;
	float area;
	int label;
	std::vector<float> ids_;
}ObjInfo;


namespace deepsort {

	struct OptionsDeepSORT {
		OptionsDeepSORT() {
			nn_budget = 100;//len(feature);
			max_cosine_distance = 0.6;
			max_iou_distance = 0.7;
			max_age = 70;
			n_init = 3;
		}
		int nn_budget;
		float max_cosine_distance;
		float max_iou_distance;
		int max_age;
		int n_init;
	};

	class DeepSORT {
	public:
		DeepSORT();
		~DeepSORT();

		int init();
		int load_detections(std::vector<ObjInfo>& obj_info);
		int update();
		int get_results(std::vector<ObjInfo>& obj_info, std::vector<int>& obj_id);
	private:
    OptionsDeepSORT options_deepsort_;
		std::unique_ptr<tracker> tracker_;
    DETECTIONS detections_;
	};
}
#endif // DEEPSORT_DEPLOY_DEEPSORT_H_