/*
** output.h for CentreonBroker in ./inc/conf
** 
** Made by Matthieu Kermagoret <mkermagoret@merethis.com>
** 
** Copyright Merethis
** See LICENSE file for details.
** 
** Started on  06/17/09 Matthieu Kermagoret
** Last update 06/17/09 Matthieu Kermagoret
*/

#ifndef CONF_OUTPUT_H_
# define CONF_OUTPUT_H_

# include <string>

namespace                CentreonBroker
{
  namespace              Conf
  {
    class                Output
    {
     private:
      enum               String
      {
	DB = 0,
	HOST,
	PASSWORD,
	PREFIX,
	TYPE,
	USER,
	STRING_NB
      };
      std::string        strings_[STRING_NB];

     public:
                         Output();
                         Output(const Output& output);
                         ~Output();
      Output&            operator=(const Output& output);
      bool               operator==(const Output& output);
      const std::string& GetDb() const throw ();
      const std::string& GetHost() const throw ();
      const std::string& GetPassword() const throw ();
      const std::string& GetPrefix() const throw ();
      const std::string& GetType() const throw ();
      const std::string& GetUser() const throw ();
      void               SetDb(const std::string& db);
      void               SetHost(const std::string& host);
      void               SetPassword(const std::string& password);
      void               SetPrefix(const std::string& prefix);
      void               SetType(const std::string& type);
      void               SetUser(const std::string& user);
    };
  }
}

#endif /* !CONF_OUTPUT_H_ */
