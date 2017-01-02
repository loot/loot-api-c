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

/**
 *  @mainpage
 *  @author WrinklyNinja
 *  @version 0.9.2
 *  @copyright
 *      The LOOT API is distributed under the GNU General Public License v3.0.
 *      For the full text of the license, see the "GNU GPL v3.txt" file
 *      included in the source archive.
 *
 *  @section intro_sec Introduction
 *      LOOT is a utility that helps users avoid serious conflicts between
 *      their mods by setting their plugins in an optimal load order. It also
 *      provides tens of thousands of plugin-specific messages, including usage
 *      notes, requirements, incompatibilities, bug warnings and installation
 *      mistake notifications, and thousands of Bash Tag suggestions.
 *
 *      This metadata that LOOT supplies is stored in its masterlist, which is
 *      maintained by the LOOT team using information provided by mod authors
 *      and users. Users can also add to and modify the metadata used by LOOT
 *      through the use of userlist files. The LOOT API provides a way for
 *      third-party developers to access this metadata for use in their own
 *      programs.
 *
 *      All further API documentation is contained within the documentation for
 *      api.h.
 *
 * @section credit_sec Credits
 *      The LOOT API is written by [WrinklyNinja]
 *      (https://github.com/WrinklyNinja) in C/C++ and makes use of the
 *      [Boost](http://www.boost.org/),
 *      [libespm](https://github.com/WrinklyNinja/libespm),
 *      [libgit2](https://github.com/libgit2/libgit2),
 *      [libloadorder](https://github.com/WrinklyNinja/libloadorder/),
 *      [Pseudosem](https://github.com/WrinklyNinja/pseudosem) and
 *      [yaml-cpp](https://github.com/WrinklyNinja/yaml-cpp) libraries. Copyright
 *      license information for all these may be found in the
 *      "docs/licenses/Licenses.txt" file.
 *
 * @section history_sec Version History
 *      ## 0.9.2 - *3 August 2016*
 *
 *        * Fixed the wrong API binary being packaged. This caused the v0.9.0 and v0.9.1 API releases to actually be re-releases of a snapshot build made at some point between v0.8.1 and v0.9.0: the affected API releases were taken offline once this was discovered.
 *        * Fixed `loot_get_plugin_tags()`` remembering results and including them in the results of subsequent calls.
 *        * Fixed an error occurring when the user's temporary files directory didn't exist and updating the masterlist tried to create a directory there.
 *        * Fixed errors when reading some Oblivion plugins during sorting, including the official DLC.
 *        * libespm (2.5.5) and Pseudosem (1.1.0) dependencies have been updated to the versions given in brackets.
 *
 *      ## 0.9.1 - *23 June 2016*
 *
 *        * No API changes.
 *
 *      ## 0.9.0 - *21 May 2016*
 *
 *        * Moved API header location to the more standard `include/loot/api.h`.
 *        * Documented LOOT's masterlist versioning system.
 *        * Made all API outputs fully const to make it clear they should not
 *          be modified and to avoid internal const casting.
 *        * Removed the `loot_cleanup()` function, as the one string it used to
 *          destroy is now stored on the stack and so destroyed when the API is
 *          unloaded.
 *        * The `loot_db` type is now an opaque struct, and functions that used
 *          to take it as a value now take a pointer to it.
 *        * Removed the `loot_lang_any` constant. The `loot_lang_english`
 *          constant should be used instead.
 *
 *      ## 0.8.1 - *27 September 2015*
 *
 *        * Fixed crash when loading plugins due to lack of thread safety.
 *        * Fixed masterlist updater and validator not checking for valid
 *          condition and regex syntax.
 *        * Check for safe file paths when parsing conditions.
 *        * Updated Boost (1.59.0), libgit2 (0.23.2) and CEF (branch 2454)
 *          dependencies. This fixes the masterlist updater not working
 *          correctly for Windows Vista users.
 *
 *      ## 0.8.0 - *22 July 2015*
 *
 *        * Fixed many miscellaneous bugs, including initialisation crashes and
 *          incorrect metadata input/output handling.
 *        * Fixed LOOT silently discarding some non-unique metadata: an error
 *          will now occur when loading or attempting to apply such metadata.
 *        * Fixed and improved LOOT's version comparison behaviour for a wide
 *          variety of version string formats. This involved removing LOOT's
 *          usage of the Alphanum code library.
 *        * Improved plugin loading performance for computers with weaker
 *          multithreading capabilities (eg. non-hyperthreaded dual-core or
 *          single-core CPUs).
 *        * LOOT no longer outputs validity warnings for inactive plugins.
 *        * Metadata syntax support changes, see the metadata syntax document
 *          for details.
 *        * Updated libgit2 to v0.23.0.
 *
 *      ## 0.7.1 - *22 June 2015*
 *
 *        * Fixed "No existing load order position" errors when sorting.
 *        * Fixed output of Bash Tag removal suggestions in
 *          `loot_write_minimal_list()`.
 *
 *      ## 0.7.0 - *20 May 2015*
 *
 *        * Initial API release.
 */

/**
 *  @file api.h
 *  @brief This file contains the API frontend.
 *
 *  @note The LOOT API is *not* thread safe. Thread safety is a goal, but one
 *        that has not yet been achieved. Bear this in mind if using it in a
 *        multi-threaded client.
 *
 *  @section var_sec Variable Types
 *
 *  The LOOT API uses character strings and integers for information
 *  input/output.
 *  - All strings are null-terminated byte character strings encoded in UTF-8.
 *  - All codes are unsigned integers at least 16 bits in size.
 *  - All array sizes are unsigned integers at least 16 bits in size.
 *  - File paths are case-sensitive if and only if the underlying file system
 *    is case-sensitive.
 *
 *  @section memory_sec Memory Management
 *
 *  The LOOT API manages the memory of strings and arrays it returns, so such
 *  strings and arrays should not be deallocated by the client.
 *
 *  Data returned by a function lasts until a function is called which returns
 *  data of the same type (eg. a string is stored until the client calls
 *  another function which returns a string, an integer array lasts until
 *  another integer array is returned, etc.).
 *
 *  All allocated memory is freed when loot_destroy_db() is called, except the
 *  string allocated by loot_get_error_message(), which must be freed by
 *  calling loot_cleanup().
 */

#ifndef LOOT_LOOT_C_API
#define LOOT_LOOT_C_API

#include <stddef.h>

#if defined(_MSC_VER)
/* MSVC doesn't support C99, so do the stdbool.h definitions ourselves.
   START OF stdbool.h DEFINITIONS. */
#   ifndef __cplusplus
#       define bool  _Bool
#       define true  1
#       define false 0
#   endif
#   define __bool_true_false_are_defined 1
/* END OF stdbool.h DEFINITIONS. */
#else
#   include <stdbool.h>
#endif

/* set up dll import/export decorators
   when compiling the dll on windows, ensure LOOT_EXPORT is defined. clients
   that use this header do not need to define anything to import the symbols
   properly. */
#if defined(_WIN32) || defined(_WIN64)
#   ifdef LOOT_C_API_STATIC
#       define LOOT_C_API
#   elif defined LOOT_C_API_EXPORT
#       define LOOT_C_API __declspec(dllexport)
#   else
#       define LOOT_C_API __declspec(dllimport)
#   endif
#else
#   define LOOT_C_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /**********************************************************************//**
     * Types
     *************************************************************************/

    /**
     *  @brief A structure that holds all game-specific data used by the API.
     *  @details Used to keep each game's data independent. Abstracts the
     *           definition of the API's internal state while still providing
     *           type safety across the library. Multiple handles can also be
     *           made for each game, though it should be kept in mind that the
     *           API is not thread-safe.
     */
  typedef struct loot_db loot_db;

  /**
   *  @brief A structure that holds the type of a message and the message
   *      string itself.
   */
  typedef struct {
      /**
       *  @var type
       *      The type of the message, specified using one of the message
       *      type codes.
       *  @var message
       *      The message string, which may be formatted using
       *      [GitHub Flavored Markdown]
       *      (https://help.github.com/articles/github-flavored-markdown).
       */
    unsigned int type;
    const char * message;
  } loot_message;

  /**********************************************************************//**
   *  @name Return Codes
   *  @brief Error codes signify an issue that caused a function to exit
   *         prematurely. If a function exits prematurely, a reversal of any
   *         changes made during its execution is attempted before it exits.
   *************************************************************************/
  /**@{*/

  LOOT_C_API extern const unsigned int loot_ok;  /**< The function completed successfully. */
  LOOT_C_API extern const unsigned int loot_error_unknown;  /**< An unknown error occurred. */
  LOOT_C_API extern const unsigned int loot_error_file_access;  /**< A file could not be accessed. */
  LOOT_C_API extern const unsigned int loot_error_condition_syntax;  /**< There was an error parsing a metadata condition. */
  LOOT_C_API extern const unsigned int loot_error_regex_syntax;  /**< There was an error parsing a regular expression. */
  LOOT_C_API extern const unsigned int loot_error_game_detection;  /**< The given game could not be found. */
  LOOT_C_API extern const unsigned int loot_error_libloadorder;  /**< There was an error in performing a load order operation. */
  LOOT_C_API extern const unsigned int loot_error_libgit2;  /**< An error occurred while performing a git operation (updating or getting the masterlist version). */
  LOOT_C_API extern const unsigned int loot_error_os;   /**< An error occurred during a call to an operating system API. */
  LOOT_C_API extern const unsigned int loot_error_cyclic_interaction;  /**< A cyclic interaction was found while sorting plugins. */
  LOOT_C_API extern const unsigned int loot_error_git_state;  /**< An error occurred while performing a git operation (updating or getting the masterlist version). */
  LOOT_C_API extern const unsigned int loot_error_no_memory;  /**< The API was unable to allocate the required memory. */
  LOOT_C_API extern const unsigned int loot_error_argument;  /**< Invalid arguments were given for the function. */

  /**
   *  @brief Matches the value of the highest-numbered return code.
   *  @details Provided in case clients wish to incorporate additional return
   *           codes in their implementation and desire some method of
   *           avoiding value conflicts.
   */
  LOOT_C_API extern const unsigned int loot_return_max;

  /**@}*/
  /**********************************************************************//**
   *  @name Game Codes
   *  @brief Used with loot_create_db().
   *************************************************************************/
  /**@{*/

  LOOT_C_API extern const unsigned int loot_game_tes4;  /**< Game code for The Elder Scrolls IV: Oblivion. */
  LOOT_C_API extern const unsigned int loot_game_tes5;  /**< Game code for The Elder Scrolls V: Skyrim. */
  LOOT_C_API extern const unsigned int loot_game_tes5se;  /**< Game code for The Elder Scrolls V: Skyrim. */
  LOOT_C_API extern const unsigned int loot_game_fo3;  /**< Game code for Fallout 3. */
  LOOT_C_API extern const unsigned int loot_game_fonv;  /**< Game code for Fallout: New Vegas. */
  LOOT_C_API extern const unsigned int loot_game_fo4;  /**< Game code for Fallout: New Vegas. */

  /**@}*/
  /**********************************************************************//**
   *  @name Message Type Codes
   *  @brief Used with the loot_message structure.
   *************************************************************************/
  /**@{*/

  LOOT_C_API extern const unsigned int loot_message_say;  /**< Denotes a generic note-type message. */
  LOOT_C_API extern const unsigned int loot_message_warn;  /**< Denotes a warning message. */
  LOOT_C_API extern const unsigned int loot_message_error;  /**< Denotes an error message. */

  /**@}*/
  /**********************************************************************//**
   *  @name Message Language Codes
   *  @brief Used with loot_eval_lists().
   *************************************************************************/
  /**@{*/

  LOOT_C_API extern const unsigned int loot_lang_english;  /**< Tells the API to preferentially select English messages. */
  LOOT_C_API extern const unsigned int loot_lang_spanish;  /**< Tells the API to preferentially select Spanish messages */
  LOOT_C_API extern const unsigned int loot_lang_russian;  /**< Tells the API to preferentially select Russian messages. */
  LOOT_C_API extern const unsigned int loot_lang_french;  /**< Tells the API to preferentially select French messages. */
  LOOT_C_API extern const unsigned int loot_lang_chinese;  /**< Tells the API to preferentially select Chinese messages. */
  LOOT_C_API extern const unsigned int loot_lang_polish;  /**< Tells the API to preferentially select Polish messages. */
  LOOT_C_API extern const unsigned int loot_lang_brazilian_portuguese;  /**< Tells the API to preferentially select Brazilian Portuguese messages. */
  LOOT_C_API extern const unsigned int loot_lang_finnish; /**< Tells the API to preferentially select Finnish messages. */
  LOOT_C_API extern const unsigned int loot_lang_german; /**< Tells the API to preferentially select German messages. */
  LOOT_C_API extern const unsigned int loot_lang_danish; /**< Tells the API to preferentially select Danish messages. */
  LOOT_C_API extern const unsigned int loot_lang_korean; /**< Tells the API to preferentially select Korean messages. */

  /**@}*/
  /**********************************************************************//**
   *  @name Plugin Cleanliness Codes
   *  @brief Used with loot_get_dirty_info().
   *************************************************************************/
  /**@{*/

  LOOT_C_API extern const unsigned int loot_cleanliness_clean;  /**< Denotes that the plugin queried does not need cleaning. */
  LOOT_C_API extern const unsigned int loot_cleanliness_dirty;  /**< Denotes that the plugin queried needs cleaning. */
  LOOT_C_API extern const unsigned int loot_cleanliness_do_not_clean;  /**< Denotes that the plugin queried should not be cleaned. */
  LOOT_C_API extern const unsigned int loot_cleanliness_unknown;  /**< Denotes that the API is unable to determine whether or not the plugin queried needs cleaning. */

  /**@}*/
  /**********************************************************************//**
   *  @name Error Handling Functions
   *************************************************************************/
  /**@{*/

  /**
   *  @brief Returns the message for the last error or warning encountered.
   *  @details Outputs a string giving the a message containing the details
   *           of the last error or warning encountered by a function. Each
   *           time this function is called, the memory for the previous
   *           message is freed, so only one error message is available at
   *           any one time.
   *  @param message
   *      A pointer to the error details string outputted by the function.
   *  @returns A return code.
   */
  LOOT_C_API unsigned int loot_get_error_message(const char ** const message);

  /**@}*/
  /**********************************************************************//**
   *  @name Version Functions
   *************************************************************************/
  /**@{*/

  /**
   *  @brief Checks for API compatibility.
   *  @details Checks whether the loaded API is compatible with the given
   *           version of the API, abstracting API stability policy away from
   *           clients. The version numbering used is major.minor.patch.
   *  @param versionMajor
   *      The major version number to check.
   *  @param versionMinor
   *      The minor version number to check.
   *  @param versionPatch
   *      The patch version number to check.
   *  @returns True if the API versions are compatible, false otherwise.
   */
  LOOT_C_API bool loot_is_compatible(const unsigned int versionMajor,
                                     const unsigned int versionMinor,
                                     const unsigned int versionPatch);

    /**
     *  @brief Gets the API version.
     *  @details Outputs the major, minor and patch version numbers for the
     *           loaded API. The version numbering used is major.minor.patch.
     *  @param versionMajor
     *      A pointer to the major version number.
     *  @param versionMinor
     *      A pointer to the minor version number.
     *  @param versionPatch
     *      A pointer to the patch version number.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_get_version(unsigned int * const versionMajor,
                                           unsigned int * const versionMinor,
                                           unsigned int * const versionPatch);

    /**
     *  @brief Get the Git revision of the code from which the binary was
     *         built.
     *  @param revision
     *      A pointer to the shortened Git revision ID string.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_get_build_id(const char ** const revision);

  /**@}*/
  /**********************************************************************//**
   *  @name Lifecycle Management Functions
   *************************************************************************/
  /**@{*/

  /**
   *  @brief Initialise a new database handle.
   *  @details Creates a handle for a database, which is then used by all
   *           database functions.
   *  @param db
   *      A pointer to the handle that is created by the function.
   *  @param clientGame
   *      A game code for which to create the handle.
   *  @param gamePath
   *      The relative or absolute path to the game folder, or `NULL`.
   *      If `NULL`, the API will attempt to detect the data path of the
   *      specified game.
   *  @param gameLocalPath
   *      The relative or absolute path to the game's folder in
   *      `%LOCALAPPDATA%`, or `NULL`. If `NULL`, the API will attempt to
   *      look up the path that `%LOCALAPPDATA%` corresponds to. This
   *      parameter is provided so that systems lacking that environmental
   *      variable (eg. Linux) can still use the API.
   *  @returns A return code.
   */
  LOOT_C_API unsigned int loot_create_db(loot_db ** const db,
                                         const unsigned int clientGame,
                                         const char * const gamePath,
                                         const char * const gameLocalPath);

    /**
     *  @brief Destroy an existing database handle.
     *  @details Destroys the given database handle, freeing up memory
     *           allocated during its use, excluding any memory allocated to
     *           error messages.
     *  @param db
     *      The database handle to destroy.
     */
  LOOT_C_API void loot_destroy_db(loot_db * const db);

  /**@}*/
  /**********************************************************************//**
   *  @name Database Loading Functions
   *************************************************************************/
  /**@{*/

  /**
   *  @brief Loads the masterlist and userlist from the paths specified.
   *  @details Can be called multiple times, each time replacing the
   *           previously-loaded data.
   *  @param db
   *      The database the function acts on.
   *  @param masterlistPath
   *      A string containing the relative or absolute path to the masterlist
   *      file that should be loaded.
   *  @param userlistPath
   *      A string containing the relative or absolute path to the userlist
   *      file that should be loaded, or `NULL`. If `NULL`, no userlist will
   *      be loaded.
   *  @returns A return code.
   */
  LOOT_C_API unsigned int loot_load_lists(loot_db * const db,
                                          const char * const masterlistPath,
                                          const char * const userlistPath);

    /**
     *  @brief Evaluates all conditions and regular expression metadata
     *         entries.
     *  @details Repeated calls re-evaluate the metadata from scratch. This
     *           function affects the output of all the database access
     *           functions.
     *  @param db
     *      The database the function acts on.
     *  @param language
     *      The language code that is used for message language comparisons.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_eval_lists(loot_db * const db);

    /**********************************************************************//**
     *  @name LOOT Functionality Functions
     *************************************************************************/
    /**@{*/

    /**
     *  @brief Calculates a new load order for the game's installed plugins
     *         (including inactive plugins) and outputs the sorted order.
     *  @details Pulls metadata from the masterlist and userlist if they are
     *           loaded, and reads the contents of each plugin. No changes are
     *           applied to the load order used by the game. This function does
     *           not load or evaluate the masterlist or userlist.
     *  @param db
     *      The database the function acts on.
     *  @param plugins
     *      A pointer to an array of plugin filenames to be sorted. If sorting
     *      is successful, this array will be modified to list the plugins in
     *      their sorted load order without reallocating memory.
     *  @param numPlugins
     *      A pointer to the size of the `sortedPlugins` array.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_sort_plugins(loot_db * const db,
                                            const char ** const plugins,
                                            const size_t numPlugins);

    /**
     *  @brief Update the given masterlist.
     *  @details Uses Git to update the given masterlist to a given remote.
     *           If the masterlist doesn't exist, this will create it. This
     *           function also initialises a Git repository in the given
     *           masterlist's parent folder. If the masterlist was not already
     *           up-to-date, it will be re-loaded, but not re-evaluated.
     *
     *           If a Git repository is already present, it will be used to
     *           perform a diff-only update, but if for any reason a
     *           fast-forward merge update is not possible, the existing
     *           repository will be deleted and a new repository cloned from
     *           the given remote.
     *  @param db
     *      The database the function acts on.
     *  @param masterlistPath
     *      A string containing the relative or absolute path to the masterlist
     *      file that should be updated. The filename must match the filename
     *      of the masterlist file in the given remote repository, otherwise it
     *      will not be updated correctly. Although LOOT itself expects this
     *      filename to be "masterlist.yaml", the API does not check for any
     *      specific filename.
     *  @param remoteURL
     *      The URL of the remote from which to fetch updates. This can also be
     *      a relative or absolute path to a local repository.
     *  @param remoteBranch
     *      The branch of the remote from which to apply updates. LOOT's
     *      official masterlists are versioned using separate branches for each
     *      new version of the masterlist syntax, so if you're using them,
     *      check their repositories to see which is the latest release branch.
     *  @param updated
     *      `true` if the masterlist was updated. `false` if no update was
     *      necessary, ie. it was already up-to-date. If `true`, the masterlist
     *      will have been re-loaded, but will need to be re-evaluated
     *      separately.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_update_masterlist(loot_db * const db,
                                                 const char * const masterlistPath,
                                                 const char * const remoteURL,
                                                 const char * const remoteBranch,
                                                 bool * const updated);

    /**
     *  @brief Get the given masterlist's revision.
     *  @details Getting a masterlist's revision is only possible if it is
     *           found inside a local Git repository.
     *  @param db
     *      The database the function acts on.
     *  @param masterlistPath
     *      A string containing the relative or absolute path to the masterlist
     *      file that should be queried.
     *  @param getShortID
     *      If `true`, the shortest unique hexadecimal revision hash that is at
     *      least 7 characters long will be outputted. Otherwise, the full 40
     *      character hash will be outputted.
     *  @param revisionID
     *      A pointer to a string containing the outputted revision hash for
     *      the masterlist. If the masterlist doesn't exist, or there is no Git
     *      repository at its location, this will be `NULL`.
     *  @param revisionDate
     *      A pointer to a string containing the ISO 8601 formatted revision
     *      date, ie. YYYY-MM-DD. If the masterlist doesn't exist, or there is
     *      no Git repository at its location, this will be `NULL`.
     *  @param isModified
     *      A pointer to a boolean that is `true` if the masterlist has been
     *      edited since the outputted revision, or `false` if it is at exactly
     *      the revision given.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_get_masterlist_revision(loot_db * const db,
                                                       const char * const masterlistPath,
                                                       const bool getShortID,
                                                       const char ** const revisionID,
                                                       const char ** const revisionDate,
                                                       bool * const isModified);

    /**@}*/
    /**********************************************************************//**
     *  @name Database Access Functions
     *************************************************************************/
    /**@{*/

    /**
     *  @brief Outputs the Bash Tags suggested for addition and removal by the
     *         database for the given plugin.
     *  @param db
     *      The database the function acts on.
     *  @param plugin
     *      The filename of the plugin to look up Bash Tag suggestions for.
     *  @param tags_added
     *      A pointer to the outputted array of UIDs of the Bash Tags suggested
     *      for addition to the specified plugin. `NULL` if no Bash Tag
     *      additions are suggested.
     *  @param numTags_added
     *      A pointer to the size of the tags_added array. `0` if `tags_added`
     *      is `NULL`.
     *  @param tags_removed
     *      A pointer to the outputted array of UIDs of the Bash Tags suggested
     *      for removal from the specified plugin. `NULL` if no Bash Tag
     *      removals are suggested.
     *  @param numTags_removed
     *      A pointer to the size of the `tags_removed` array. `0` if
     *      `tags_removed` is `null`.
     *  @param userlistModified
     *      `true` if the Bash Tag suggestions were modified by the data in the
     *      userlist, `false` otherwise.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_get_plugin_tags(loot_db * const db,
                                               const char * const plugin,
                                               const char * const ** const tags_added,
                                               size_t * const numTags_added,
                                               const char * const ** const tags_removed,
                                               size_t * const numTags_removed,
                                               bool * const userlistModified);

    /**
     *  @brief Outputs the messages associated with the given plugin in the
     *         database.
     *  @param db
     *      The database the function acts on.
     *  @param plugin
     *      The filename of the plugin to look up messages for.
     *  @param plugin
     *      The preferred language for messages.
     *  @param messages
     *      A pointer to the outputted array of messages associated with the
     *      specified plugin, given as loot_message structures. `NULL` if the
     *      plugin has no messages associated with it.
     *  @param numMessages
     *      A pointer to the size of the outputted array. If no messages are
     *      outputted, this will be `0`.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_get_plugin_messages(loot_db * const db,
                                                   const char * const plugin,
                                                   const unsigned int language,
                                                   const loot_message ** const messages,
                                                   size_t * const numMessages);

    /**
     *  @brief Determines the database's knowledge of a plugin's dirtiness.
     *  @details Outputs whether the plugin should be cleaned or not, or if
     *           no data is available. The mechanism used to determine that
     *           a plugin should not be cleaned is not very reliable, and is
     *           likely to fail if `loot_eval_lists()` was called with a
     *           language other than English. As such, some plugins that should
     *           not be cleaned may have the `loot_needs_cleaning_unknown`
     *           code outputted.
     *  @param db
     *      The database the function acts on.
     *  @param plugin
     *      The plugin to look up dirty status information for.
     *  @param needsCleaning
     *      A pointer to a plugin cleanliness code.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_get_dirty_info(loot_db * const db,
                                              const char * const plugin,
                                              unsigned int * const needsCleaning);

    /**
     *  @brief Writes a minimal metadata file that only contains plugins with
     *         Bash Tag suggestions and/or dirty info, plus the suggestions and
     *         info themselves.
     *  @param db
     *      The database the function acts on.
     *  @param outputFile
     *      The path to which the file shall be written.
     *  @param overwrite
     *      If `false` and `outputFile` already exists, no data will be
     *      written. Otherwise, data will be written.
     *  @returns A return code.
     */
  LOOT_C_API unsigned int loot_write_minimal_list(loot_db * const db,
                                                  const char * const outputFile,
                                                  const bool overwrite);

    /**@}*/

#ifdef __cplusplus
}
#endif

#endif
