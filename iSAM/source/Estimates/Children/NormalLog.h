/**
 * @file NormalLog.h
 * @author  Scott Rasmussen (scott.rasmussen@zaita.com)
 * @version 1.0
 * @date 8/03/2013
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
#ifndef ESTIMATES_NORMALLOG_H_
#define ESTIMATES_NORMALLOG_H_

// Headers
#include "Estimates/Estimate.h"

// Namespaces
namespace isam {
namespace estimates {

/**
 * Class definition
 */
class NormalLog : public isam::Estimate {
public:
  // Methods
  NormalLog();
  virtual                     ~NormalLog() = default;
  void                        DoValidate() override final;
  Double                      GetScore(Double param) override final;

private:
  // Members
  Double                      mu_;
  Double                      sigma_;
};

} /* namespace estimates */
} /* namespace isam */
#endif /* PRIORS_NORMALLOG_H_ */