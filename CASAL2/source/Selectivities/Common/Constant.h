/**
 * @file Constant.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 11/01/2013
 * @section LICENSE
 *
 * Copyright NIWA Science �2013 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * The time class represents a moment of time.
 *
 * $Date: 2008-03-04 16:33:32 +1300 (Tue, 04 Mar 2008) $
 */
#ifndef CONSTANT_H_
#define CONSTANT_H_

// Headers
#include "Selectivities/Selectivity.h"

// Namespacaes
namespace niwa {
namespace selectivities {

/**
 * Class Definition
 */
class Constant : public niwa::Selectivity {
public:
  // Methods
  explicit Constant(Model* model);
  virtual                     ~Constant() = default;
  void                        DoValidate() override final { };
  Double                      GetAgeResult(unsigned age, AgeLength* age_length) override final;
  Double                      GetLengthResult(unsigned length_bin) override final;
  void                        RebuildCache() override final;

protected:
  //Methods
  Double                      GetLengthBasedResult(unsigned age, AgeLength* age_length, unsigned year = 0, int time_step_index = -1) override final;

private:
  // Members
  Double                      c_;
};

} /* namespace selectivities */
} /* namespace niwa */
#endif /* CONSTANT_H_ */
