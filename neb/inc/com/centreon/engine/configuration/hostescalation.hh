/*
** Copyright 2011-2013 Merethis
**
** This file is part of Centreon Engine.
**
** Centreon Engine is free software: you can redistribute it and/or
** modify it under the terms of the GNU General Public License version 2
** as published by the Free Software Foundation.
**
** Centreon Engine is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Centreon Engine. If not, see
** <http://www.gnu.org/licenses/>.
*/

#ifndef CCE_CONFIGURATION_HOSTESCALATION_HH
#  define CCE_CONFIGURATION_HOSTESCALATION_HH

#  include <set>
#  include "com/centreon/engine/configuration/group.hh"
#  include "com/centreon/engine/configuration/object.hh"
#  include "com/centreon/engine/opt.hh"
#  include "com/centreon/engine/namespace.hh"

CCE_BEGIN()

namespace                  configuration {
  class                    hostescalation : public object {
  public:
    enum                   action_on {
      none = 0,
      down = (1 << 0),
      unreachable = (1 << 1),
      recovery = (1 << 2)
    };
    typedef hostescalation key_type;

                           hostescalation();
                           hostescalation(hostescalation const& right);
                           ~hostescalation() throw ();
    hostescalation&        operator=(hostescalation const& right);
    bool                   operator==(
                             hostescalation const& right) const throw ();
    bool                   operator!=(
                             hostescalation const& right) const throw ();
    bool                   operator<(
                             hostescalation const& right) const;
    void                   check_validity() const;
    key_type const&        key() const throw ();
    void                   merge(object const& obj);
    bool                   parse(char const* key, char const* value);

    list_string&           contactgroups() throw ();
    list_string const&     contactgroups() const throw ();
    bool                   contactgroups_defined() const throw ();
    list_string&           contacts() throw ();
    list_string const&     contacts() const throw ();
    bool                   contacts_defined() const throw ();
    void                   escalation_options(
                             unsigned short options) throw ();
    unsigned short         escalation_options() const throw ();
    void                   escalation_period(std::string const& period);
    std::string const&     escalation_period() const throw ();
    bool                   escalation_period_defined() const throw ();
    void                   first_notification(unsigned int n) throw ();
    unsigned int           first_notification() const throw ();
    list_string&           hostgroups() throw ();
    list_string const&     hostgroups() const throw ();
    list_string&           hosts() throw ();
    list_string const&     hosts() const throw ();
    void                   last_notification(unsigned int n) throw ();
    unsigned int           last_notification() const throw ();
    void                   notification_interval(unsigned int interval);
    unsigned int           notification_interval() const throw ();
    bool                   notification_interval_defined() const throw ();

  private:
    struct                 setters {
      char const*          name;
      bool                 (*func)(hostescalation&, char const*);
    };

    bool                   _set_contactgroups(std::string const& value);
    bool                   _set_contacts(std::string const& value);
    bool                   _set_escalation_options(std::string const& value);
    bool                   _set_escalation_period(std::string const& value);
    bool                   _set_first_notification(unsigned int value);
    bool                   _set_hostgroups(std::string const& value);
    bool                   _set_hosts(std::string const& value);
    bool                   _set_last_notification(unsigned int value);
    bool                   _set_notification_interval(unsigned int value);

    group                  _contactgroups;
    group                  _contacts;
    opt<unsigned short>    _escalation_options;
    opt<std::string>       _escalation_period;
    opt<unsigned int>      _first_notification;
    group                  _hostgroups;
    group                  _hosts;
    opt<unsigned int>      _last_notification;
    opt<unsigned int>      _notification_interval;
    static setters const   _setters[];
  };

  typedef shared_ptr<hostescalation>   hostescalation_ptr;
  typedef std::set<hostescalation_ptr> set_hostescalation;
}

CCE_END()

#endif // !CCE_CONFIGURATION_HOSTESCALATION_HH
