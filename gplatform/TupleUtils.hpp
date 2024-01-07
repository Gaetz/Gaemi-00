//
// Created by gaetz on 07/01/24.
//

#ifndef GPLATFORM_TUPLEUTILS_HPP
#define GPLATFORM_TUPLEUTILS_HPP

#include "Defines.hpp"
#include <tuple>

template <class T, class... Ts>
constexpr std::tuple<Ts...> TupleTail(const std::tuple<T, Ts...>& t) {
    return std::apply([](auto, auto&&... tail) {
        return std::make_tuple(std::forward<decltype(tail)>(tail)...);
    }, t);
}

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type RemoveRowFromTuple(std::tuple<Tp...>&, size_t) { }

template<std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type RemoveRowFromTuple(std::tuple<Tp...>& t, size_t row) {
    auto& vec = std::get<I>(t);
    vec.erase(vec.begin() + row);
    RemoveRowFromTuple<I + 1, Tp...>(t, row);
}


#endif //GPLATFORM_TUPLEUTILS_HPP
