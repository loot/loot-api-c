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

#ifndef LOOT_TESTS_API_LOOT_SORT_PLUGINS_TEST
#define LOOT_TESTS_API_LOOT_SORT_PLUGINS_TEST

#include "loot/c_api.h"

#include "test/api_game_operations_test.h"

namespace loot {
namespace test {
class loot_sort_plugins_test : public ApiGameOperationsTest {
protected:
  loot_sort_plugins_test() :
    sortedPlugins_(nullptr),
    numPlugins_(0) {}

  const char ** sortedPlugins_;
  size_t numPlugins_;
};

// Pass an empty first argument, as it's a prefix for the test instantation,
// but we only have the one so no prefix is necessary.
INSTANTIATE_TEST_CASE_P(,
                        loot_sort_plugins_test,
                        ::testing::Values(
                          loot_game_tes4,
                          loot_game_tes5,
                          loot_game_fo3,
                          loot_game_fonv,
                          loot_game_fo4));

TEST_P(loot_sort_plugins_test, shouldReturnAnArgumentErrorIfAnyOfThePointerArgumentsAreNull) {
  EXPECT_EQ(loot_error_argument, loot_sort_plugins(NULL, sortedPlugins_, numPlugins_));
  EXPECT_EQ(loot_error_argument, loot_sort_plugins(db_, NULL, numPlugins_));
}

TEST_P(loot_sort_plugins_test, shouldReturnAnArgumentErrorIfTheNumPluginsArgumentIsZero) {
  EXPECT_EQ(loot_error_argument, loot_sort_plugins(db_, sortedPlugins_, 0));
}

TEST_P(loot_sort_plugins_test, shouldSucceedIfPassedValidArguments) {
  std::vector<const char *> sortedPlugins = {
    blankDifferentMasterDependentEsp.c_str(),
    blankEsp.c_str(),
    blankPluginDependentEsp.c_str(),
    blankDifferentEsp.c_str(),
    blankDifferentPluginDependentEsp.c_str(),
    masterFile.c_str(),
    blankEsm.c_str(),
    blankMasterDependentEsm.c_str(),
    blankDifferentEsm.c_str(),
    blankDifferentMasterDependentEsm.c_str(),
    blankMasterDependentEsp.c_str(),
  };

  std::vector<std::string> expectedOrder = {
      masterFile,
      blankEsm,
      blankMasterDependentEsm,
      blankDifferentEsm,
      blankDifferentMasterDependentEsm,
      blankMasterDependentEsp,
      blankDifferentMasterDependentEsp,
      blankEsp,
      blankPluginDependentEsp,
      blankDifferentEsp,
      blankDifferentPluginDependentEsp,
  };

  ASSERT_NO_THROW(GenerateMasterlist());
  ASSERT_EQ(loot_ok, loot_load_lists(db_, masterlistPath.string().c_str(), NULL));

  EXPECT_EQ(loot_ok, loot_sort_plugins(db_, &sortedPlugins[0], sortedPlugins.size()));

  for (size_t i = 0; i < expectedOrder.size(); ++i) {
    EXPECT_EQ(expectedOrder[i], sortedPlugins[i]);
  }
}
}
}

#endif
