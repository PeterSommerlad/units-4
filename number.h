#include "unit.h"

namespace units {

template<typename N, typename U> class unit_number;
template<typename U, typename N> unit_number<N, U> make_unit_number(const N& n) {
    return unit_number<N, U>{n};
}

template<typename N, typename U>
class unit_number {
    static_assert(std::is_arithmetic<N>::value,
        "N-parameter must be arithmetic.");
    static_assert(unit_detail::is_unit<U>::value,
        "U-parameter must be a unit type.");

public:
    using number_type = N;
    using unit_type = U;

    explicit unit_number(const N& x) : value_(x) {}

    template<typename M>
    explicit unit_number(const M& x) : value_(x) {}

    template<typename M>
    unit_number(const unit_number<M, U>& x) : value_(x.value_) {}

    template<typename M>
    unit_number<N, U>& operator= (const unit_number<M, U>& x) {
        value_ = x.value_;
        return *this;
    }

    template<typename M>
    explicit operator M() const {
        return M{value_};
    }

    unit_number<N, U> operator- () const {
        return make_unit_number<N, U>(-value_);
    }

    template<typename M>
    auto operator+ (const unit_number<M, U>& number) const {
        return make_unit_number<U>(value_ + number.value_);
    }

    template<typename M>
    unit_number<N, U>& operator+ (const unit_number<M, U>& number) {
        value_ += number.value_;
        return *this;
    }

    template<typename M>
    auto operator- (const unit_number<M, U>& number) const {
        return make_unit_number<U>(value_ - number.value_);
    }

    template<typename M, typename V>
    auto operator* (const unit_number<M, V>& number) const {
        using R = decltype(U{} * V{});
        return make_unit_number<R>(value_ * number.value_);
    }

    template<typename... D>
    auto operator* (const unit<D...>& u) const {
        using R = decltype(U{} * unit<D...>{});
        return make_unit_number<R>(value_);
    }

    template<typename M, typename V>
    auto operator/ (const unit_number<M, V>& number) const {
        using R = decltype(U{} / V{});
        return make_unit_number<R>(value_ * number.value_);
    }

    N value() const {
        return value_;
    }

    template<typename M, typename V>
    friend class unit_number;

private:
    N value_;

};

template<typename N, typename... D>
unit_number<N, unit<D...>> operator* (const N& n, const unit<D...>& u) {
    return make_unit_number<unit<D...>>(n);
}

template<typename N, typename R, typename U>
unit_number<N, U> operator* (const N& n, const unit_multiple<R, U>&) {
    return make_unit_number<U>(n * (N{R::num} / N{R::den}));
}

template<typename N, typename U>
std::ostream& operator<< (std::ostream& os, const unit_number<N, U>& num) {
    return os << num.value() << " " << U{};
}

template<typename U> using u_float       = unit_number<float, U>;
template<typename U> using u_double      = unit_number<double, U>;
template<typename U> using u_long_double = unit_number<long double, U>;

template<typename U> using scalar_float       = u_float<Scalar>;
template<typename U> using scalar_double      = u_double<Scalar>;
template<typename U> using scalar_long_double = u_long_double<Scalar>;

template<typename U> using u_short      = unit_number<short, U>;
template<typename U> using u_ushort     = unit_number<unsigned short, U>;
template<typename U> using u_int        = unit_number<int, U>;
template<typename U> using u_uint       = unit_number<unsigned int, U>;
template<typename U> using u_long       = unit_number<long, U>;
template<typename U> using u_ulong      = unit_number<unsigned long, U>;
template<typename U> using u_long_long  = unit_number<long long, U>;
template<typename U> using u_ulong_long = unit_number<unsigned long long, U>;

template<typename U> using scalar_short      = u_short<Scalar>;
template<typename U> using scalar_ushort     = u_ushort<Scalar>;
template<typename U> using scalar_int        = u_int<Scalar>;
template<typename U> using scalar_uint       = u_uint<Scalar>;
template<typename U> using scalar_long       = u_long<Scalar>;
template<typename U> using scalar_ulong      = u_ulong<Scalar>;
template<typename U> using scalar_long_long  = u_long_long<Scalar>;
template<typename U> using scalar_ulong_long = u_ulong_long<Scalar>;

namespace number_detail {

    template<typename U> struct unit_type_impl;

    template<typename... DimExps>
    struct unit_type_impl<unit<DimExps...>> {
        using type = unit<DimExps...>;
    };

    template<typename R, typename U>
    struct unit_type_impl<unit_multiple<R, U>> {
        using type = U;
    };

    template<typename N, typename U>
    struct unit_type_impl<unit_number<N, U>> {
        using type = U;
    };

} /* namespace number_detail */

template<typename T>
using unit_type = typename ::units::number_detail::unit_type_impl<T>::type;

} /* namespace units */

