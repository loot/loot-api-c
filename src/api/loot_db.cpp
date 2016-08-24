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

#include "api/loot_db.h"

#include <algorithm>

loot_db::loot_db(const loot::GameType gameType, const std::string& gamePath, const std::string& gameLocalDataPath)
  : database(loot::CreateDatabase(gameType, gamePath, gameLocalDataPath)) {}

std::shared_ptr<loot::DatabaseInterface> loot_db::getDatabase() {
  return database;
}

const char * loot_db::getRevisionIdString() const {
  return revisionId.c_str();
}

const char * loot_db::getRevisionDateString() const {
  return revisionDate.c_str();
}

const std::vector<const char *>& loot_db::getAddedTags() const {
  return addedTags;
}

const std::vector<const char *>& loot_db::getRemovedTags() const {
  return removedTags;
}

const std::vector<loot_message>& loot_db::getPluginMessages() const {
  return cPluginMessages;
}

void loot_db::setRevisionIdString(const std::string& str) {
  revisionId = str;
}

void loot_db::setRevisionDateString(const std::string& str) {
  revisionDate = str;
}

void loot_db::setAddedTags(const std::set<std::string>& names) {
  addedTags.clear();
  for (const auto& name : names) {
    addedTags.push_back(storeTag(name));
  }
}

void loot_db::setRemovedTags(const std::set<std::string>& names) {
  removedTags.clear();
  for (const auto& name : names) {
    removedTags.push_back(storeTag(name));
  }
}

void loot_db::setPluginMessages(const std::vector<loot::SimpleMessage>& pluginMessages) {
  cPluginMessages.resize(pluginMessages.size());
  pluginMessageStrings.resize(pluginMessages.size());

  size_t i = 0;
  for (const auto& message : pluginMessages) {
    pluginMessageStrings[i] = message.text;

    cPluginMessages[i].type = static_cast<unsigned int>(message.type);
    cPluginMessages[i].message = pluginMessageStrings[i].c_str();

    ++i;
  }
}

void loot_db::clearArrays() {
  bashTags.clear();

  addedTags.clear();
  removedTags.clear();

  cPluginMessages.clear();
  pluginMessageStrings.clear();
}

const char * loot_db::storeTag(const std::string & tag) {
  return bashTags.insert(tag).first->c_str();
}
