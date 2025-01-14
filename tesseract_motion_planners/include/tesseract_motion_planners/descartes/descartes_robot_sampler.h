/**
 * @file descartes_robot_sampler.h
 * @brief Tesseract Descartes Kinematics Sampler
 *
 * @author Levi Armstrong
 * @date June 25, 2020
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2020, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_MOTION_PLANNERS_DESCARTES_ROBOT_SAMPLER_H
#define TESSERACT_MOTION_PLANNERS_DESCARTES_ROBOT_SAMPLER_H
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <descartes_light/core/waypoint_sampler.h>
#include <Eigen/Dense>
#include <vector>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_kinematics/core/inverse_kinematics.h>
#include <tesseract_environment/core/types.h>
#include <tesseract_motion_planners/descartes/descartes_utils.h>
#include <tesseract_motion_planners/descartes/descartes_collision.h>
#include <tesseract_motion_planners/descartes/types.h>

namespace tesseract_planning
{
template <typename FloatType>
class DescartesRobotSampler : public descartes_light::WaypointSampler<FloatType>
{
public:
  /**
   * @brief This is a descartes sampler for a robot.
   * @param target_pose The target pose in robot base link coordinates
   * @param target_pose_sampler The target pose sampler function to be used
   * @param robot_kinematics The robot inverse kinematics object
   * @param collision The collision interface
   * @param curret_state The currect state of the system
   * @param robot_tcp The robot tcp to be used.
   * @param allow_collision If true and no valid solution was found it will return the best of the worst
   * @param is_valid This is a user defined function to filter out solution
   */
  DescartesRobotSampler(const Eigen::Isometry3d& target_pose,
                        PoseSamplerFn target_pose_sampler,
                        tesseract_kinematics::InverseKinematics::ConstPtr ik,
                        DescartesCollision::Ptr collision,
                        const Eigen::Isometry3d& tcp,
                        bool allow_collision,
                        DescartesVertexEvaluator::Ptr is_valid,
                        std::vector<Eigen::Index> redundancy_capable_joints);

  std::vector<descartes_light::StateSample<FloatType>> sample() const override;

private:
  /** @brief The target pose to sample */
  Eigen::Isometry3d target_pose_;

  /** @brief The target pose to sample */
  PoseSamplerFn target_pose_sampler_;

  /** @brief The robot inverse kinematics */
  tesseract_kinematics::InverseKinematics::ConstPtr ik_;

  /** @brief The collision interface */
  DescartesCollision::Ptr collision_;

  /** @brief The robot tool center point */
  Eigen::Isometry3d tcp_;

  /** @brief Flag indicating whether states found to be in collision should be returned */
  bool allow_collision_;

  /** @brief The number of joints in the robot */
  int dof_;

  /** @brief The seed for inverse kinematics which is zeros */
  Eigen::VectorXd ik_seed_;

  /** @brief This is the vertex evaluator to filter out solution */
  DescartesVertexEvaluator::Ptr is_valid_;

  /** @brief Vector of indices indicating which joints in the motion group are capable of producing redundant joint
   * states */
  std::vector<Eigen::Index> redundancy_capable_joints_;
};

using DescartesRobotSamplerF = DescartesRobotSampler<float>;
using DescartesRobotSamplerD = DescartesRobotSampler<double>;

}  // namespace tesseract_planning
#endif  // TESSERACT_MOTION_PLANNERS_DESCARTES_ROBOT_SAMPLER_H
