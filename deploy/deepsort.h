#ifndef DEEPSORT_DEPLOY_DEEPSORT_H_
#define DEEPSORT_DEPLOY_DEEPSORT_H_

#include <memory>
#include <vector>

#include "common/tracker.h"
#include "utils/model.h"
#include "net/ncnn_centernet.h"

namespace deepsort {

	struct OptionsDeepSORT {
		OptionsDeepSORT() {
			nn_budget = 100;
			max_cosine_distance = 0.2;
			max_iou_distance = 0.7;
			max_age = 30;
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