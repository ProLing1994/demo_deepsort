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

  int DeepSORT::load_detections(std::vector<ObjInfo>& obj_info) {
    for(int i = 0; i < obj_info.size(); i++)
    {
      DETECTION_ROW box;
      box.tlwh = DETECTBOX(obj_info[i].x1, obj_info[i].y1, obj_info[i].x2 - obj_info[i].x1, obj_info[i].y2 - obj_info[i].y1);    
      for(int j = 0; j < 128; j++)
      {
        box.feature[j] = obj_info[i].ids_[j];
      }
      detections_.push_back(box);
    }
    return 0;
  }

  int DeepSORT::update() {
    tracker_->predict();
    tracker_->update(detections_);
  }

  int DeepSORT::get_results(std::vector<ObjInfo>* obj_info, std::vector<int>* obj_id) {
    for(Track& track : tracker_->tracks) {
      if(!track.is_confirmed() || track.time_since_update > 1) continue;
      obj_id->push_back(track.track_id);
      // TO DO: track.to_tlwh() -> obj_info
    }
  }

}