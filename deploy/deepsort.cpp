#include "deepsort.h"

namespace deepsort {
  DeepSORT::DeepSORT() {
    options_deepsort_ = OptionsDeepSORT();
    detections_.clear();
  }

  DeepSORT::~DeepSORT() {
  }

  DETECTIONS load_detections(std::vector<ObjInfo>& obj_info); {
      DETECTIONS detections;
      for(int i = 0; i < obj_info.size(); i++)
      {
        DETECTION_ROW box;
        box.tlwh = DETECTBOX(obj_info[i].x1, obj_info[i].y1, obj_info[i].x2 - obj_info[i].x1, obj_info[i].y2 - obj_info[i].y1);    
        for(int j = 0; j < 128; j++)
        {
          box.feature[j] = obj_info[i].ids_[j];
        }
        detections.push_back(box);
      }
      return detections;

  }
}