#ifndef UNITS_COMPILE_STRING_H
#define UNITS_COMPILE_STRING_H

#include "meta.h"

#include <type_traits>
#include <iostream>
#include <string>

namespace units {

#define COMPILE_STRING(s) string_detail::make_compile_string([]{               \
        struct temp {                                                          \
            static constexpr decltype(auto) get() { return s; }                \
        };                                                                     \
        return temp{};                                                         \
    }())

template<char c>
using char_type = std::integral_constant<char, c>;

template<typename T>
struct is_char_type : public std::false_type {};
template<char c>
struct is_char_type<std::integral_constant<char, c>> : public std::true_type {};

template<typename... Chars>
class compile_string {
    using self = compile_string<Chars...>;
    static_assert(meta::all<is_char_type, self>::value,
                  "All compile_string template parameters must be character types.");

public:
    constexpr std::size_t size() const {
        return sizeof...(Chars);
    }

    constexpr const char* c_str() const {
        return c_string;
    }

    std::string str() const {
        return std::string{c_str()};
    }

    template<typename... RChars>
    constexpr bool operator< (compile_string<RChars...> other) const {
        return compare_with(other) < 0;
    }

    template<typename... RChars>
    constexpr bool operator> (compile_string<RChars...> other) const {
        return compare_with(other) > 0;
    }

    template<typename... RChars>
    constexpr bool operator<= (compile_string<RChars...> other) const {
        return compare_with(other) <= 0;
    }

    template<typename... RChars>
    constexpr bool operator>= (compile_string<RChars...> other) const {
        return compare_with(other) >= 0;
    }

    template<typename... RChars>
    constexpr bool operator== (compile_string<RChars...> other) const {
        return compare_with(other) == 0;
    }

    template<typename... RChars>
    constexpr bool operator!= (compile_string<RChars...> other) const {
        return compare_with(other) != 0;
    }

private:

    static const char c_string[];

    template<typename... RChars>
    constexpr int compare_with(compile_string<RChars...>) const {
        using Other = compile_string<RChars...>;
        return meta::lexicographical_compare<self, Other>::value;
    }
};

template<typename... Chars>
const char compile_string<Chars...>::c_string[] {Chars::value..., '\0'};

template<typename Left, typename Right>
struct compile_string_less {
    static constexpr bool value = (Left{} < Right{});
};

template<typename Left, typename Right>
struct compile_string_equal {
    static constexpr bool value = (Left{} == Right{});
};

template<typename T>
struct is_compile_string : public std::false_type {};
template<typename... Chars>
struct is_compile_string<compile_string<Chars...>> : public std::true_type {};

namespace string_detail {

template<typename P, std::size_t... Indices>
constexpr auto make_compile_string_impl(P producer, std::index_sequence<Indices...>) {
    return compile_string<char_type<P::get()[Indices]>...>{};
}

template<typename P>
constexpr auto make_compile_string(P producer) {
    using sequence = std::make_index_sequence<sizeof(P::get()) - 1>;
    return make_compile_string_impl(producer, sequence{});
}

} /* namespace string_detail */

template<typename... Chars>
std::ostream& operator<< (std::ostream& os, const compile_string<Chars...>& cs) {
    return os << cs.c_str();
}

} /* namespace units */

#endif
