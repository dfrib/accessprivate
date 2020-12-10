/// Copyright (c) 2020 David Friberg
///
/// This software is released under the MIT License.
/// https://opensource.org/licenses/MIT

#pragma once

namespace accessprivate {

/// Top-level accessor class whose member class specializations are used
/// to define class- and members of the class-specific accessors.
///
/// \tparam mem_ptr  Non-type template parameter of placeholder type, intended
///                  to take template arguments (whilst specializing the)
///                  classes) that are pointers to private members.
template <auto mem_ptr>
struct AccessPrivate {
  /// Static data member that is intende to store the access-eras pointer
  /// to a private member function or pointer to a private data member.
  static constexpr auto kMemPtr = mem_ptr;

  /// Member class intended to be explicitly specialized to provide non-const
  /// accessors to the class over whose private data member is is being
  /// specialized.
  ///
  /// This member class should only ever be defined in explicit
  /// specializations.
  struct Delegate;

  /// Member class intended to be explicitly specialized to provide const
  /// accessors to the class over whose private data member is is being
  /// specialized.
  ///
  /// This member class should only ever be defined in explicit
  /// specializations.
  struct ConstDelegate;  // To allow overloading accessors on constness.
};

}  // namespace accessprivate

/// Macro for defining a non-const accessor function.
///
/// For an argument that is a qualified class name (say `QCN`) and an argument
/// that is a (private) class data member (say `CDM`) of the former class,
/// invocation of the macro over these arguments defines the following
/// non-const accessor function
/// ```{.cpp}
/// auto& get_<CDM>(<QCN>&)
/// ```
/// into the `accessprivate` namespace scope.
///
/// Example usage:
/// ```{.cpp}
/// // From global namespace scope.
/// DEFINE_ACCESSOR(foo::Foo, x)
/// // -> auto& accessprivate::get_x(foo::Foo&)
/// ```
///
/// \attention The accessor macro needs to be invoked from the global namespace
///            scope.
///
/// \param qualified_class_name  Qualified class name
/// \param class_data_member     (Private) class data member of the class
#define DEFINE_ACCESSOR(qualified_class_name, class_data_member)               \
  namespace accessprivate {                                                    \
  template <>                                                                  \
  struct AccessPrivate<&qualified_class_name::class_data_member>::Delegate {   \
    friend auto &get_##class_data_member(qualified_class_name &obj) {          \
      return obj.*kMemPtr;                                                     \
    }                                                                          \
  };                                                                           \
  auto &get_##class_data_member(qualified_class_name &obj);                    \
  }

/// Macro for defining const accessor function.
///
/// For an argument that is a qualified class name (say `QCN`) and an argument
/// that is a (private) class data member (say `CDM`) of the former class,
/// invocation of the macro over these arguments defines the following const
/// accessor function
/// ```{.cpp}
/// auto const& get_<CDM>(<QCN> const&)
/// ```
/// into the `accessprivate` namespace scope.
///
/// Example usage:
/// ```{.cpp}
/// // From global namespace scope.
/// DEFINE_ACCESSOR_C(foo::Foo, x)
/// // -> auto const& accessprivate::get_x(foo::Foo const&)
/// ```
///
/// \attention The accessor macro needs to be invoked from the global namespace
///            scope.
///
/// \param qualified_class_name  Qualified class name
/// \param class_data_member     (Private) class data member of the class
#define DEFINE_ACCESSOR_C(qualified_class_name, class_data_member)             \
  namespace accessprivate {                                                    \
  template <>                                                                  \
  struct AccessPrivate<                                                        \
      &qualified_class_name::class_data_member>::ConstDelegate {               \
    friend auto const &                                                        \
        get_##class_data_member(qualified_class_name const &obj) {             \
      return obj.*kMemPtr;                                                     \
    }                                                                          \
  };                                                                           \
  auto const &get_##class_data_member(qualified_class_name const &obj);        \
  }
