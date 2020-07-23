#include "deepsort.h"

namespace deepsort {
  
  DeepSORT::DeepSORT() {
    options_deepsort_ = OptionsDeepSORT();
    detections_.clear();
  }

  DeepSORT::~DeepSORT() {
  }

  int DeepSORT::init() {
    tracker_.reset(new tracker(options_deepsort_.max_cosine_distance,
                               options_deepsort_.nn_budget,
                               options_deepsort_.max_iou_distance,
                               options_deepsort_.max_age,
                               options_deepsort_.n_init));
    return 0;
  }

  int DeepSORT::load_detections(float* net_outputs) {
    
  }

  int DeepSORT::update() {
    tracker_->predict();
    tracker_->update(detections_);
  }
}