/*
** Copyright 2013 Centreon
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** For more information : contact@centreon.com
*/

#include <iomanip>
#include <sstream>
#include "com/centreon/broker/config/applier/state.hh"
#include "com/centreon/broker/neb/internal.hh"
#include "com/centreon/broker/neb/statistics/active_service_execution_time.hh"
#include "com/centreon/broker/neb/statistics/compute_value.hh"
#include "com/centreon/engine/globals.hh"

using namespace com::centreon::broker;
using namespace com::centreon::broker::neb;
using namespace com::centreon::broker::neb::statistics;

/**
 *  Default constructor.
 */
active_service_execution_time::active_service_execution_time()
  : plugin("active_service_execution_time") {}

/**
 *  Copy constructor.
 *
 *  @param[in] right Object to copy.
 */
active_service_execution_time::active_service_execution_time(active_service_execution_time const& right)
 : plugin(right) {}

/**
 *  Destructor.
 */
active_service_execution_time::~active_service_execution_time() {}

/**
 *  Assignment operator.
 *
 *  @param[in] right Object to copy.
 *
 *  @return This object.
 */
active_service_execution_time& active_service_execution_time::operator=(active_service_execution_time const& right) {
  plugin::operator=(right);
  return (*this);
}

/**
 *  Get statistics.
 *
 *  @param[out] output   The output return by the plugin.
 *  @param[out] perfdata The perf data return by the plugin.
 */
void active_service_execution_time::run(
              std::string& output,
	      std::string& perfdata) {
  compute_value<double> cv;
  for (service* s(service_list); s; s = s->next)
    if (s->check_type == SERVICE_CHECK_ACTIVE)
      cv << s->execution_time;

  if (cv.size()) {
    // Output.
    std::ostringstream oss;
    oss << "Engine " << config::applier::state::instance().poller_name()
        << " has an average active service execution time of "
        << std::fixed << std::setprecision(2) << cv.avg() << "s";
    output = oss.str();

    // Perfdata.
    oss.str("");
    oss << "avg=" << cv.avg() << "s min=" << cv.min()
        << "s max=" << cv.max() << "s";
    perfdata = oss.str();
  }
  else {
    // Output.
    output = "No active service to compute active service "
      "execution time on " + config::applier::state::instance().poller_name();
  }
  return ;
}
