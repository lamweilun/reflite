#include <iostream>
#include "reflite.h"  // include this header

struct Birthday
{
  int day = 1, month = 1, year = 1900;

  // Declare your reflections
  REFLITE_START(Birthday)
  REFLITE_ADD(day)
  REFLITE_ADD(month)
  REFLITE_ADD(year)
  REFLITE_END
};

// Overload the operator<< for the std::cout below
std::ostream& operator<<(std::ostream& os, const Birthday& rhs)
{
  return os << rhs.day << '/' << rhs.month << '/' << rhs.year;
}

struct Human
{
  Birthday birthday;
  const char* name;
  float height;

  // Declare your reflections
  REFLITE_START(Human)
  REFLITE_ADD(birthday)
  REFLITE_ADD(name)
  REFLITE_ADD(height)
  REFLITE_END
};

int main()
{
  // Given a "Human" object
  Human h1 {{17, 6, 1996}, "John", 1.8f};

  // Iterate through its reflected members
  REFLITE_VISIT_START(Human, memberInfo)
  std::cout << memberInfo.name << std::endl;
  std::cout << h1.*memberInfo.data << std::endl;
  REFLITE_VISIT_END
}
