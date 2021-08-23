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

### Usage:
```C++
#include <iostream>
#include "reflite.h" // Include this header

struct S
{
  // Members must be public for it to be "reflected"
  int a;

  /*
  - Reflection is achieved in 2 steps, declaration then definition.
  - Declaration and Definition must be contiguous in order.
  - Do not declare/define methods or members while reflecting.
  */

  // Declare your reflections
  REFLITE_TYPE(S)
  REFLITE_START                 // Start of declaration block
  REFLITE_ADD(S, int, "a")      // (this_type, member_type, "name")
  REFLITE_END                   // End of declaration block

  // Define your reflections
  REFLITE_DEFINE_START          // Start of definition block
  REFLITE_DEFINE_ADD(S, a, "a") // (this_type, member, "same name as declaration")
  REFLITE_DEFINE_END            // End of definition block
};

struct M
{
  float b;
  char c;
  double d;

  /*
  - For reflection of multiple members, 
  - add a comma at the end of each "ADD" command

  - Ensure that there is an equal number of declarations and definitions
  */

  // Declare your reflections
  REFLITE_TYPE(M)
  REFLITE_START
  REFLITE_ADD(M, float, "b"),   // Add a comma for every additional member
  REFLITE_ADD(M, char, "c"),
  REFLITE_ADD(M, double, "d")   // No commas required for the last member
  REFLITE_END

  // Define your reflections
  REFLITE_DEFINE_START
  REFLITE_DEFINE_ADD(M, b, "b"),  // You need commas here as well
  REFLITE_DEFINE_ADD(M, c, "c"),
  REFLITE_DEFINE_ADD(M, d, "d")   // No commas required for the last member
  REFLITE_DEFINE_END
};

int main
(
  M m{1.0f, 'e', 2.0};

  // You can then iterate through a struct's reflection like so
  // The following is not compile-time, it is still runtime generated.

  // Takes in a reflected type, and you provide an "ID" for the iterated data
  REFLITE_VISIT_START(M, memberInfo)
  std::cout << memberInfo.name << std::endl;      // Access the reflected name
  std::cout << m.*memberInfo.data << std::endl;   // Access the data of a certain object
  REFLITE_VISIT_END

  /*
   This will output:

   b
   1.0
   c
   e
   d
   2.0
   */
)
```

### Sample demo:
```C++
#include <iostream>
#include "reflite.h"

struct Birthday
{
  int day = 1, month = 1, year = 1900;

  REFLITE_TYPE(Birthday)
  REFLITE_START
  REFLITE_ADD(Birthday, int, "day"),
  REFLITE_ADD(Birthday, int, "month"),
  REFLITE_ADD(Birthday, int, "year")
  REFLITE_END

  REFLITE_DEFINE_START
  REFLITE_DEFINE_ADD(Birthday, day, "day"),
  REFLITE_DEFINE_ADD(Birthday, month, "month"),
  REFLITE_DEFINE_ADD(Birthday, year, "year")
  REFLITE_DEFINE_END
};

// Overloaded operator<< for the std::cout below
std::ostream& operator<<(std::ostream& os, const Birthday& rhs)
{
  return os << rhs.day << '/' << rhs.month << '/' << rhs.year;
}

struct Human
{
  Birthday birthday;
  const char* name;
  float height;

  REFLITE_TYPE(Human)
  REFLITE_START
  REFLITE_ADD(Human, Birthday, "birthday"),
  REFLITE_ADD(Human, const char*, "name"),
  REFLITE_ADD(Human, float, "height")
  REFLITE_END

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

  /*
   This will output:

   birthday
   17/6/1996
   name
   John
   height
   1.8
   */
}
```
