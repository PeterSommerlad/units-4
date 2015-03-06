#ifndef UNITS_META_H
#define UNITS_META_H

#include <type_traits>
#include <utility>

namespace units {
namespace meta {

// Type List

template<typename... Args>
struct type_list {
    static constexpr std::size_t size() { return sizeof...(Args); }
};

// Rebind

namespace detail {

    template<template<typename...> class Y, typename List> struct rebind_impl;
    template<template<typename...> class Y,
             template<typename...> class X, typename... Args>
    struct rebind_impl<Y, X<Args...>> {
        using type = Y<Args...>;
    };

} /* namespace detail */

template<template<typename...> class Y, typename List>
using rebind = typename ::units::meta::detail::rebind_impl<Y, List>::type;


// All

template<template<typename> class Predicate, typename List> struct all;
template<template<typename> class Predicate,
         template<typename...> class X,
         typename Head, typename... Tail>
struct all<Predicate, X<Head, Tail...>> {
    static constexpr bool value = Predicate<Head>::value and all<Predicate, X<Tail...>>::value;
};

template<template<typename> class Predicate, template<typename...> class X, typename Head>
struct all<Predicate, X<Head>> {
    static constexpr bool value = Predicate<Head>::value;
};

template<template<typename> class Predicate, template<typename...> class X>
struct all<Predicate, X<>> : public std::true_type {};

// Lexicographical compare

template<typename Left, typename Right> struct lexicographical_compare;
template<template<typename...> class X,
         typename LHead, typename... LTail,
         typename RHead, typename... RTail>
struct lexicographical_compare<X<LHead, LTail...>, X<RHead, RTail...>> {
    static constexpr int value = (LHead::value < RHead::value) ? -1 : (
            (LHead::value > RHead::value) ? 1 : (
                lexicographical_compare<X<LTail...>, X<RTail...>>::value
            )
        );
};

template<template<typename...> class X, typename... Left>
struct lexicographical_compare<X<Left...>, X<>> {
    static constexpr int value = 1;
};

template<template<typename...> class X, typename... Right>
struct lexicographical_compare<X<>, X<Right...>> {
    static constexpr int value = -1;
};

template<template<typename...> class X>
struct lexicographical_compare<X<>, X<>> {
    static constexpr int value = 0;
};

// Static if

namespace detail {

    template<bool predicate, typename A, typename B>
    struct static_if_impl {
        using type = A;
    };
    template<typename A, typename B>
    struct static_if_impl<false, A, B> {
        using type = B;
    };

} /* namespace detail */

template<bool predicate, typename A, typename B>
using static_if = typename ::units::meta::detail::static_if_impl<predicate, A, B>::type;

// Prepend

namespace detail {

    template<typename Head, typename List> struct prepend_impl;
    template<template<typename...> class X,
             typename Head, typename... Args>
    struct prepend_impl<Head, X<Args...>> {
        using type = X<Head, Args...>;
    };
    template<template<typename...> class X,
             typename Head>
    struct prepend_impl<Head, X<>> {
        using type = X<Head>;
    };

} /* namespace detail */

template<typename Head, typename List>
using prepend = typename ::units::meta::detail::prepend_impl<Head, List>::type;

// Append

namespace detail {

    template<typename Tail, typename List> struct append_impl;
    template<template<typename...> class X,
             typename Tail, typename... Args>
    struct append_impl<Tail, X<Args...>> {
        using type = X<Args..., Tail>;
    };

} /* namespace detail */

template<typename Tail, typename List>
using append = typename ::units::meta::detail::append_impl<Tail, List>::type;

// Concat

namespace detail {

    template<typename Left, typename Right> struct concat2_impl;
    template<template<typename...> class X,
             typename... Left, typename... Right>
    struct concat2_impl<X<Left...>, X<Right...>> {
        using type = X<Left..., Right...>;
    };

    template<typename... Lists> struct concat_impl;
    template<template<typename...> class X, typename... Args, typename... Lists>
    struct concat_impl<X<Args...>, Lists...> {
        using concat_rest = typename concat_impl<Lists...>::type;
        using type = typename concat2_impl<X<Args...>, concat_rest>::type;
    };
    template<template<typename...> class X, typename... Args>
    struct concat_impl<X<Args...>> {
        using type = X<Args...>;
    };

} /* namespace detail */

template<typename... Lists>
using concat = typename ::units::meta::detail::concat_impl<Lists...>::type;

// Filter

namespace detail {

    template<template<typename> class Predicate, typename List> struct filter_impl;
    template<template<typename> class Predicate,
             template<typename...> class X,
             typename Head, typename... Tail>
    struct filter_impl<Predicate, X<Head, Tail...>> {
        using type = static_if< Predicate<Head>::value
            , prepend<Head, typename filter_impl<Predicate, X<Tail...>>::type>
            , typename filter_impl<Predicate, X<Tail...>>::type
        >;
    };
    template<template<typename> class Predicate,
             template<typename...> class X>
    struct filter_impl<Predicate, X<>> {
        using type = X<>;
    };


} /* namespace detail */

template<template<typename> class Predicate, typename List>
using filter = typename ::units::meta::detail::filter_impl<Predicate, List>::type;

// Map

namespace detail {

    template<template<typename> class Transform, typename List> struct map_impl;
    template<template<typename> class Transform,
             template<typename...> class X,
             typename Head, typename... Tail>
    struct map_impl<Transform, X<Head, Tail...>> {
        using type = prepend<
            Transform<Head>,
            typename map_impl<Transform, X<Tail...>>::type
        >;
    };
    template<template<typename> class Transform,
             template<typename...> class X>
    struct map_impl<Transform, X<>> {
        using type = X<>;
    };

} /* namespace detail */

template<template<typename> class Transform, typename List>
using map = typename ::units::meta::detail::map_impl<Transform, List>::type;

// Bind First

template<template<typename, typename> class Function, typename Bound>
struct bind_first_value {
    template<typename T>
    struct action {
        static constexpr bool value = Function<Bound, T>::value;
    };
};

// Bind Second

template<template<typename, typename> class Function, typename Bound>
struct bind_second_value {
    template<typename T>
    struct action {
        static constexpr bool value = Function<T, Bound>::value;
    };
};

template<template<typename, typename> class Function, typename Bound>
struct bind_second_type {
    template<typename T>
    using action = Function<T, Bound>;
};

// Invert Predicate

template<template<typename> class Predicate>
struct invert_predicate {
    template<typename T>
    struct action {
        static constexpr bool value = not Predicate<T>::value;
    };
};

// Sort

namespace detail {
    template<template<typename, typename> class Less, typename List> struct sort_impl;
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename Pivot, typename... Args>
    struct sort_impl<Less, X<Pivot, Args...>> {
    private:
        using LeftFiltered = filter<bind_second_value<Less, Pivot>::template action, X<Args...>>;
        using LeftSorted = typename sort_impl<Less, LeftFiltered>::type;

        using RightFiltered = filter<invert_predicate<bind_second_value<Less, Pivot>::template action>::template action, X<Args...>>;
        using RightSorted = typename sort_impl<Less, RightFiltered>::type;

    public:
        using type = concat<LeftSorted, X<Pivot>, RightSorted>;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename Pivot>
    struct sort_impl<Less, X<Pivot>> {
        using type = X<Pivot>;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X>
    struct sort_impl<Less, X<>> {
        using type = X<>;
    };
}

template<template<typename, typename> class Less, typename List>
using sort = typename ::units::meta::detail::sort_impl<Less, List>::type;

// Is sorted

template<template<typename, typename> class Less, typename List> struct is_sorted;
template<template<typename, typename> class Less,
         template<typename...> class X,
         typename First, typename Second, typename... Rest>
struct is_sorted<Less, X<First, Second, Rest...>> {
    static constexpr bool value = !Less<Second, First>::value
        and is_sorted<Less, X<Second, Rest...>>::value;
};
template<template<typename, typename> class Less,
         template<typename...> class X, typename Arg>
struct is_sorted<Less, X<Arg>> : public std::true_type {};
template<template<typename, typename> class Less,
         template<typename...> class X>
struct is_sorted<Less, X<>> : public std::true_type {};

// Is Unique

template<template<typename, typename> class Equal, typename List> struct is_unique;
template<template<typename, typename> class Equal,
         template<typename...> class X,
         typename First, typename Second, typename... Rest>
struct is_unique<Equal, X<First, Second, Rest...>> {
    static constexpr bool value = !Equal<First, Second>::value
        and is_unique<Equal, X<Second, Rest...>>::value;
};
template<template<typename, typename> class Equal,
         template<typename...> class X, typename Arg>
struct is_unique<Equal, X<Arg>> : public std::true_type {};
template<template<typename, typename> class Equal,
         template<typename...> class X>
struct is_unique<Equal, X<>> : public std::true_type {};

// Intersection

namespace detail {

    template<template<typename, typename> class Less, typename Left, typename Right>
    struct intersection_impl;
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename LHead, typename... LTail,
             typename RHead, typename... RTail>
    struct intersection_impl<Less, X<LHead, LTail...>, X<RHead, RTail...>> {
    private:
        static constexpr bool left_less = Less<LHead, RHead>::value;
        static constexpr bool right_less = Less<RHead, LHead>::value;
    public:
        using type = static_if<!left_less and !right_less
            , prepend<
                std::pair<LHead, RHead>,
                typename intersection_impl<Less, X<LTail...>, X<RTail...>>::type
              >
            , static_if<left_less
                , typename intersection_impl<Less, X<LTail...>, X<RHead, RTail...>>::type
                , typename intersection_impl<Less, X<LHead, LTail...>, X<RTail...>>::type
            >
        >;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename... Left>
    struct intersection_impl<Less, X<Left...>, X<>> {
        using type = X<>;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename... Right>
    struct intersection_impl<Less, X<>, X<Right...>> {
        using type = X<>;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X>
    struct intersection_impl<Less, X<>, X<>> {
        using type = X<>;
    };

} /* namespace detail */

template<template<typename, typename> class Less, typename Left, typename Right>
using intersection = typename ::units::meta::detail::intersection_impl<Less, Left, Right>::type;

// Union

namespace detail {

    template<template<typename, typename> class Less, typename Left, typename Right>
    struct union_impl;
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename LHead, typename... LTail,
             typename RHead, typename... RTail>
    struct union_impl<Less, X<LHead, LTail...>, X<RHead, RTail...>> {
    private:
        static constexpr bool left_less = Less<LHead, RHead>::value;
        static constexpr bool right_less = Less<RHead, LHead>::value;
    public:
        using type = static_if<!left_less and !right_less
            , prepend<LHead, typename union_impl<Less, X<LTail...>, X<RTail...>>::type>
            , static_if<left_less
                , prepend<LHead, typename union_impl<Less, X<LTail...>, X<RHead, RTail...>>::type>
                , prepend<RHead, typename union_impl<Less, X<LHead, LTail...>, X<RTail...>>::type>
            >
        >;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename... Left>
    struct union_impl<Less, X<Left...>, X<>> {
        using type = X<Left...>;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename... Right>
    struct union_impl<Less, X<>, X<Right...>> {
        using type = X<Right...>;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X>
    struct union_impl<Less, X<>, X<>> {
        using type = X<>;
    };

} /* namespace detail */

template<template<typename, typename> class Less, typename Left, typename Right>
using union_ = typename ::units::meta::detail::union_impl<Less, Left, Right>::type;

// Difference

namespace detail {

    template<template<typename, typename> class Less, typename Left, typename Right>
    struct difference_impl;
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename LHead, typename... LTail,
             typename RHead, typename... RTail>
    struct difference_impl<Less, X<LHead, LTail...>, X<RHead, RTail...>> {
    private:
        static constexpr bool left_less = Less<LHead, RHead>::value;
        static constexpr bool right_less = Less<RHead, LHead>::value;
    public:
        using type = static_if<!left_less and !right_less
            , typename difference_impl<Less, X<LTail...>, X<RTail...>>::type
            , static_if<left_less
                , prepend<LHead, typename difference_impl<Less, X<LTail...>, X<RHead, RTail...>>::type>
                , typename difference_impl<Less, X<LHead, LTail...>, X<RTail...>>::type
            >
        >;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename... Left>
    struct difference_impl<Less, X<Left...>, X<>> {
        using type = X<Left...>;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X,
             typename... Right>
    struct difference_impl<Less, X<>, X<Right...>> {
        using type = X<>;
    };
    template<template<typename, typename> class Less,
             template<typename...> class X>
    struct difference_impl<Less, X<>, X<>> {
        using type = X<>;
    };

} /* namespace detail */

template<template<typename, typename> class Less, typename Left, typename Right>
using difference = typename ::units::meta::detail::difference_impl<Less, Left, Right>::type;


} /* namespace meta */
} /* namespace units */

#endif
