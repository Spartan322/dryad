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

#define DRYAD_DECLVAL(...) dryad::_detail::declval<__VA_ARGS__>()

#define DRYAD_DECAY_DECLTYPE(...) std::decay_t<decltype(__VA_ARGS__)>

#define DRYAD_AS_CONST(Ptr) (const_cast<const std::remove_pointer_t<decltype(Ptr)>*>(Ptr))
#define DRYAD_CTHIS DRYAD_AS_CONST(this)

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

template <int Priority>
struct priority_tag;
template <>
struct priority_tag<0>
{};
template <int Priority>
struct priority_tag : priority_tag<Priority - 1>
{};
} // namespace dryad::_detail

//=== force inline ===//
#ifndef DRYAD_FORCE_INLINE
#    if defined(__has_cpp_attribute)
#        if __has_cpp_attribute(gnu::always_inline)
#            define DRYAD_FORCE_INLINE [[gnu::always_inline]]
#        elif defined(_MSC_VER)
#            define DRYAD_FORCE_INLINE __forceinline
#        endif
#    endif
#
#    ifndef DRYAD_FORCE_INLINE
#        define DRYAD_FORCE_INLINE inline
#    endif
#endif

//=== empty_member ===//
#ifndef DRYAD_EMPTY_MEMBER

#    if defined(__has_cpp_attribute)
#        if __has_cpp_attribute(no_unique_address) || __has_cpp_attribute(msvc::no_unique_address)
#            define DRYAD_HAS_EMPTY_MEMBER 1
#        endif
#    endif
#    ifndef DRYAD_HAS_EMPTY_MEMBER
#        define DRYAD_HAS_EMPTY_MEMBER 0
#    endif

#    if DRYAD_HAS_EMPTY_MEMBER
#        if __has_cpp_attribute(msvc::no_unique_address)
#            define DRYAD_EMPTY_MEMBER                                                             \
                _Pragma("warning(push)") _Pragma("warning(disable : 4848)")                        \
                    [[msvc::no_unique_address]] _Pragma("warning(pop)")
#        else
#            define DRYAD_EMPTY_MEMBER [[no_unique_address]]
#        endif
#    else
#        define DRYAD_EMPTY_MEMBER
#    endif

#endif

#ifdef _MSC_VER
#    include <intrin.h>
#    pragma warning(push)
#    pragma warning(disable : 4244)
DRYAD_FORCE_INLINE int builtin_clzll(unsigned long long x)
{
    return __lzcnt64(x);
}
#    pragma warning(pop)
#else
DRYAD_FORCE_INLINE inline int builtin_clzll(unsigned long long x)
{
    return __builtin_clzll(x);
}
#endif

#endif // DRYAD_DETAIL_CONFIG_HPP_INCLUDED
