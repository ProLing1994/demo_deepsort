#ifndef _DEEPSORT_DEPLOY_COMMON_MODEL_H
#define _DEEPSORT_DEPLOY_COMMON_MODEL_H

#include "datatype.h"


// * Each rect's data structure.
// * tlwh: topleft point & (w,h)
// * confidence: detection confidence.
// * feature: the rect's 128d feature.
// */
class DETECTION_ROW
{
public:
    DETECTBOX tlwh;
    float confidence;
    FEATURE feature;
    DETECTBOX to_xyah() const;
    DETECTBOX to_tlbr() const;
};

#endif // _DEEPSORT_DEPLOY_COMMON_MODEL_H
