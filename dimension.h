#ifndef UNITS_DIMENSION_H
#define UNITS_DIMENSION_H

#include "compile_string.h"

namespace units {


template<typename Name, typename BaseUnit>
struct dimension {
    static_assert(is_compile_string<Name>::value,
        "Dimension name must be a compile-time string.");
    static_assert(is_compile_string<BaseUnit>::value,
        "Dimension base unit must be a compile-time string.");
    using name = Name;
    using base_unit = BaseUnit;
};

template<typename N, typename B>
std::ostream& operator<< (std::ostream& os, const dimension<N, B>&) {
    return os << "[Dim " << N{} << "]";
}

namespace dim_detail {

    template<typename T>
    struct is_dimension : public std::false_type {};
    template<typename N, typename B>
    struct is_dimension<dimension<N, B>> : public std::true_type {};

} /* namespace dim_detail */

} /* namespace units */

#endif

