// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/catch.hpp>
#include <jfc/types.h>

#include <jfc/http/context.h>

using namespace jfc;

TEST_CASE("jfc::http::get test", "[jfc::http::get]")
{
    auto pContext = http::context::make(http::context::implementation::curl);

    SECTION("get construct")
    {
        REQUIRE(true);
    }
}

