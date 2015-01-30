/**
 * @file Manager.h
 * @author Scott Rasmussen (scott.rasmussen@zaita.com)
 * @github https://github.com/Zaita
 * @date 27/01/2015
 * @section LICENSE
 *
 * Copyright NIWA Science �2014 - www.niwa.co.nz
 *
 * @section DESCRIPTION
 *
 * << Add Description >>
 */
#ifndef TIMEVARYING_MANAGER_H_
#define TIMEVARYING_MANAGER_H_

// Headers
#include "BaseClasses/Manager.h"
#include "TimeVarying/TimeVarying.h"

// Namespaces
namespace niwa {
namespace timevarying {

/**
 * Class Definition
 */
class Manager : public niwa::oldbase::Manager<niwa::timevarying::Manager, niwa::TimeVarying> {
  friend class niwa::oldbase::Manager<niwa::timevarying::Manager, niwa::TimeVarying>;
public:
  // methods
  virtual                     ~Manager() = default;
  void                        Update(unsigned current_year);

protected:
  // methods
  Manager();
};

} /* namespace timevarying */
} /* namespace niwa */
#endif /* TIMEVARYING_MANAGER_H_ */