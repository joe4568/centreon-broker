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

#ifndef CCB_MYSQL_BIND_HH
#  define CCB_MYSQL_BIND_HH

#  include <mysql.h>
#  include <string>
#  include <vector>
#  include "com/centreon/broker/io/data.hh"
#  include "com/centreon/broker/namespace.hh"
#  include "com/centreon/broker/misc/unordered_hash.hh"

CCB_BEGIN()

// Forward declarations
class                       mysql;

class                       mysql_bind {
 public:
                            mysql_bind();
                            mysql_bind(int size);
                            mysql_bind(mysql_bind const& other);
                            ~mysql_bind();
  void                      set_size(int size);
  void                      set_value_as_i32(
                              int range,
                              int value);
  void                      set_value_as_u32(
                              int range,
                              unsigned int value);
  void                      set_value_as_u64(
                              int range,
                              unsigned long long value);
  void                      set_value_as_bool(
                              int range,
                              bool value);
  void                      set_value_as_f64(
                              int range,
                              double value);
  void                      set_value_as_null(int range);
  void                      set_value_as_tiny(int range, char value);
  void                      set_value_as_str(
                              int range,
                              std::string const& value);
  void                      set_value_as_f32(int range, float value);
  int                       get_size() const;

  MYSQL_BIND const*         get_bind() const;
 private:
  std::vector<MYSQL_BIND>   _bind;

  // The buffers contained by _bind
  std::vector<std::string>  _buffer;

  // The buffers lengths contained by _bind
  std::vector<unsigned long int> _length;
};

CCB_END()

#endif  //CCB_MYSQL_BIND_HH
