## reflite
- C++ header-only compile time class/struct introspection helper.
- Compatible with C++17 and above.

### My motivations for making this:
- I was working on my own C++ game engine project and I needed a quick and easy way to setup reflection of data for my engine components. It also needs to be easily extendable to be used in a variety of systems such as serialization/deserialization and registration of GUI layouts. Existing solutions, like RTTR, are great but I just want to keep the project as lean as possible. So I gave myself a challenge and after some days of research, this is what I came up with.

### Pros and Cons
- Pros:
  - Lightweight.
  - Header only.
  - No other dependencies.
- Cons:
  - Does not handle member functions.
  - Does not handle inheritance.
  - C++14 and below not supported.
  - You tell me, there's probably a lot more.

### Special Thanks:
[Malody Hoe - undefinist](https://github.com/undefinist) for the help in reducing macros usage.

---

### Sample Usage:
```C++
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

  /* This will output:
  birthday
  17/6/1996
  name
  John
  height
  1.8
  */
}
```
