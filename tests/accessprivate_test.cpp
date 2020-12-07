#include <cassert>
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
// -> auto& accessprivate::get_y(Bar&)

void non_const_accessor() {
    bar::Bar b{};

    assert(b.getX() == 42);
    assert(b.getY() == 88);

    accessprivate::get_x(b) = 10;
    accessprivate::get_y(b) = 43;

    assert(b.getX() == 10);
    assert(b.getY() == 43);

    ++accessprivate::get_x(b);
    --accessprivate::get_y(b);

    assert(b.getX() == 11);
    assert(b.getY() == 42);
}

DEFINE_ACCESSOR_C(bar::Bar, x)
// -> auto const& accessprivate::get_x(Bar const&)
DEFINE_ACCESSOR_C(bar::Bar, y)
// -> auto const& accessprivate::get_y(Bar const&)

void const_accessor() {
    const bar::Bar b{};
    assert(accessprivate::get_x(b) == 42);
    assert(accessprivate::get_y(b) == 88);
}

int main() {
    non_const_accessor();
    const_accessor();
}
