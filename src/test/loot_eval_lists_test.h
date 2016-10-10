/*  LOOT

A load order optimisation tool for Oblivion, Skyrim, Fallout 3 and
Fallout: New Vegas.

Copyright (C) 2014-2016    WrinklyNinja

This file is part of LOOT.

LOOT is free software: you can redistribute
it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

LOOT is distributed in the hope that it will
be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with LOOT.  If not, see
<https://www.gnu.org/licenses/>.
*/

#ifndef LOOT_TESTS_API_LOOT_EVAL_LISTS_TEST
#define LOOT_TESTS_API_LOOT_EVAL_LISTS_TEST

#include "loot/c_api.h"

#include "test/api_game_operations_test.h"

namespace loot {
namespace test {
class loot_eval_lists_test : public ApiGameOperationsTest {};

// Pass an empty first argument, as it's a prefix for the test instantation,
// but we only have the one so no prefix is necessary.
INSTANTIATE_TEST_CASE_P(,
                        loot_eval_lists_test,
                        ::testing::Values(
                          loot_game_tes4,
                          loot_game_tes5,
                          loot_game_fo3,
                          loot_game_fonv,
                          loot_game_fo4));

TEST_P(loot_eval_lists_test, shouldReturnAnArgumentErrorIfPassedANullPointer) {
  EXPECT_EQ(loot_error_argument, loot_eval_lists(NULL));
}

TEST_P(loot_eval_lists_test, shouldReturnOkIfDbPointerIsValid) {
  EXPECT_EQ(loot_ok, loot_eval_lists(db_));
}
}
}

#endif
