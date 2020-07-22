#ifndef _DEEPSORT_DEPLOY_DEEPSORT_H_
#define _DEEPSORT_DEPLOY_DEEPSORT_H_

#include <vector>

#include "common/model.h"

namespace deepsort {

	struct OptionsDeepSORT {
		OptionsDeepSORT() {
			nn_budget = 100;
			max_cosine_distance = 0.2;
		}
		int nn_budget;
		float max_cosine_distance;
	};

	class DeepSORT {
	public:
		DeepSORT();
		~DeepSORT();

    int load_detections(std::vector<ObjInfo>& obj_info);
	private:
    OptionsDeepSORT options_deepsort_;
    std::vector<DETECTION_ROW> detections_;
	};
}
#endif // _DEEPSORT_DEPLOY_DEEPSORT_H_