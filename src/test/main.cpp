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

#include <gtest/gtest.h>

#include "loot_create_db_test.h"
#include "loot_eval_lists_test.h"
#include "loot_get_dirty_info_test.h"
#include "loot_get_masterlist_revision_test.h"
#include "loot_get_plugin_messages_test.h"
#include "loot_get_plugin_tags_test.h"
#include "loot_load_lists_test.h"
#include "loot_sort_plugins_test.h"
#include "loot_update_masterlist_test.h"
#include "loot_write_minimal_list_test.h"
#include "test_api.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
