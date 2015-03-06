#ifndef UNITS_UNIT_H
#define UNITS_UNIT_H

#include "dimension.h"

#include <ratio>

namespace units {

template<typename... DimExps> class unit;

namespace unit_detail {

    template<typename Dim, int exp>
    struct dim_exp {
        static_assert(dim_detail::is_dimension<Dim>::value,
            "Unit entry Dim parameter must be a dimension type.");
        using dimension = Dim;
        static constexpr int exponent = exp;
    };

    template<typename T>
    struct is_dim_exp : public std::false_type {};
    template<typename Dim, int exp>
    struct is_dim_exp<dim_exp<Dim, exp>> : public std::true_type {};

    template<typename Left, typename Right>
    struct dim_exp_less {
        static_assert(is_dim_exp<Left>::value,
            "Left argument must be a dim exp.");
        static_assert(is_dim_exp<Right>::value,
            "Right argument must be a dim exp.");
        static constexpr bool value =
            compile_string_less<typename Left::dimension::name,
                                typename Right::dimension::name>::value;
    };

    template<typename Left, typename Right>
    struct dim_exp_equal {
        static_assert(is_dim_exp<Left>::value,
            "Left argument must be a dim exp.");
        static_assert(is_dim_exp<Right>::value,
            "Right argument must be a dim exp.");
        static constexpr bool value =
            compile_string_equal<typename Left::dimension::name,
                                 typename Right::dimension::name>::value;
    };

    template<typename Pair>
    struct dim_exp_multiply_impl {
    private:
        using Left = typename Pair::first_type;
        using Right = typename Pair::second_type;
        static_assert(is_dim_exp<Left>::value,
            "Left argument must be a dim exp.");
        static_assert(is_dim_exp<Right>::value,
            "Right argument must be a dim exp.");
        using left_dim = typename Left::dimension;
        using right_dim = typename Left::dimension;
        static_assert(std::is_same<left_dim, right_dim>::value,
            "Cannot multiply dim exps with mismatching dimensions.");

    public:
        using type = dim_exp<left_dim, Left::exponent + Right::exponent>;
    };

    template<typename Pair>
    using dim_exp_multiply = typename dim_exp_multiply_impl<Pair>::type;

    template<typename Arg>
    struct is_negative_exp {
        static_assert(is_dim_exp<Arg>::value,
            "Argument must be a dim exp.");
        static constexpr bool value = Arg::exponent < 0;
    };

    template<typename Arg>
    struct is_positive_exp {
        static_assert(is_dim_exp<Arg>::value,
            "Argument must be a dim exp.");
        static constexpr bool value = Arg::exponent > 0;
    };

    template<typename Arg>
    struct is_nonzero_exp {
        static_assert(is_dim_exp<Arg>::value,
            "Argument must be a dim exp.");
        static constexpr bool value = Arg::exponent != 0;
    };

    template<typename T>
    struct is_unit : public std::false_type {};
    template<typename... D>
    struct is_unit<unit<D...>> : public std::true_type {};

    template<typename T>
    struct is_ratio : public std::false_type {};
    template<intmax_t N, intmax_t D>
    struct is_ratio<std::ratio<N, D>> : public std::true_type {};

    template<typename Left, typename Right>
    struct unit_multiply_impl {
    private:
        using common = meta::filter<is_nonzero_exp,
            meta::map<dim_exp_multiply,
                meta::intersection<dim_exp_less, Left, Right>
            >
        >;
        using left_uniq = meta::difference<dim_exp_less, Left, Right>;
        using right_uniq = meta::difference<dim_exp_less, Right, Left>;
        using combined = meta::sort<dim_exp_less,
            meta::concat<left_uniq, common, right_uniq>
        >;
    public:
        using type = meta::rebind<unit, combined>;
    };

    template<typename Left, typename Right>
    using unit_multiply = typename unit_multiply_impl<Left, Right>::type;

    template<typename DimExp, typename Power>
    struct dim_exp_exp_impl {
        static_assert(is_dim_exp<DimExp>::value,
            "Parameter must be a dim exp.");
        using type = dim_exp<typename DimExp::dimension, DimExp::exponent * Power::value>;
    };

    template<typename DimExp, typename Power>
    using dim_exp_exp = typename dim_exp_exp_impl<DimExp, Power>::type;

    template<typename List, int power>
    struct unit_exp_impl {
        using type = meta::rebind<unit,
            meta::filter<is_nonzero_exp,
                meta::map<
                    meta::bind_second_type<dim_exp_exp, std::integral_constant<int, power>>::template action,
                    List
                >
            >
        >;
    };

    template<typename List> struct unit_printer {
        static void print(std::ostream& os, bool close, bool invert) {}
    };

    template<typename Head, typename... Tail>
    struct unit_printer<meta::type_list<Head, Tail...>> {
        static void print(std::ostream& os, bool close, bool invert) {
            using base_name = typename Head::dimension::base_unit;
            os << base_name{};
            if (Head::exponent != (invert ? -1 : 1)) {
                os << "^" << (invert ? -1 : 1) * Head::exponent;
            }
            os << "*";
            unit_printer<meta::type_list<Tail...>>::print(os, close, invert);
        }
    };

    template<typename Last>
    struct unit_printer<meta::type_list<Last>> {
        static void print(std::ostream& os, bool close, bool invert) {
            using base_name = typename Last::dimension::base_unit;
            os << base_name{};
            if (Last::exponent != (invert ? -1 : 1)) {
                os << "^" << (invert ? -1 : 1) * Last::exponent;
            }
            if (close) {
                os << ")";
            }
        }
    };

} /* namespace unit_detail */

template<typename... DimExps>
class unit {
    using list = meta::type_list<DimExps...>;
    static_assert(meta::all<unit_detail::is_dim_exp, list>::value,
        "All unit arguments must be exponentiated dimensions.");
    static_assert(meta::all<unit_detail::is_nonzero_exp, list>::value,
        "All unit arguments must have nonzero exponents.");
    static_assert(meta::is_sorted<unit_detail::dim_exp_less, list>::value,
        "Unit arguments must be sorted.");
    static_assert(meta::is_unique<unit_detail::dim_exp_equal, list>::value,
        "Unit arguments must be unique.");
    using dims_list = meta::type_list<typename DimExps::dimension...>;
    using self = unit<DimExps...>;

public:
    template<typename... OtherDims>
    auto operator* (const unit<OtherDims...>& other) const {
        using other_t = unit<OtherDims...>;
        using result_t = unit_detail::unit_multiply<self, other_t>;
        return result_t{};
    }

    template<int power>
    auto exp() const {
        using result_t = typename unit_detail::unit_exp_impl<self, power>::type;
        return result_t{};
    }

    template<typename... OtherDims>
    auto operator/ (const unit<OtherDims...>& other) const {
        using inverse_t = typename decltype(other.invert())::self;
        using result_t = unit_detail::unit_multiply<self, inverse_t>;
        return result_t{};
    }

    template<typename... D>
    friend class unit;

private:
    auto invert() const {
        using result_t = typename unit_detail::unit_exp_impl<self, -1>::type;
        return result_t{};
    }
};

template<typename Ratio, typename Unit>
struct unit_multiple {
    static_assert(unit_detail::is_ratio<Ratio>::value,
        "Ratio parameter must be a std::ratio type.");
    static_assert(unit_detail::is_unit<Unit>::value,
        "Unit parameter must be a unit type.");

    using ratio_type = Ratio;
    using unit_type = Unit;

    template<intmax_t N, intmax_t D>
    auto operator* (const std::ratio<N, D>&) const {
        using ResultRatio = std::ratio_multiply<Ratio, std::ratio<N, D>>;
        return unit_multiple<ResultRatio, Unit>{};
    }

    template<typename R, typename U>
    auto operator* (const unit_multiple<R, U>&) const {
        using ResultRatio = std::ratio_multiply<Ratio, R>;
        using ResultUnit = decltype(Unit{} * U{});
        return unit_multiple<ResultRatio, ResultUnit>{};
    }

    template<intmax_t N, intmax_t D>
    auto operator/ (const std::ratio<N, D>&) const {
        using ResultRatio = std::ratio_divide<Ratio, std::ratio<N, D>>;
        return unit_multiple<ResultRatio, Unit>{};
    }

    template<typename R, typename U>
    auto operator/ (const unit_multiple<R, U>&) const {
        using ResultRatio = std::ratio_divide<Ratio, R>;
        using ResultUnit = decltype(Unit{} / U{});
        return unit_multiple<ResultRatio, ResultUnit>{};
    }
};

template<intmax_t N, intmax_t D, typename... Dims>
unit_multiple<std::ratio<N, D>, unit<Dims...>>
operator* (const std::ratio<N, D>&, const unit<Dims...>&) {
    return {};
}

template<intmax_t N, intmax_t D, typename RRatio, typename Unit>
auto operator* (const std::ratio<N, D>& lhs, const unit_multiple<RRatio, Unit>& rhs) {
    return rhs * lhs;
}

std::ostream& operator<< (std::ostream& os, const unit<>&) {
    return os;
}

template<typename... Dims>
std::ostream& operator<< (std::ostream& os, const unit<Dims...>&) {
    using list = meta::type_list<Dims...>;
    using num_dims = meta::filter<unit_detail::is_positive_exp, list>;
    using denom_dims = meta::filter<unit_detail::is_negative_exp, list>;

    if (num_dims::size() == 0 and denom_dims::size() == 0) {
        return os << "[scalar]";
    }

    if (num_dims::size() == 0) {
        os << "1";
    } else if (num_dims::size() == 1 or denom_dims::size() == 0) {
        unit_detail::unit_printer<num_dims>::print(os, false, false);
    } else if (denom_dims::size() != 0) {
        os << "(";
        unit_detail::unit_printer<num_dims>::print(os, true, false);
    }

    if (denom_dims::size() != 0) {
        os << "/";
    }

    if (denom_dims::size() == 1) {
        unit_detail::unit_printer<denom_dims>::print(os, false, true);
    } else if(denom_dims::size() != 0) {
        os << "(";
        unit_detail::unit_printer<denom_dims>::print(os, true, true);
    }
    return os;
}

#define DEFINE_DIMENSION(name, base)                                           \
    auto name##_symbol = COMPILE_STRING(#name);                                \
    auto base##_symbol = COMPILE_STRING(#base);                                \
    using name##_dim = dimension<decltype(name##_symbol), decltype(base##_symbol)>;\
    using name = unit<unit_detail::dim_exp<name##_dim, 1>>;                    \
    name base{};

using Scalar = unit<>;


} /* namespace units */

#endif
