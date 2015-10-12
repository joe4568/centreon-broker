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

#ifndef CCB_DUMPER_DB_DUMP_COMMITTED_HH
#  define CCB_DUMPER_DB_DUMP_COMMITTED_HH

#  include "com/centreon/broker/io/data.hh"
#  include "com/centreon/broker/namespace.hh"

CCB_BEGIN()

namespace               dumper {
  /**
   *  @class db_dump_committed db_dump_committed.hh "com/centreon/broker/dumper/db_dump_committed.hh"
   *  @brief DB dump committed event.
   *
   *  A general notification that the database just has been dumped,
   *  and endpoints should maybe reload their configurations.
   */
  class                 db_dump_committed : public io::data {
  public:
                        db_dump_committed();
                        db_dump_committed(db_dump_committed const& other);
                        ~db_dump_committed();
    db_dump_committed&  operator=(db_dump_committed const& other);
    unsigned int        type() const;

  private:
    void                _internal_copy(db_dump_committed const& other);
  };
}

CCB_END()

#endif // !CCB_DUMPER_DB_DUMP_COMMITTED_HH