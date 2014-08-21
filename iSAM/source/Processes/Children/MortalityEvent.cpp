/**
 * @file MortalityEvent.cpp
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 21/12/2012
 * @section LICENSE
 *
 * Copyright NIWA Science �2012 - www.niwa.co.nz
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */

// Headers
#include "MortalityEvent.h"

#include "Penalties/Manager.h"
#include "Selectivities/Manager.h"
#include "Utilities/DoubleCompare.h"

// Namespaces
namespace isam {
namespace processes {

/**
 * Default Constructor
 */
MortalityEvent::MortalityEvent() {
  is_mortality_process = true;

  parameters_.Bind<string>(PARAM_CATEGORIES, &category_names_, "Categories", "");
  parameters_.Bind<unsigned>(PARAM_YEARS, &years_, "Years", "");
  parameters_.Bind<double>(PARAM_CATCHES, &catches_, "Catches", "");
  parameters_.Bind<double>(PARAM_U_MAX, &u_max_, "U Max", "", 0.99);
  parameters_.Bind<string>(PARAM_SELECTIVITIES, &selectivity_names_, "List of selectivities", "");
  parameters_.Bind<string>(PARAM_PENALTY, &penalty_name_, "Penalty label", "", "");

  RegisterAsEstimable(PARAM_U_MAX, &u_max_);
  RegisterAsEstimable(PARAM_CATCHES, &catch_years_);

  model_ = Model::Instance();
}

/**
 * Validate our Mortality Event Process
 *
 * 1. Check for the required parameters
 * 2. Assign any remaining variables
 */
void MortalityEvent::DoValidate() {
  // Validate that our number of years_ and catches_ vectors are the same size
  if (years_.size() != catches_.size()) {
    LOG_ERROR(parameters_.location(PARAM_CATCHES)
        << ": Number of catches_ provided does not match the number of years_ provided."
        << " Expected " << years_.size() << " but got " << catches_.size());
  }

  // Validate that the number of selectivities is the same as the number of categories
  if (category_names_.size() != selectivity_names_.size()) {
    LOG_ERROR(parameters_.location(PARAM_SELECTIVITIES)
        << " Number of selectivities provided does not match the number of categories provided."
        << " Expected " << category_names_.size() << " but got " << selectivity_names_.size());
  }

  // Validate: catches_ and years_
  for(unsigned i = 0; i < years_.size(); ++i) {
    if (catch_years_.find(years_[i]) != catch_years_.end()) {
      LOG_ERROR(parameters_.location(PARAM_YEARS) << " year " << years_[i] << " has already been specified, please remove the duplicate");
    }

    catch_years_[years_[i]] = catches_[i];
  }

  // Validate u_max
  if (u_max_ < 0.0 || u_max_ > 1.0)
    LOG_ERROR(parameters_.location(PARAM_U_MAX) << ": u_max must be between 0.0 and 1.0 (inclusive). Value defined was " << AS_DOUBLE(u_max_));
}

/**
 * Build the runtime relationships required
 * - Build partition reference
 */
void MortalityEvent::DoBuild() {
  partition_.Init(category_names_);

  for (string label : selectivity_names_) {
    SelectivityPtr selectivity = selectivities::Manager::Instance().GetSelectivity(label);
    if (!selectivity)
      LOG_ERROR(parameters_.location(PARAM_SELECTIVITIES) << ": selectivity " << label << " does not exist. Have you defined it?");

    selectivities_.push_back(selectivity);
  }

  if (penalty_name_ != "") {
    penalty_ = penalties::Manager::Instance().GetPenalty(penalty_name_);
    if (!penalty_) {
      LOG_ERROR(parameters_.location(PARAM_PENALTY) << ": penalty " << penalty_name_ << " does not exist. Have you defined it?");
    }
  }
}

/**
 * Execute our mortality event object.
 *
 */
void MortalityEvent::Execute() {
  if (catch_years_.find(model_->current_year()) == catch_years_.end())
    return;

  /**
   * Work our how much of the stock is vulnerable
   */
  Double vulnerable = 0.0;
  unsigned i = 0;
  for (auto categories : partition_) {
    unsigned j = 0;
    for (Double& data : categories->data_) {
      Double local_vulnerable = data * selectivities_[i]->GetResult(categories->min_age_ + j);
      vulnerable += local_vulnerable;
      vulnerable_[categories->name_][categories->min_age_ + j] = local_vulnerable;
      ++j;
    }

    ++i;
  }
//  for (auto iterator = partition_->begin(); iterator != partition_->end(); ++iterator, ++i) {
//    unsigned min_age = (*iterator)->min_age_;
//
//    for (unsigned offset = 0; offset < (*iterator)->data_.size(); ++offset) {
//      Double temp = (*iterator)->data_[offset] * selectivities_[i]->GetResult(min_age + offset);
//
//      vulnerable = vulnerable + temp;
//      vulnerable_[(*iterator)->name_][min_age + offset] = temp;
//    }
//  }

  /**
   * Work out the exploitation rate to remove (catch/vulnerable)
   */
  Double exploitation = catch_years_[model_->current_year()] / utilities::doublecompare::ZeroFun(vulnerable);
  if (exploitation > u_max_) {
    exploitation = u_max_;
    if (penalty_)
      penalty_->Trigger(label_, catch_years_[model_->current_year()], vulnerable*u_max_);

  } else if (exploitation < 0.0) {
    exploitation = 0.0;
  }
  LOG_INFO("year: " << model_->current_year() << "; exploitation: " << AS_DOUBLE(exploitation));

  /**
   * Remove the stock now. The amount to remove is
   * vulnerable * exploitation
   */
  for (auto categories : partition_) {
    unsigned offset = 0;
    for (Double& data : categories->data_) {
      data -= vulnerable_[categories->name_][categories->min_age_ + offset] * exploitation;
      offset++;
    }
  }

//
//  for (auto iterator = partition_->Begin(); iterator != partition_->End(); ++iterator) {
//    unsigned min_age = (*iterator)->min_age_;
//
//    for (unsigned offset = 0; offset < (*iterator)->data_.size(); ++offset) {
//      Double temp = vulnerable_[(*iterator)->name_][min_age + offset] * exploitation;
//      (*iterator)->data_[offset] -= temp;
//    }
//  }
}


} /* namespace processes */
} /* namespace isam */
