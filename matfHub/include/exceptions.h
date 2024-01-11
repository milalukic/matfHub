#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class ScheduleNetworkException : public std::exception
{
public:
  const char *what()
  {
    return "Greska prilikom dohvatanja podataka sa MATF servera";
  }
};

#endif // EXCEPTIONS_H
