/*
** Copyright 2018 Centreon
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

#ifndef CCB_REDIS_STREAM_HH
#  define CCB_REDIS_STREAM_HH

#  include <memory>
#  include "com/centreon/broker/misc/shared_ptr.hh"
#  include "com/centreon/broker/io/stream.hh"

CCB_BEGIN()

namespace          redis {

  // Forward declaration
  class redisdb;

  /**
   *  @class stream stream.hh "com/centreon/broker/redis/stream.hh"
   *  @brief redis stream.
   *
   *  Stream events into redis database.
   */
  class             stream : public io::stream {
  public:
                    stream(
                      std::string const& address,
                      unsigned short port,
                      std::string const& password);
    bool            read(misc::shared_ptr<io::data>& d, time_t deadline);
    int             write(misc::shared_ptr<io::data> const& d);

  private:
    stream&         operator=(stream const& other);
                    stream(stream const& other);

    redis::redisdb* _redisdb;
    unsigned int    _pending_queries;
    unsigned int    _actual_query;
    unsigned int    _queries_per_transaction;
  };
}

CCB_END()

#endif // !CCB_REDIS_STREAM_HH
