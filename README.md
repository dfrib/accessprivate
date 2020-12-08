# accessprivate - Accessing private members in C++20

This library is a result of the article [A foliage of folly](https://dfrib.github.io/a-foliage-of-folly/), and offers a standard-conformant utility for accessing (including write access to) private members in C++20.

This lib should not be used in production code, but can be useful e.g. for injection testing.

Refer to the article for details.

## API

| Macro                             | Accessor                              |
| --------------------------------- | ------------------------------------- |
| `DEFINE_ACCESSOR(<QCN>, <CDM>)`   | `auto& get_<CDM>(<QCN>&)`             |
| `DEFINE_ACCESSOR_C(<QCN>, <CDM>)` | `auto const& get_<CDM>(<QCN> const&)` |

The `<QCN>` and `<CDM>` arguments shall be a _qualified class name_ and a the name of a _class data member_ (of the former), respectively.

## Demo

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
    std::cout << accessprivate::get_x(b) << " "    // 42
              << accessprivate::get_x(b) << "\n";  // 88
}
```

## Constraints

The respective non-const `DEFINE_ACCESSOR` and const `DEFINE_ACCESSOR_C` accessor macros:

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

whereas the following are both invalid uses of the macros (and will be diagnosed as such by the compiler):

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

DEFINE_ACCESSOR(Foo, x)  // not invoked from the global namespace scope
}  // namespace ns
```

### Language version

As is covered in detailed in the article, the techniques of the library depends on a new minor C++20 feature, but is likely work work also for C++17 for major compilers such as GCC and Clang (albeit being non-conformant in C++17).

## Running tests

```sh
# (clone repo): from repo root
$ mkdir build
$ cd build
$ cmake ../
$ cmake --build .
$ make test
```
