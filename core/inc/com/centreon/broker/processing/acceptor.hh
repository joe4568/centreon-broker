/*
** Copyright 2015 Centreon
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

#ifndef CCB_PROCESSING_ACCEPTOR_HH
#  define CCB_PROCESSING_ACCEPTOR_HH

#  include <ctime>
#  include <string>
#  include "com/centreon/broker/misc/shared_ptr.hh"
#  include "com/centreon/broker/misc/unordered_hash.hh"
#  include "com/centreon/broker/namespace.hh"
#  include "com/centreon/broker/processing/thread.hh"

CCB_BEGIN()

// Forward declaration.
namespace       io {
  class         endpoint;
}

namespace       processing {
  // Forward declaration.
  class         feeder;

  /**
   *  @class acceptor acceptor.hh "com/centreon/broker/processing/acceptor.hh"
   *  @brief Accept incoming connections.
   *
   *  Accept incoming connections and launch a feeder thread.
   */
  class         acceptor : public thread {
  public:
                acceptor(
                  misc::shared_ptr<io::endpoint> endp,
                  std::string const& name);
                ~acceptor();
    void        accept();
    void        exit();
    void        run();
    void        set_read_filters(uset<unsigned int> const& filters);
    void        set_retry_interval(time_t retry_interval);
    void        set_write_filters(uset<unsigned int> const& filters);

  private:
                acceptor(acceptor const& other);
    acceptor&   operator=(acceptor const& other);
    void        _wait_feeders();

    misc::shared_ptr<io::endpoint>
                _endp;
    std::list<misc::shared_ptr<processing::feeder> >
                _feeders;
    std::string _name;
    uset<unsigned int>
                _read_filters;
    time_t      _retry_interval;
    uset<unsigned int>
                _write_filters;
  };
}

CCB_END()

#endif // !CCB_PROCESSING_ACCEPTOR_HH
