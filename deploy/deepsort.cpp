#include "deepsort.h"

namespace deepsort {
  DeepSORT::DeepSORT() {
    options_deepsort_ = OptionsDeepSORT();
    detections_.clear();
  }

  DeepSORT::~DeepSORT() {
  }

  int DeepSORT::load_detections(float* net_outputs) {
    
  }
}