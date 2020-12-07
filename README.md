# accessprivate

This library is a result of the article [A foliage of folly](https://dfrib.github.io/a-foliage-of-folly/), and offers a standard-conformant utility for accessing (including write access to) private members.

This lib should not be used in production code, but can be useful e.g. for injection testing.

Refer to the article for details.

### Demo

```c++
// demo.cpp
#include <iostream>
#include "accessprivate/accessprivate.h"

namespace bar {

struct Bar {
    int getX() const { return x; }
    int getY() const { return y; }
private:
    int x{42};
    int y{88};
};

}  // namespace bar

DEFINE_ACCESSOR(bar::Bar, x)
// -> auto& accessprivate::get_x(Bar&)
DEFINE_ACCESSOR(bar::Bar, y)
// -> auto& *accessprivate::get_y(Bar&)

void non_const_accessor() {
    bar::Bar b{};
    accessprivate::get_x(b) = 13;
    accessprivate::get_y(b) = 33;
    std::cout << b.getX() << " " << b.getY();  // 13 33
}

DEFINE_ACCESSOR_C(bar::Bar, x)
// -> auto const& accessprivate::get_x(Bar const&)
DEFINE_ACCESSOR_C(bar::Bar, y)
// -> auto const& accessprivate::get_y(Bar const&)

void const_accessor() {
    const bar::Bar b{};
    std::cout << accessprivate::get_x(b) << " "    // 13
              << accessprivate::get_x(b) << "\n";  // 33
}
```

### Constraints

The respective non-const and const accessor macros:

- shall be invoked from the global namespace scope,
- shall, for a given set of arguments, only be invoked once within a given TU (lest you have an ODR-violation; diagnosable however)

The following is a valid use of the macros:
```c++
// valid.cpp
namespace ns {
class Foo { int x; int y; }
}  // namespace ns

DEFINE_ACCESSOR(ns::Foo, x)
DEFINE_ACCESSOR(ns::Foo, y)
DEFINE_ACCESSOR_C(ns::Foo, x)
DEFINE_ACCESSOR_C(ns::Foo, y)
```

whereas the following are all invalid uses of the macros (and will be diagnosed as such by the compiler):

```c++
// invalid_a.cpp
namespace ns {
class Foo { int x; int y; }
}  // namespace ns

DEFINE_ACCESSOR(ns::Foo, x)
DEFINE_ACCESSOR(ns::Foo, x)  // redefinition
```

```c++
// invalid_b.cpp
namespace ns {
class Foo { int x; int y; }

    DEFINE_ACCESSOR(ns::Foo, x)  // not global namespace scope
}  // namespace ns
```

### Running tests

```sh
# from repo root
$ cmake .
$ cmake --build .
$ make test
```
