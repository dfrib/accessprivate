/// Copyright (c) 2020 David Friberg
///
/// This software is released under the MIT License.
/// https://opensource.org/licenses/MIT

#pragma once

namespace accessprivate {

template <auto mem_ptr>
struct AccessPrivate
{
    // kMemPtr is intended to be either a pointer to a private
    // member function or pointer to a private data member.
    static constexpr auto kMemPtr = mem_ptr;

    // Define only in explicit specializations.
    struct Delegate;
    struct ConstDelegate;  // To allow overloading accessors on constness.
};

}  // namespace accessprivate

// Note: accessor macros need to be invoked from global scope (not from
// within a namespace scope).

/// DEFINE_ACCESSOR(<qualified class name>, <class data member>)
/// Defines a non-const accessor:
///   auto& get_<class data member>(<qualified class name>&)
///
/// into the ::accessprivate namespace scope.
///
/// Example usage:
///   DEFINE_ACCESSOR(foo::Foo, x)
///
/// Defines:
///   auto& accessprivate::get_x(foo::Foo&)
#define DEFINE_ACCESSOR(qualified_class_name, class_data_member)\
namespace accessprivate {\
template <>\
struct AccessPrivate<&qualified_class_name::class_data_member>::Delegate {\
    friend auto& get_##class_data_member(\
        qualified_class_name& obj) { return obj.*kMemPtr; }\
};\
auto& get_##class_data_member(qualified_class_name& obj);\
}

/// DEFINE_ACCESSOR_C(<qualified class name>, <class data member>)
/// Defines a const accessor:
///   auto const& get_<class data member>(<qualified class name> const&)
///
/// into the ::accessprivate namespace scope.
///
/// Example usage:
///   DEFINE_ACCESSOR_C(foo::Foo, x)
///
/// Defines:
///   auto const& accessprivate::get_x(foo::Foo const&)
#define DEFINE_ACCESSOR_C(qualified_class_name, class_data_member)\
namespace accessprivate {\
template <>\
struct AccessPrivate<&qualified_class_name::class_data_member>::ConstDelegate {\
    friend auto const& get_##class_data_member(\
        qualified_class_name const& obj) { return obj.*kMemPtr; }\
};\
auto const& get_##class_data_member(qualified_class_name const& obj);\
}

