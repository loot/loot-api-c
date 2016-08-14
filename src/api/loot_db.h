/*  LOOT

    A load order optimisation tool for Oblivion, Skyrim, Fallout 3 and
    Fallout: New Vegas.

    Copyright (C) 2013-2016    WrinklyNinja

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

#ifndef LOOT_API_LOOT_DB
#define LOOT_API_LOOT_DB

#include <loot/api.h>
#include <vector>
#include <unordered_set>

#include "loot/c_api.h"

struct loot_db {
  loot_db(const loot::GameType gameType,
          const std::string& gamePath,
          const std::string& gameLocalDataPath);

  std::shared_ptr<loot::DatabaseInterface> getDatabase();

  const char * getRevisionIdString() const;
  const char * getRevisionDateString() const;

  const std::vector<const char *>& getPluginNames() const;

  const std::vector<const char *>& getAddedTags() const;
  const std::vector<const char *>& getRemovedTags() const;

  const std::vector<loot_message>& getPluginMessages() const;

  void setRevisionIdString(const std::string& str);
  void setRevisionDateString(const std::string& str);

  void setPluginNames(const std::vector<std::string>& plugins);

  void setAddedTags(const std::set<std::string>& names);
  void setRemovedTags(const std::set<std::string>& names);

  void setPluginMessages(const std::vector<loot::PluginMessage>& pluginMessages);

  void clearArrays();
private:
  const char * storeTag(const std::string& tag);

  std::shared_ptr<loot::DatabaseInterface> database;

  std::string revisionId;
  std::string revisionDate;

  std::vector<std::string> pluginNames;
  std::vector<const char *> cPluginNames;

  std::unordered_set<std::string> bashTags;
  std::vector<const char *> addedTags;
  std::vector<const char *> removedTags;

  std::vector<loot_message> cPluginMessages;
  std::vector<std::string> pluginMessageStrings;
};

#endif
