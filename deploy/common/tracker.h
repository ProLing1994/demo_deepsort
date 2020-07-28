#ifndef DEEPSORT_DEPLOY_COMMON_TRACKER_H
#define DEEPSORT_DEPLOY_COMMON_TRACKER_H

#include <vector>

#include "nn_matching.h"
#include "kalmanfilter.h"
#include "track.h"
#include "../utils/model.h"

namespace deepsort {

	class tracker {
	public:
		tracker(float max_cosine_distance, 
				int nn_budget,
				float max_iou_distance,
				int max_age, 
				int n_init);

		void predict();
		void update(const DETECTIONS& detections);

		typedef DYNAMICM (tracker::* GATED_METRIC_FUNC)(
				std::vector<Track>& tracks,
				const DETECTIONS& dets,
				const std::vector<int>& track_indices,
				const std::vector<int>& detection_indices);

	public:
		float max_iou_distance;
		int max_age;
		int n_init;
		int _next_idx;//_next_idx += 1 in func _initiate_track;
		NearNeighborDisMetric* metric;
		KalmanFilter* kf;
		std::vector<Track> tracks;

	private:    
		void _match(const DETECTIONS& detections, TRACHER_MATCHD& res);
		void _initiate_track(const DETECTION_ROW& detection);
		
		DYNAMICM gated_matric(std::vector<Track>& tracks,
													const DETECTIONS& dets,
													const std::vector<int>& track_indices,
													const std::vector<int>& detection_indices);
		DYNAMICM iou_cost(std::vector<Track>& tracks,
											const DETECTIONS& dets,
											const std::vector<int>& track_indices,
											const std::vector<int>& detection_indices);
		Eigen::VectorXf iou(DETECTBOX& bbox,
												DETECTBOXSS &candidates);
	};
}
#endif // DEEPSORT_DEPLOY_COMMON_TRACKER_H
