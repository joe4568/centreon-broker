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

#include <iostream>
#include <QHostAddress>
#include <QTcpSocket>
#include <QStringList>
#include <QVariant>
#include <sstream>
#include "com/centreon/broker/exceptions/msg.hh"
#include "com/centreon/broker/logging/logging.hh"
#include "com/centreon/broker/redis/redisdb.hh"

using namespace com::centreon::broker;
using namespace com::centreon::broker::redis;

/**
 *  Default constructor.
 */
redisdb::redisdb(
           std::string const& address,
           unsigned short port,
           std::string const& password)
  : _socket(new QTcpSocket),
    _address(address),
    _port(port),
    _password(password),
    _size(0) {
  _connect();
  _check_redis_server();
  _check_redis_documents();
}

redisdb::~redisdb() {
  delete _socket;
}

void redisdb::_check_redis_server() {
  *this << "list";
  QByteArray& ret(push_command("$6\r\nmodule\r\n"));
  QVariant lst(parse(ret));
  QVariantList res(lst.toList());
  bool module_found(false);
  bool version_ok(false);

  /* Checking if redisearch module is present */
  if (res.size() >= 1) {
    for (int i(0); i < res.size(); ++i) {
      QVariantList module(res[i].toList());
      for (int j(0); j < module.size(); j += 2) {
        QString const& key(module[j].toString());
        QVariant const& value(module[j + 1]);

        if (key == "name" && value.toString() == "ft")
          module_found = true;
        else if (key == "ver" && value.toInt() >= 10007)
          version_ok = true;
      }
      if (module_found)
        break;
    }
  }
  if (!module_found || !version_ok) {
    throw (exceptions::msg()
      << "redis: The redisearch module is not installed or its version "
      << " is too old ( < 10007) on the redis server '"
      << _address << ":" << _port << "'");
  }
}

void redisdb::_check_redis_documents() {
  // Check services index
  *this << "idx:services";
  QString ret(push_command("$4\r\ntype\r\n"));
  if (ret == "+none\r\n") {
    logging::info(logging::medium)
      << "redis: initialization of services indexes";

    *this << "services" << "NOOFFSETS" << "NOFREQS" << "NOHL"
          << "SCHEMA"
          << "criticality_id" << "NUMERIC" << "SORTABLE"
          << "host_name" << "TEXT" << "SORTABLE"
          << "host_id" << "NUMERIC"
          << "service_description" << "TEXT" << "SORTABLE"
          << "service_id" << "NUMERIC"
          << "current_state" << "NUMERIC" << "SORTABLE"
          << "last_state_change" << "NUMERIC" << "SORTABLE"
          << "last_hard_state_change" << "NUMERIC" << "SORTABLE"
          << "last_check" << "NUMERIC" << "SORTABLE"
          << "current_check_attempt" << "NUMERIC" << "SORTABLE"
          << "plugin_output" << "TEXT" << "SORTABLE"
          << "state_type" << "NUMERIC" << "NOINDEX"
          << "next_check" << "NUMERIC" << "NOINDEX"
          << "max_check_attempts" << "NUMERIC" << "NOINDEX"
          << "enabled" << "NUMERIC" << "NOINDEX"
          << "scheduled_downtime_depth" << "NUMERIC" << "NOINDEX"
          << "flap_detection" << "NUMERIC" << "NOINDEX"
          << "active_checks" << "NUMERIC" << "NOINDEX"
          << "passive_checks" << "NUMERIC" << "NOINDEX"
          << "acknowledged" << "NUMERIC"
          << "notify" << "NUMERIC" << "NOINDEX"
          << "action_url" << "TEXT" << "NOINDEX"
          << "notes" << "TEXT" << "NOINDEX"
          << "notes_url" << "TEXT" << "NOINDEX"
          << "event_handler_enabled" << "NUMERIC" << "NOINDEX"
          << "flapping" << "NUMERIC" << "NOINDEX"
          << "icon_image" << "TEXT" << "NOINDEX"
          << "criticality_level" << "NUMERIC" << "NOINDEX"
          << "service_groups" << "TAG"
          << "poller_id" << "TAG"
          << "host_groups" << "TAG"
          << "display_name" << "TEXT" << "NOINDEX";

    ret = push_command("$9\r\nFT.CREATE\r\n");

    if (ret != "+OK\r\n")
      throw (exceptions::msg()
        << "redis: Unexpected error while creating the idx:services type on "
        << _address << ":" << _port << " (" << ret.toStdString() << ")");
  }
  else if (ret == "+ft_index0\r\n") {
  }
  else
    throw (exceptions::msg()
      << "redis: Unexpected error while checking the idx:services type on "
      << _address << ":" << _port << "(" << ret.toStdString() << ")");

  // Check hosts index
  *this << "idx:hosts";
  ret = push_command("$4\r\ntype\r\n");
  if (ret == "+none\r\n") {
    logging::info(logging::medium)
      << "redis: initialization of hosts indexes";

    *this << "hosts" << "NOOFFSETS" << "NOHL" << "NOFREQS"
          << "SCHEMA"
          << "criticality_id" << "NUMERIC" << "SORTABLE"
          << "name" << "TEXT" << "SORTABLE"
          << "alias" << "TEXT" << "NOINDEX"
          << "address" << "TEXT" << "SORTABLE"
          << "current_state" << "NUMERIC" << "SORTABLE"
          << "plugin_output" << "TEXT" << "SORTABLE"
          << "enabled" << "NUMERIC" << "NOINDEX"
          << "scheduled_downtime_depth" << "NUMERIC" << "NOINDEX"
          << "active_checks" << "NUMERIC" << "NOINDEX"
          << "passive_checks" << "NUMERIC" << "NOINDEX"
          << "acknowledged" << "NUMERIC"
          << "action_url" << "TEXT" << "NOINDEX"
          << "notes" << "TEXT" << "NOINDEX"
          << "notes_url" << "TEXT" << "NOINDEX"
          << "icon_image" << "TEXT" << "NOINDEX"
          << "poller_id" << "TAG"
          << "host_groups" << "TAG"
          << "criticality_level" << "NUMERIC" << "NOINDEX";
    ret = push_command("$9\r\nFT.CREATE\r\n");
    if (ret != "+OK\r\n")
      throw (exceptions::msg()
        << "redis: Unexpected error while creating the idx:hosts type on "
        << _address << ":" << _port << " (" << ret.toStdString() << ")");
  }
  else if (ret == "+ft_index0\r\n") {
  }
  else
    throw (exceptions::msg()
      << "redis: Unexpected error while checking the idx:hosts type on "
      << _address << ":" << _port << " (" << ret.toStdString() << ")");
}

/**
 *  Clear the redisdb object.
 */
void redisdb::clear() {
  _oss.str("");
  _content.clear();
  _size = 0;
}

redisdb& redisdb::operator<<(std::string const& str) {
  _oss << '$' << str.size() << "\r\n" << str << "\r\n";
  ++_size;
  return *this;
}

redisdb& redisdb::operator<<(int val) {
  std::ostringstream oss;
  oss << val;
  std::string l(oss.str());
  _oss << '$' << l.size() << "\r\n" << l << "\r\n";
  ++_size;
  return *this;
}

std::string redisdb::str(std::string const& cmd) {
  unsigned int size;
  if (cmd.empty())
    size = _size;
  else
    size = _size + 1;
  if (size >= 2) {
    std::ostringstream oss;
    oss << size;
    std::string l(oss.str());
    std::string retval("*");
    retval.append(l).append("\r\n");
    if (!cmd.empty())
      retval.append(cmd);
    retval.append(_oss.str());
    return retval;
  }
  else {
    std::string retval(_oss.str());
    return retval;
  }
}

QByteArray& redisdb::del() {
  return push_command("$3\r\ndel\r\n");
}

QByteArray& redisdb::push_command(std::string const& cmd) {
  _content.append(str(cmd));
  _oss.str("");
  _size = 0;

  if (_socket->state() != QTcpSocket::ConnectedState)
    _connect();

  if (_content.empty())
    throw (exceptions::msg()
      << "redis: Attempt to send empty data to "
      << _address << ":" << _port);

  if (_socket->write(_content.c_str()) != static_cast<qint64>(_content.size()))
    throw (exceptions::msg()
      << "redis: Couldn't write content to "
      << _address << ":" << _port
      << ": " << _socket->errorString().toStdString());

  while (_socket->bytesToWrite()) {
    if (!_socket->waitForBytesWritten())
      throw (exceptions::msg()
        << "redis: Couldn't send data to "
        << _socket->peerAddress().toString().toStdString()
        << ":" << _socket->peerPort()
        << ": " << _socket->errorString().toStdString());
  }

  if (!_socket->waitForReadyRead()) {
    throw (exceptions::msg()
      << "redis: Couldn't read data from "
      << _socket->peerAddress().toString().toStdString()
      << ":" << _socket->peerPort()
      << ": " << _socket->errorString().toStdString());
  }
  _result.clear();
  _result.append(_socket->readAll());
  clear();
  return _result;
}

QByteArray& redisdb::push(neb::custom_variable const& cv) {
  // We only work with CRITICALITY_LEVEL and CRITICALITY_ID for now.
  // For a host:
  // * h:host_id
  //     - criticality_id or criticality_level
  //
  // For a service:
  // * s:host_id:service_id
  //     - criticality_id or criticality_level
  //
  logging::info(logging::high)
    << "redis: push custom var "
    << " host_id " << cv.host_id
    << " service_id " << cv.service_id
    << " name " << cv.name.toStdString()
    << " value " << cv.value.toStdString();

  std::string tag;
  if (cv.name == "CRITICALITY_ID")
    tag = "criticality_id";
  else if (cv.name == "CRITICALITY_LEVEL")
    tag = "criticality_level";
  else {
    _result.clear();
    return _result;
  }

  std::ostringstream oss;
  if (cv.service_id) {
    oss << "s:" << cv.host_id << ':' << cv.service_id;
    *this << "services" << oss.str();
  }
  else {
    oss << "h:" << cv.host_id;
    *this << "hosts" << oss.str();
  }

  *this << 1 << "REPLACE" << "PARTIAL" << "FIELDS"
        << tag << cv.value.toStdString();
  return push_command("$6\r\nFT.ADD\r\n");
}

QByteArray& redisdb::push(instance_broadcast const& ib) {
  // Here, there are some cleanup to do...
  // FIXME DBR
  _result.clear();
  return _result;
}

QByteArray& redisdb::push(neb::host_group_member const& hgm) {
  // Values to push in redis:
  // * hg:host_id : it is a bitfield of hostgroup_id
  logging::info(logging::high)
    << "redis: push host_group_member "
    << "(host_id: " << hgm.host_id << ", group_id: " << hgm.group_id << ")";

  std::ostringstream oss;
  oss << "hg:" << hgm.host_id;
  std::string hg_key(oss.str());
  *this << hg_key << hgm.group_id << 1;
  push_command("$6\r\nSETBIT\r\n");

  *this << hg_key;
  QByteArray& hg(push_command("$3\r\nGET\r\n"));
  QByteArray hg_bf(parse(hg).toByteArray());
  std::string host_groups(parse_bitfield(hg_bf));

  oss.str("");
  oss << "h:" << hgm.host_id;

  logging::info(logging::high)
    << "redis: push host_group_member "
    << "host_groups: " << host_groups;

  *this << "hosts" << oss.str() << 1 << "REPLACE" << "PARTIAL" << "FIELDS"
    << "host_groups" << host_groups;
  push_command("$6\r\nFT.ADD\r\n");

  // Let's propagate to services attached to the host_id's host
  oss.str("");
  oss << "s:" << hgm.host_id << ":*";

  int next(0);
  do {
    *this << "SCAN" << 0 << "MATCH" << oss.str() << "COUNT" << 1000;
    push_command();
    QVariantList lst(redisdb::parse(_result).toList());
    next = lst[0].toInt();
    QVariantList items(lst[1].toList());
    for (int i = 0; i < items.size(); ++i) {
      *this << "services" << items[i].toByteArray().constData()
            << 1 << "REPLACE" << "PARTIAL" << "FIELDS"
            << "host_groups" << host_groups;
      push_command("$6\r\nFT.ADD\r\n");
    }
  } while (next);

  return _result;
}

std::string redisdb::parse_bitfield(QByteArray const& bf) {
  std::ostringstream oss;
  int j(0);
  for (QByteArray::const_iterator
         it(bf.begin()), end(bf.end());
       it != end;
       ++it) {
    char c(*it);
    if (c == 0) {
      j += 8;
      continue;
    }
    for (int k(128); k >= 1; k = k >> 1, j++) {
      std::cout << "j = " << j << " ; k = " << k << std::endl;
      if (k & c) {
        oss << j << ',';
        std::cout << "=> j = " << j << " ; k = " << k << std::endl;
      }
    }
  }
  return oss.str();
}

QByteArray& redisdb::push(neb::host const& h) {
  logging::info(logging::high)
    << "redis: push host "
    << "(host_id: " << h.host_id << ")";

  std::ostringstream oss;
  oss << "h:" << h.host_id;
  std::string host_key(oss.str());

  *this << host_key << "name" << "poller_id";
  push_command("$5\r\nHMGET\r\n");

  *this << "hosts" << host_key << 1 << "REPLACE" << "PARTIAL" << "FIELDS"
    << "name" << h.host_name.toStdString()
    << "alias" << h.alias.toStdString()
    << "address" << h.address.toStdString()
    << "action_url" << h.action_url.toStdString()
    << "notes" << h.notes.toStdString()
    << "notes_url" << h.notes_url.toStdString()
    << "poller_id" << h.poller_id
    << "icon_image" << h.icon_image.toStdString();
  push_command("$6\r\nFT.ADD\r\n");

  oss.str("");
  oss << "p:" << h.poller_id;
  *this << oss.str() << h.host_id << 1;
  return push_command("$6\r\nSETBIT\r\n");
}

QByteArray& redisdb::push(neb::host_status const& hs) {
  logging::info(logging::high)
    << "redis: push host_status "
    << "(host_id: " << hs.host_id << ")";

  std::ostringstream oss;
  oss << "h:" << hs.host_id;

  *this << "hosts" << oss.str() << 1 << "REPLACE" << "PARTIAL" << "FIELDS"
    << "current_state" << hs.current_state
    << "enabled" << hs.enabled
    << "scheduled_downtime_depth" << hs.downtime_depth
    << "plugin_output" << hs.output.toStdString()
    << "active_checks" << hs.active_checks_enabled
    << "passive_checks" << hs.passive_checks_enabled
    << "acknowledged" << hs.acknowledged;
  return push_command("$6\r\nFT.ADD\r\n");
}

QByteArray& redisdb::push(neb::instance const& inst) {
  // Values to push in redis:
  // * i:poller_id
  //    - name
  std::ostringstream oss;
  oss << "i:" << inst.poller_id;
  std::string ist(oss.str());
  *this << oss.str() << inst.name.toStdString();
  return push_command("$3\r\nset\r\n");
}

QByteArray& redisdb::push(neb::service_group_member const& sgm) {
  logging::info(logging::high)
    << "redis: push service_group_member "
    << "(host_id: " << sgm.host_id << ", service_id: " << sgm.service_id << ")";
  // Values to push in redis:
  // sg:group_id { s:host_id:service_id } it is a set of service keys.
  std::ostringstream oss;
  oss << "sg:" << sgm.group_id;
  *this << oss.str();
  oss.str("");
  oss << 's' << sgm.host_id << ':' << sgm.service_id;
  *this << oss.str()
    << sgm.group_id;
  return push_command("$4\r\nSADD\r\n");
}

QByteArray& redisdb::push(neb::service_status const& ss) {
  logging::info(logging::high)
    << "redis: push service_status "
    << "(host_id: " << ss.host_id << ", service_id: " << ss.service_id << ")";

  std::ostringstream oss;
  oss << "s:" << ss.host_id << ':' << ss.service_id;
  std::string svc(oss.str());

  *this << "services" << svc << 1 << "REPLACE" << "PARTIAL" << "FIELDS"
    << "current_state" << ss.current_state
    << "state_type" << ss.state_type
    << "last_check" << ss.last_check
    << "next_check" << ss.next_check
    << "max_check_attempts" << ss.max_check_attempts
    << "last_state_change" << ss.last_state_change
    << "last_hard_state_change" << ss.last_hard_state_change
    << "enabled" << ss.enabled
    << "scheduled_downtime_depth" << ss.downtime_depth
    << "plugin_output" << ss.output.toStdString()
    << "current_check_attempt" << ss.current_check_attempt
    << "flap_detection" << ss.flap_detection_enabled
    << "active_checks" << ss.active_checks_enabled
    << "passive_checks" << ss.passive_checks_enabled
    << "acknowledged" << ss.acknowledged;
  return push_command("$6\r\nFT.ADD\r\n");
}

QByteArray& redisdb::push(neb::service const& s) {
  logging::info(logging::high)
    << "redis: push service "
    << "(host_id: " << s.host_id << ", service_id: " << s.service_id << ")";
  std::ostringstream oss;
  oss << "h:" << s.host_id;

  *this << oss.str() << "name" << "poller_id" << "host_groups";
  push_command("$5\r\nhmget\r\n");
  oss.str("");
  oss << "s:" << s.host_id << ':' << s.service_id;
  std::string svc(oss.str());

  *this << "services" << svc << 1 << "REPLACE" << "PARTIAL" << "FIELDS"
    << "service_description" << s.service_description.toStdString()
    << "service_id" << s.service_id
    << "host_id" << s.host_id
    << "notify" << s.notifications_enabled
    << "action_url" << s.action_url.toStdString()
    << "notes" << s.notes.toStdString()
    << "notes_url" << s.notes_url.toStdString()
    << "event_handler_enabled" << s.event_handler_enabled
    << "flapping" << s.is_flapping
    << "icon_image" << s.icon_image.toStdString()
    << "display_name" << s.display_name.toStdString();

  try {
    QVariant res(redisdb::parse(_result));
    QVariantList lst(res.toList());
    *this << "host_name" << lst[0].toString().toStdString()
          << "poller_id" << lst[1].toInt()
          << "host_groups" << lst[2].toString().toStdString();
  }
  catch (std::exception const& e) {
    logging::error(logging::high)
      << "redis: unable to get host name for host id " << s.host_id;
  }

  return push_command("$6\r\nFT.ADD\r\n");
}

std::string const&  redisdb::get_content() const {
  return _content;
}

void redisdb::_connect() {
  _socket->connectToHost(_address.c_str(), _port);
  if (!_socket->waitForConnected())
    throw (exceptions::msg()
      << "redis: Couldn't connect to "
      << _address << ":" << _port
      << ": " << _socket->errorString().toStdString());

  if (!_password.empty()) {
    std::ostringstream oss;
    oss << "*2\r\n$4\r\nauth\r\n"
      << '$' << _password.size() << "\r\n" << _password << "\r\n";

    std::string query(oss.str());
    if (_socket->write(query.c_str()) != static_cast<qint64>(query.size()))
      throw (exceptions::msg()
        << "redis: Couldn't authenticate to "
      << _address << ":" << _port
      << ": " << _socket->errorString().toStdString());

    while (_socket->bytesToWrite()) {
      if (!_socket->waitForBytesWritten())
        throw (exceptions::msg()
          << "redis: Couldn't send authentication to "
          << _socket->peerAddress().toString().toStdString()
          << ":" << _socket->peerPort()
          << ": " << _socket->errorString().toStdString());
    }

    if (!_socket->waitForReadyRead()) {
      throw (exceptions::msg()
        << "redis: Couldn't read data from "
        << _socket->peerAddress().toString().toStdString()
        << ":" << _socket->peerPort()
        << ": " << _socket->errorString().toStdString());
    }
    if (_socket->readAll() != "+OK\r\n")
      throw (exceptions::msg()
        << "redis: Couldn't authenticate to "
        << _socket->peerAddress().toString().toStdString()
        << ":" << _socket->peerPort()
        << ": bad password");
  }
}

QVariant redisdb::parse(QByteArray const& array) {
  QByteArray::const_iterator it(array.begin());
  return _parse(array, it);
}

QVariant redisdb::_parse(QByteArray const& arr,
                         QByteArray::const_iterator& it) {
  switch (*it) {
    case '$':
      return _parse_str(arr, it);
    case '*':
      return _parse_array(arr, it);
    case ':':
      return _parse_int(arr, it);
    default:
      throw (exceptions::msg()
        << "redis: Unable to parse the string '"
        << arr.constData() << "' that is nor a string, nor an array");
  }
  return QVariant();
}

QVariant redisdb::_parse_str(QByteArray const& arr, QByteArray::const_iterator& it) {
  size_t len(0);
  if (*it != '$')
    throw (exceptions::msg()
      << "redis: Unable to parse '" << arr.constData() << "' as a string");
  ++it;
  while (*it != '\r') {
    len = len * 10 + (*it - '0');
    ++it;
  }

  /* Now it points at '\r' */
  it += 2;
  QVariant retval (QVariant(QByteArray(it, len)));
  it += len + 2;
  return retval;
}

QVariant redisdb::_parse_int(QByteArray const& arr, QByteArray::const_iterator& it) {
  int value(0);
  if (*it != ':')
    throw (exceptions::msg()
      << "redis: Unable to parse '" << arr.constData() << "' as an integer");
  ++it;
  while (*it != '\r') {
    value = value * 10 + *it;
    ++it;
  }

  /* Now it points at '\r' */
  it += 2;
  return QVariant(value);
}

QVariant redisdb::_parse_array(QByteArray const& arr, QByteArray::const_iterator& it) {
  size_t array_size(0);
  if (*it != '*')
    throw (exceptions::msg()
      << "redis: Unable to parse '" << arr.constData() << "' as an array");
  ++it;
  while (*it != '\r') {
    array_size = array_size * 10 + (*it - '0');
    ++it;
  }

  /* Now it points at '\r' */
  it += 2;
  QVariantList retval;
  while (array_size > 0) {
    QVariant ret(_parse(arr, it));
    retval.append(ret);
    --array_size;
  }
  return QVariant(retval);
}

std::string const& redisdb::get_address() const {
  return _address;
}

unsigned short const redisdb::get_port() const {
  return _port;
}
