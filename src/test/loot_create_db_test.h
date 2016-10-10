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

#ifndef LOOT_TESTS_API_LOOT_CREATE_DB_TEST
#define LOOT_TESTS_API_LOOT_CREATE_DB_TEST

#include "loot/c_api.h"

#include <climits>

#include "test/common_game_test_fixture.h"

namespace loot {
namespace test {
class loot_create_db_test :
  public ::testing::TestWithParam<unsigned int>,
  public CommonGameTestFixture {
protected:
  loot_create_db_test() :
    CommonGameTestFixture(GetParam()),
    db_(nullptr) {}

  void SetUp() {
    setUp();
  }

  void TearDown() {
    tearDown();

    ASSERT_NO_THROW(loot_destroy_db(db_));
  }

  loot_db * db_;
};

// Pass an empty first argument, as it's a prefix for the test instantation,
// but we only have the one so no prefix is necessary.
INSTANTIATE_TEST_CASE_P(,
                        loot_create_db_test,
                        ::testing::Values(
                          loot_game_tes4,
                          loot_game_tes5,
                          loot_game_fo3,
                          loot_game_fonv,
                          loot_game_fo4));

TEST_P(loot_create_db_test, shouldSucceedIfPassedValidParametersWithRelativePaths) {
  EXPECT_EQ(loot_ok, loot_create_db(&db_, GetParam(), dataPath.parent_path().string().c_str(), localPath.string().c_str()));
  EXPECT_NE(nullptr, db_);
}

TEST_P(loot_create_db_test, shouldSucceedIfPassedValidParametersWithAbsolutePaths) {
  boost::filesystem::path game = boost::filesystem::current_path() / dataPath.parent_path();
  boost::filesystem::path local = boost::filesystem::current_path() / localPath;

  EXPECT_EQ(loot_ok, loot_create_db(&db_, GetParam(), game.string().c_str(), local.string().c_str()));
  EXPECT_NE(nullptr, db_);
}

TEST_P(loot_create_db_test, shouldReturnAnArgumentErrorIfPassedANullPointer) {
  EXPECT_EQ(loot_error_argument, loot_create_db(NULL, GetParam(), dataPath.parent_path().string().c_str(), localPath.string().c_str()));
}

TEST_P(loot_create_db_test, shouldReturnAnArgumentErrorIfPassedAnInvalidGameType) {
  EXPECT_EQ(loot_error_argument, loot_create_db(&db_, UINT_MAX, dataPath.parent_path().string().c_str(), localPath.string().c_str()));
}

TEST_P(loot_create_db_test, shouldReturnAnArgumentErrorIfPassedAGamePathThatDoesNotExist) {
  EXPECT_EQ(loot_error_argument, loot_create_db(&db_, GetParam(), missingPath.string().c_str(), localPath.string().c_str()));
}

TEST_P(loot_create_db_test, shouldReturnAnArgumentErrorIfPassedALocalPathThatDoesNotExist) {
  EXPECT_EQ(loot_error_argument, loot_create_db(&db_, GetParam(), dataPath.parent_path().string().c_str(), missingPath.string().c_str()));
}

#ifdef _WIN32
TEST_P(loot_create_db_test, shouldReturnOkIfPassedANullLocalPathPointer) {
  EXPECT_EQ(loot_ok, loot_create_db(&db_, GetParam(), dataPath.parent_path().string().c_str(), NULL));
}
#endif
}
}

#endif
