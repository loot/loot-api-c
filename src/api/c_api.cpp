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

#include "loot/c_api.h"

#include <algorithm>
#include <regex>

#include <loot/api.h>

#include "api/loot_db.h"

using loot::GameType;
using loot::MessageType;
using loot::LanguageCode;

const unsigned int loot_ok = 0;
const unsigned int loot_error_unknown = 1;
const unsigned int loot_error_file_access = 2;
const unsigned int loot_error_condition_syntax = 3;
const unsigned int loot_error_regex_syntax = 4;
const unsigned int loot_error_game_detection = 5;
const unsigned int loot_error_libloadorder = 6;
const unsigned int loot_error_libgit2 = 7;
const unsigned int loot_error_os = 8;
const unsigned int loot_error_cyclic_interaction = 9;
const unsigned int loot_error_git_state = 10;
const unsigned int loot_error_no_memory = 11;
const unsigned int loot_error_argument = 12;
const unsigned int loot_return_max = loot_error_argument;

// The following are the games identifiers used by the API.
const unsigned int loot_game_tes4 = static_cast<unsigned int>(GameType::tes4);
const unsigned int loot_game_tes5 = static_cast<unsigned int>(GameType::tes5);
const unsigned int loot_game_tes5se = static_cast<unsigned int>(GameType::tes5se);
const unsigned int loot_game_fo3 = static_cast<unsigned int>(GameType::fo3);
const unsigned int loot_game_fonv = static_cast<unsigned int>(GameType::fonv);
const unsigned int loot_game_fo4 = static_cast<unsigned int>(GameType::fo4);

const unsigned int loot_message_say = static_cast<unsigned int>(MessageType::say);
const unsigned int loot_message_warn = static_cast<unsigned int>(MessageType::warn);
const unsigned int loot_message_error = static_cast<unsigned int>(MessageType::error);

// LOOT message languages.
const unsigned int loot_lang_english = static_cast<unsigned int>(LanguageCode::english);
const unsigned int loot_lang_spanish = static_cast<unsigned int>(LanguageCode::spanish);
const unsigned int loot_lang_russian = static_cast<unsigned int>(LanguageCode::russian);
const unsigned int loot_lang_french = static_cast<unsigned int>(LanguageCode::french);
const unsigned int loot_lang_chinese = static_cast<unsigned int>(LanguageCode::chinese);
const unsigned int loot_lang_polish = static_cast<unsigned int>(LanguageCode::polish);
const unsigned int loot_lang_brazilian_portuguese = static_cast<unsigned int>(LanguageCode::brazilian_portuguese);
const unsigned int loot_lang_finnish = static_cast<unsigned int>(LanguageCode::finnish);
const unsigned int loot_lang_german = static_cast<unsigned int>(LanguageCode::german);
const unsigned int loot_lang_danish = static_cast<unsigned int>(LanguageCode::danish);
const unsigned int loot_lang_korean = static_cast<unsigned int>(LanguageCode::korean);

// LOOT cleanliness codes.
const unsigned int loot_cleanliness_clean = static_cast<unsigned int>(loot::PluginCleanliness::clean);
const unsigned int loot_cleanliness_dirty = static_cast<unsigned int>(loot::PluginCleanliness::dirty);
const unsigned int loot_cleanliness_do_not_clean = static_cast<unsigned int>(loot::PluginCleanliness::do_not_clean);
const unsigned int loot_cleanliness_unknown = static_cast<unsigned int>(loot::PluginCleanliness::unknown);

std::string extMessageStr;

unsigned int c_error(const unsigned int code, const std::string& what) {
  extMessageStr = what;
  return code;
}

unsigned int c_error(std::exception_ptr exceptionPtr) {
  try {
    std::rethrow_exception(exceptionPtr);
  } catch (loot::CyclicInteractionError& e) {
    return c_error(loot_error_cyclic_interaction, e.what());
  } catch (loot::GitStateError& e) {
    return c_error(loot_error_git_state, e.what());
  } catch (loot::GameDetectionError& e) {
    return c_error(loot_error_game_detection, e.what());
  } catch (loot::ConditionSyntaxError& e) {
    return c_error(loot_error_condition_syntax, e.what());
  } catch (loot::FileAccessError& e) {
    return c_error(loot_error_file_access, e.what());
  } catch (std::ios_base::failure& e) {
    return c_error(loot_error_file_access, e.what());
  } catch (std::system_error& e) {
    if (e.code().category() == std::system_category())
      return c_error(loot_error_os, e.what());
    else if (e.code().category() == loot::libgit2_category())
      return c_error(loot_error_libgit2, e.what());
    else if (e.code().category() == loot::libloadorder_category())
      return c_error(loot_error_libloadorder, e.what());
    else
      return c_error(loot_error_unknown, e.what());
  } catch (std::regex_error& e) {
    return c_error(loot_error_regex_syntax, e.what());
  } catch (std::invalid_argument& e) {
    return c_error(loot_error_argument, e.what());
  } catch (std::bad_alloc& e) {
    return c_error(loot_error_no_memory, e.what());
  } catch (std::exception& e) {
    return c_error(loot_error_unknown, e.what());
  }
}

//////////////////////////////
// Error Handling Functions
//////////////////////////////

// Outputs a string giving the details of the last time an error or
// warning return code was returned by a function. The string exists
// until this function is called again or until CleanUpAPI is called.
LOOT_C_API unsigned int loot_get_error_message(const char ** const message) {
  if (message == nullptr)
    return c_error(loot_error_argument, "Null message pointer passed.");

  *message = extMessageStr.c_str();

  return loot_ok;
}

//////////////////////////////
// Version Functions
//////////////////////////////

LOOT_C_API bool loot_is_compatible(const unsigned int versionMajor, const unsigned int versionMinor, const unsigned int versionPatch) {
  return loot::IsCompatible(versionMajor, versionMinor, versionPatch);
}

LOOT_C_API unsigned int loot_get_version(unsigned int * const versionMajor, unsigned int * const versionMinor, unsigned int * const versionPatch) {
  if (versionMajor == nullptr || versionMinor == nullptr || versionPatch == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  *versionMajor = loot::LootVersion::major;
  *versionMinor = loot::LootVersion::minor;
  *versionPatch = loot::LootVersion::patch;

  return loot_ok;
}

LOOT_C_API unsigned int loot_get_build_id(const char ** const revision) {
  if (revision == nullptr)
    return c_error(loot_error_argument, "Null message pointer passed.");

  *revision = loot::LootVersion::revision.c_str();

  return loot_ok;
}

////////////////////////////////////
// Lifecycle Management Functions
////////////////////////////////////

LOOT_C_API unsigned int loot_create_db(loot_db ** const db,
                                       const unsigned int clientGame,
                                       const char * const gamePath,
                                       const char * const gameLocalPath) {
  if (db == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");
  if (GameType(clientGame) != GameType::tes4
      && GameType(clientGame) != GameType::tes5
      && GameType(clientGame) != GameType::fo3
      && GameType(clientGame) != GameType::fonv
      && GameType(clientGame) != GameType::fo4
      && GameType(clientGame) != GameType::tes5se) {
    return c_error(loot_error_argument, "Invalid game type passed.");
  }

  std::string game_path = "";
  if (gamePath != nullptr)
    game_path = gamePath;

  std::string game_local_path = "";
  if (gameLocalPath != nullptr)
    game_local_path = gameLocalPath;
#ifndef _WIN32
  else
    return c_error(loot_error_argument, "A local data path must be supplied on non-Windows platforms.");
#endif

  try {
    *db = new loot_db(GameType(clientGame), game_path, game_local_path);
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  return loot_ok;
}

// Destroys the given DB, freeing any memory allocated as part of its use.
LOOT_C_API void     loot_destroy_db(loot_db * const db) {
  delete db;
}

///////////////////////////////////
// Database Loading Functions
///////////////////////////////////

LOOT_C_API unsigned int loot_load_lists(loot_db * const db, const char * const masterlistPath,
                                        const char * const userlistPath) {
  if (db == nullptr || masterlistPath == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  try {
    std::string userlistPathString;
    if (userlistPath != nullptr)
      userlistPathString = userlistPath;

    db->getDatabase()->LoadLists(masterlistPath, userlistPathString);
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  //Also free memory.
  db->clearArrays();

  return loot_ok;
}

LOOT_C_API unsigned int loot_eval_lists(loot_db * const db) {
  if (db == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  try {
    db->getDatabase()->EvalLists();
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  return loot_ok;
}

////////////////////////////////////
// LOOT Functionality Functions
////////////////////////////////////

LOOT_C_API unsigned int loot_sort_plugins(loot_db * const db,
                                          const char ** const plugins,
                                          const size_t numPlugins) {
  using std::begin;
  using std::end;
  using std::find;
  using std::distance;

  if (db == nullptr || plugins == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  if (numPlugins == 0)
    return loot_ok;

  try {
    auto sortedPlugins = db->getDatabase()->SortPlugins(std::vector<std::string>(plugins, plugins + numPlugins));

    std::sort(plugins, plugins + numPlugins, [&](const char * const a, const char * const b) {
      auto aIt = find(begin(sortedPlugins), end(sortedPlugins), a);
      auto bIt = find(begin(sortedPlugins), end(sortedPlugins), b);

      return distance(begin(sortedPlugins), aIt) < distance(begin(sortedPlugins), bIt);
    });
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  return loot_ok;
}

LOOT_C_API unsigned int loot_update_masterlist(loot_db * const db,
                                               const char * const masterlistPath,
                                               const char * const remoteURL,
                                               const char * const remoteBranch,
                                               bool * const updated) {
  if (db == nullptr || masterlistPath == nullptr || remoteURL == nullptr || remoteBranch == nullptr || updated == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  *updated = false;

  try {
    *updated = db->getDatabase()->UpdateMasterlist(masterlistPath, remoteURL, remoteBranch);
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  return loot_ok;
}

LOOT_C_API unsigned int loot_get_masterlist_revision(loot_db * const db,
                                                     const char * const masterlistPath,
                                                     const bool getShortID,
                                                     const char ** const revisionID,
                                                     const char ** const revisionDate,
                                                     bool * const isModified) {
  if (db == nullptr || masterlistPath == nullptr || revisionID == nullptr || revisionDate == nullptr || isModified == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  *revisionID = nullptr;
  *revisionDate = nullptr;
  *isModified = false;

  bool modified = false;
  try {
    auto info = db->getDatabase()->GetMasterlistRevision(masterlistPath, getShortID);

    if (info.revision_date.empty() || info.revision_id.empty())
      return loot_ok;

    db->setRevisionIdString(info.revision_id);
    db->setRevisionDateString(info.revision_date);
    modified = info.is_modified;
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  *revisionID = db->getRevisionIdString();
  *revisionDate = db->getRevisionDateString();
  *isModified = modified;

  return loot_ok;
}

//////////////////////////
// DB Access Functions
//////////////////////////

LOOT_C_API unsigned int loot_get_plugin_tags(loot_db * const db, const char * const plugin,
                                             const char * const ** const tagIds_added,
                                             size_t * const numTags_added,
                                             const char * const ** const tagIds_removed,
                                             size_t * const numTags_removed,
                                             bool * const userlistModified) {
  if (db == nullptr || plugin == nullptr || tagIds_added == nullptr || numTags_added == nullptr || tagIds_removed == nullptr || numTags_removed == nullptr || userlistModified == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  //Initialise output.
  *tagIds_added = nullptr;
  *tagIds_removed = nullptr;
  *userlistModified = false;
  *numTags_added = 0;
  *numTags_removed = 0;

  try {
    auto tags = db->getDatabase()->GetPluginTags(plugin);

    db->setAddedTags(tags.added);
    db->setRemovedTags(tags.removed);
    *userlistModified = tags.userlist_modified;
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  //Set outputs.
  *numTags_added = db->getAddedTags().size();
  *numTags_removed = db->getRemovedTags().size();

  if (!db->getAddedTags().empty())
    *tagIds_added = reinterpret_cast<const char * const *>(&db->getAddedTags()[0]);

  if (!db->getRemovedTags().empty())
    *tagIds_removed = reinterpret_cast<const char * const *>(&db->getRemovedTags()[0]);

  return loot_ok;
}

// Returns the messages attached to the given plugin. Messages are valid until Load,
// loot_destroy_db or loot_get_plugin_messages are next called. plugin is case-insensitive.
// If no messages are attached, *messages will be nullptr and numMessages will equal 0.
LOOT_C_API unsigned int loot_get_plugin_messages(loot_db * const db,
                                                 const char * const plugin,
                                                 const unsigned int language,
                                                 const loot_message ** const messages,
                                                 size_t * const numMessages) {
  if (db == nullptr || plugin == nullptr || messages == nullptr || numMessages == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");
  if (language != loot_lang_english
      && language != loot_lang_spanish
      && language != loot_lang_russian
      && language != loot_lang_french
      && language != loot_lang_chinese
      && language != loot_lang_polish
      && language != loot_lang_brazilian_portuguese
      && language != loot_lang_finnish
      && language != loot_lang_german
      && language != loot_lang_danish)
    return c_error(loot_error_argument, "Invalid language code given.");

  //Initialise output.
  *messages = nullptr;
  *numMessages = 0;

  try {
    auto messages = db->getDatabase()->GetPluginMessages(plugin, loot::LanguageCode(language));

    if (messages.empty())
      return loot_ok;

    db->setPluginMessages(messages);
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  *messages = &db->getPluginMessages()[0];
  *numMessages = db->getPluginMessages().size();

  return loot_ok;
}

LOOT_C_API unsigned int loot_get_dirty_info(loot_db * const db, const char * const plugin, unsigned int * const needsCleaning) {
  if (db == nullptr || plugin == nullptr || needsCleaning == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  *needsCleaning = loot_cleanliness_unknown;

  try {
    *needsCleaning = static_cast<unsigned int>(db->getDatabase()->GetPluginCleanliness(plugin));
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  return loot_ok;
}

// Writes a minimal masterlist that only contains mods that have Bash Tag suggestions,
// and/or dirty messages, plus the Tag suggestions and/or messages themselves and their
// conditions, in order to create the Wrye Bash taglist. outputFile is the path to use
// for output. If outputFile already exists, it will only be overwritten if overwrite is true.
LOOT_C_API unsigned int loot_write_minimal_list(loot_db * const db, const char * const outputFile, const bool overwrite) {
  if (db == nullptr || outputFile == nullptr)
    return c_error(loot_error_argument, "Null pointer passed.");

  try {
    db->getDatabase()->WriteMinimalList(outputFile, overwrite);
  } catch (std::exception&) {
    return c_error(std::current_exception());
  }

  return loot_ok;
}
