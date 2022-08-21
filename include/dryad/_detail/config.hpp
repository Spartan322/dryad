// Copyright (C) 2022 Jonathan Müller and dryad contributors
// SPDX-License-Identifier: BSL-1.0

#ifndef DRYAD_DETAIL_CONFIG_HPP_INCLUDED
#define DRYAD_DETAIL_CONFIG_HPP_INCLUDED

#include <cstddef>
#include <cstdint>
#include <type_traits>

//=== utility traits===//
#define DRYAD_MOV(...) static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define DRYAD_FWD(...) static_cast<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define DRYAD_DECLVAL(...) lexy::_detail::declval<__VA_ARGS__>()

#define DRYAD_DECAY_DECLTYPE(...) std::decay_t<decltype(__VA_ARGS__)>

#define DRYAD_CTHIS (const_cast<const std::remove_pointer_t<decltype(this)>*>(this))

namespace dryad::_detail
{
template <typename... T>
constexpr bool error = false;

template <typename T>
std::add_rvalue_reference_t<T> declval();

template <typename T>
constexpr void swap(T& lhs, T& rhs)
{
    T tmp = DRYAD_MOV(lhs);
    lhs   = DRYAD_MOV(rhs);
    rhs   = DRYAD_MOV(tmp);
}

template <typename T, typename U>
constexpr bool is_decayed_same = std::is_same_v<std::decay_t<T>, std::decay_t<U>>;

template <typename T, typename Fallback>
using type_or = std::conditional_t<std::is_void_v<T>, Fallback, T>;
} // namespace dryad::_detail

#endif // DRYAD_DETAIL_CONFIG_HPP_INCLUDED

