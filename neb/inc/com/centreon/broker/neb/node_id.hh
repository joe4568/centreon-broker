/*
** Copyright 2011-2013 Centreon
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

#ifndef CCB_NEB_NODE_ID_HH
#  define CCB_NEB_NODE_ID_HH

#  include <QHash>
#  include "com/centreon/broker/namespace.hh"

CCB_BEGIN()

namespace         neb {
  /**
   *  @class node_id node_id.hh "com/centreon/broker/neb/node_id.hh"
   *  @brief Node id object.
   *
   *  This object contains the id of a node: a host id and an associated
   *  service id.  Both the host id or the service id can be zero for service
   *  associated with no hosts and inversely.
   */
  class           node_id {
  public:
                  node_id();
                  node_id(node_id const& obj);
    node_id&      operator=(node_id const& obj);
                  explicit node_id(unsigned int host_id,
                                   unsigned int service_id = 0);
    bool          operator<(node_id const& obj) const throw();
    bool          operator==(node_id const& obj) const throw();
    bool          operator!=(node_id const& obj) const throw();

    unsigned int  get_host_id() const throw();
    unsigned int  get_service_id() const throw();
    bool          is_host() const throw();
    bool          is_service() const throw();
    node_id       to_host() const throw();
    bool          empty() const throw();

  private:
    unsigned int  _host_id;
    unsigned int  _service_id;
  };

  // QHash function for hash and sets.
  uint qHash(node_id id);
}

CCB_END()

#endif // !CCB_NEB_NODE_ID_HH
