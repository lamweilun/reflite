#include <iostream>
#include "reflite.h"  // include this header

struct Birthday
{
  // Members must be public
  int day = 1, month = 1, year = 1900;

  // Declare your reflections
  REFLITE_TYPE(Birthday)
  REFLITE_START
  REFLITE_ADD(Birthday, int, "day"),
  REFLITE_ADD(Birthday, int, "month"),
  REFLITE_ADD(Birthday, int, "year")
  REFLITE_END

  // Define your reflections
  REFLITE_DEFINE_START
  REFLITE_DEFINE_ADD(Birthday, day, "day"),
  REFLITE_DEFINE_ADD(Birthday, month, "month"),
  REFLITE_DEFINE_ADD(Birthday, year, "year")
  REFLITE_DEFINE_END
};

// Overload the operator<< for the std::cout below
std::ostream& operator<<(std::ostream& os, const Birthday& rhs)
{
  return os << rhs.day << '/' << rhs.month << '/' << rhs.year;
}

struct Human
{
  // Members must be public
  Birthday birthday;
  const char* name;
  float height;

  // Declare your reflections
  REFLITE_TYPE(Human)
  REFLITE_START
  REFLITE_ADD(Human, Birthday, "birthday"),
  REFLITE_ADD(Human, const char*, "name"),
  REFLITE_ADD(Human, float, "height")
  REFLITE_END
  
  // Define your reflections
  REFLITE_DEFINE_START
  REFLITE_DEFINE_ADD(Human, birthday, "birthday"),
  REFLITE_DEFINE_ADD(Human, name, "name"),
  REFLITE_DEFINE_ADD(Human, height, "height")
  REFLITE_DEFINE_END
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
