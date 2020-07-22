#ifndef DEEPSORT_DEPLOY_COMMON_HUNGARIANOPER_H
#define DEEPSORT_DEPLOY_COMMON_HUNGARIANOPER_H

#include "munkres.h"
#include "datatype.h"

namespace deepsort {

  class HungarianOper {
  public:
      static Eigen::Matrix<float, -1, 2, Eigen::RowMajor> Solve(const DYNAMICM &cost_matrix);
  };
}
#endif // DEEPSORT_DEPLOY_COMMON_HUNGARIANOPER_H
