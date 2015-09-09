/*
** Copyright 2011-2014 Centreon
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

#ifndef CCB_NOTIFICATION_BUILDERS_COMPOSED_NOTIFICATION_RULE_BUILDER_HH
#  define CCB_NOTIFICATION_BUILDERS_COMPOSED_NOTIFICATION_RULE_BUILDER_HH

#  include <vector>
#  include "com/centreon/broker/namespace.hh"
#  include "com/centreon/broker/notification/builders/notification_rule_builder.hh"
#  include "com/centreon/broker/notification/builders/composed_builder.hh"

CCB_BEGIN()

namespace         notification {
  /**
   *  @class composed_notification_rule_builder composed_notification_rule_builder.hh "com/centreon/broker/notification/builders/composed_notification_rule_builder.hh"
   *  @brief Composed notification rule builder.
   *
   *  This class forward its method call to several other builders.
   */
  class           composed_notification_rule_builder
                    : public composed_builder<notification_rule_builder> {
  public:
                  composed_notification_rule_builder();

    virtual void  add_rule(
                    unsigned int rule_id,
                    objects::notification_rule::ptr rule);
  };

}

CCB_END()

#endif // !CCB_NOTIFICATION_BUILDERS_COMPOSED_NOTIFICATION_RULE_BUILDER_HH
