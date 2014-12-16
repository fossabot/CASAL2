/**
 * @file TimeStepProportionsAtAge.cpp
 * @author Scott Rasmussen (scott.rasmussen@zaita.com)
 * @github https://github.com/Zaita
 * @date 17/10/2014
 * @section LICENSE
 *
 * Copyright NIWA Science �2014 - www.niwa.co.nz
 *
 */

// headers
#include "TimeStepProportionsAtAge.h"

#include "TimeSteps/Manager.h"
#include "Utilities/DoubleCompare.h"

// namespaces
namespace niwa {
namespace observations {

/**
 *
 */
TimeStepProportionsAtAge::TimeStepProportionsAtAge() {
  parameters_.Bind<Double>(PARAM_TIME_STEP_PROPORTION, &time_step_proportion_, "Proportion through the time step to analyse the partition from", "", Double(0.5));

  mean_proportion_method_ = true;
}

/**
 *
 */
void TimeStepProportionsAtAge::DoBuild() {
  ProportionsAtAge::DoBuild();

  if (time_step_proportion_ < 0.0 || time_step_proportion_ > 1.0)
    LOG_ERROR(parameters_.location(PARAM_TIME_STEP_PROPORTION) << ": time_step_proportion (" << AS_DOUBLE(time_step_proportion_) << ") must be between 0.0 and 1.0");
  proportion_of_time_ = time_step_proportion_;

  TimeStepPtr time_step = timesteps::Manager::Instance().GetTimeStep(time_step_label_);
  if (!time_step)
    LOG_ERROR(parameters_.location(PARAM_TIME_STEP) << time_step_label_ << " could not be found. Have you defined it?");

  for (unsigned year : years_)
    time_step->SubscribeToBlock(shared_ptr(), year);
}

} /* namespace observations */
} /* namespace niwa */
