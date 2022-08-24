// Copyright (C) 2022 Jonathan Müller and dryad contributors
// SPDX-License-Identifier: BSL-1.0

#include <dryad/_detail/tuple.hpp>

#include <doctest/doctest.h>

TEST_CASE("_detail::tuple")
{
    SUBCASE("normal")
    {
        constexpr auto tuple = dryad::_detail::make_tuple(11, 3.14f, true, 42);
        CHECK(std::is_same_v<decltype(tuple), const dryad::_detail::tuple<int, float, bool, int>>);

        CHECK(tuple.get<0>() == 11);
        CHECK(tuple.get<1>() == 3.14f);
        CHECK(tuple.get<2>() == true);
        CHECK(tuple.get<3>() == 42);
    }
    SUBCASE("references")
    {
        int       lvalue  = 42;
        const int clvalue = 7;

        auto tuple = dryad::_detail::forward_as_tuple(lvalue, 11, clvalue);
        CHECK(std::is_same_v<decltype(tuple), dryad::_detail::tuple<int&, int&&, const int&>>);

        CHECK(std::is_same_v<decltype(tuple.get<0>()), int&>);
        CHECK(std::is_same_v<decltype(tuple.get<1>()), int&&>);
        CHECK(std::is_same_v<decltype(tuple.get<2>()), const int&>);

        const auto& ctuple = tuple;
        CHECK(std::is_same_v<decltype(ctuple.get<0>()), int&>);
        CHECK(std::is_same_v<decltype(ctuple.get<1>()), int&&>);
        CHECK(std::is_same_v<decltype(ctuple.get<2>()), const int&>);
    }
}

