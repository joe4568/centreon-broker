/*
** Copyright 2011-2013 Merethis
**
** This file is part of Centreon Broker.
**
** Centreon Broker is free software: you can redistribute it and/or
** modify it under the terms of the GNU General Public License version 2
** as published by the Free Software Foundation.
**
** Centreon Broker is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Centreon Broker. If not, see
** <http://www.gnu.org/licenses/>.
*/

#include "com/centreon/broker/misc/string.hh"
#include "com/centreon/broker/notification/objects/dependency.hh"

using namespace com::centreon::broker::notification::objects;

const dependency::name_to_action dependency::_service_actions[] =
{{"o", service_ok},
 {"w", service_warning},
 {"u", service_unknown},
 {"c", service_critical},
 {"p", service_pending}};

const dependency::name_to_action dependency::_host_actions[] =
{{"o", host_up},
 {"d", host_down},
 {"u", host_unreachable},
 {"p", host_pending}};

/**
 *  Default constructor.
 */
dependency::dependency() :
  _type(unknown),
  _dependency_type(unknown_dependency),
  _execution_failure_options(none),
  _inherits_parent(false),
  _notification_failure_options(none) {}

/**
 *  Copy constructor.
 *
 *  @param obj  The object to copy.
 */
dependency::dependency(dependency const& obj) {
  dependency::operator=(obj);
}

/**
 *  Assignment operator.
 *
 *  @param obj  The object to copy.
 *
 *  @return     A reference to this object.
 */
dependency& dependency::operator=(dependency const& obj) {
  if (this != &obj) {
    _type = obj._type;
    _dependency_period = obj._dependency_period;
    _dependency_type = obj._dependency_type;
    _execution_failure_options = obj._execution_failure_options;
    _inherits_parent = obj._inherits_parent;
    _notification_failure_options = obj._notification_failure_options;
  }
  return (*this);
}

/**
 *  Return true if this dependency is a host dependency.
 *
 *  @return  True if this dependency is a host dependency.
 */
bool dependency::is_host_dependency() const throw() {
  return (_type == host);
}

/**
 *  Set if this dependency is a host dependency.
 *
 *  @param val  The new value of the host dependency flag.
 */
void dependency::set_is_host_dependency(bool val) throw() {
  if (val)
    _type = host;
  else
    _type = service;
}

/**
 *  Return true if this dependency is a service dependency.
 *
 *  @return  True if this dependency is a service dependency.
 */
bool dependency::is_service_dependency() const throw() {
  return (_type == service);
}

/**
 *  Set if this dependency is a service dependency.
 *
 *  @param val  The new value of the service dependency flag.
 */
void dependency::set_is_service_dependency(bool val) throw() {
  if (val)
    _type = service;
  else
    _type = host;
}

/**
 *  Get the period of the dependency.
 *
 *  @return  The name of period of the dependency.
 */
std::string const& dependency::get_period() const throw() {
  return (_dependency_period);
}

/**
 *  Set the period of the dependency.
 *
 *  @param val  The new value of the period of the dependency.
 */
void dependency::set_period(std::string const& val) {
  _dependency_period = val;
}

/**
 *  Get the kind of this dependency.
 *
 *  @return  The kind of this dependency.
 */
dependency::dependency_kind dependency::get_kind() const throw() {
  return (_dependency_type);
}

/**
 *  Set the kind of this dependency.
 *
 *  @param val  The new value of the kind of this dependency.
 */
void dependency::set_kind(dependency::dependency_kind val) throw() {
  _dependency_type = val;
}

/**
 *  Get the notification failure options.
 *
 *  @return  The notification failure options.
 */
dependency::action_on
              dependency::get_notification_failure_options() const throw() {
  return (_notification_failure_options);
}

/**
 *  Set the notification failure options.
 *
 *  @param val  The new value of the notification failure options.
 */
void dependency::set_notification_failure_options(action_on val) throw() {
  _notification_failure_options = val;
}

/**
 *  Add a notification failure option to the set.
 *
 *  @param val  A notification failure option to add to the set.
 */
void dependency::set_notification_failure_option(action_on val) throw() {
  _notification_failure_options = (action_on)(_notification_failure_options | val);
}

/**
 *  Check if a notification failure option is set.
 *
 *  @param val  The notification failure option to check.
 *
 *  @return     True if the notification failure option is set.
 */
bool dependency::is_notification_failure_option_set(
                   action_on val) const throw() {
  return (_notification_failure_options & val);
}

/**
 *  Return true if the dependency inherit from its parent.
 *
 *  @return  True if the dependency inherit from its parent.
 */
bool dependency::get_inherits_parent() const throw() {
  return (_inherits_parent);
}

/**
 *  Set if the dependency inherit from its parent.
 *
 *  @param val  The new value of the inherit from its parent flag.
 */
void dependency::set_inherits_parent(bool val) throw() {
  _inherits_parent = val;
}

dependency::action_on
  dependency::get_execution_failure_options() const throw() {
  return (_execution_failure_options);
}

void dependency::set_execution_failure_options(action_on val) throw() {
  _execution_failure_options = val;
}

void dependency::set_execution_failure_option(action_on val) throw() {
  _execution_failure_options = (action_on)(_execution_failure_options | val);
}

bool dependency::is_execution_failure_option_set(action_on val) throw() {
  return (_execution_failure_options & val);
}

void dependency::parse_notification_failure_options(std::string const& line) {
  _parse_failure_options(
    line,
    &dependency::set_notification_failure_option);
}

void dependency::parse_execution_failure_options(std::string const& line) {
  _parse_failure_options(
    line,
    &dependency::set_execution_failure_option);
}

void dependency::_parse_failure_options(
       std::string const& line,
       void (dependency::*func_to_call)(action_on)) {
  // The options are type dependent. If we don't know our own type, then
  // we shouldn't try to parse anything to begin with.
  if (_type == unknown)
    return;

  if (line == "n")
    set_notification_failure_options(none);
  else {
    std::vector<std::string> tokens;
    misc::string::split(line, tokens, ',');

    static int host_actions_size = sizeof(_host_actions) / sizeof(_host_actions[0]);
    static int service_actions_size = sizeof(_service_actions) / sizeof(_service_actions[0]);
    if (_type == host) {
      for (std::vector<std::string>::const_iterator it(tokens.begin()),
                                                    end(tokens.end());
           it != end;
           ++it) {
        for (int i = 0; i < host_actions_size; ++i) {
          if (*it == _host_actions[i].name)
            (this->*func_to_call)(_host_actions[i].action);
        }
      }
    }
    else {
      for (std::vector<std::string>::const_iterator it(tokens.begin()),
                                                    end(tokens.end());
           it != end;
           ++it) {
        for (int i = 0; i < service_actions_size; ++i) {
          if (*it == _service_actions[i].name)
            (this->*func_to_call)(_service_actions[i].action);
        }
      }
    }
  }
}
