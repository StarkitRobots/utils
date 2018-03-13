/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stdexcept>
#include <ctime>

#define DEBUG_INFO                            \
  std::string(__FUNCTION__) + ":"             \
  + rhoban_utils::getBaseName(__FILE__) + ":" \
  + std::to_string(__LINE__)  + ": "

namespace rhoban_utils
{

std::vector<std::string> &split(const std::string &s, char delim,
                                std::vector<std::string> &elems);

void file_put_contents(std::string path, std::string contents);

void replaceAll(std::string& str, const std::string& from, const std::string& to);
bool strContains(const std::string &str, const std::string &other);

bool file_exists (const std::string& name);

/// Return the name of the file at the given path:
/// e.g getBaseName("toto/file.cpp") returns "file.cpp"
std::string getBaseName(const std::string & path);

/// Return the name of the path to the directory containing the given file:
/// e.g getDirName("toto/file.cpp") returns "toto"
/// Remark: getDirName("file.cpp") returns "./"
std::string getDirName(const std::string & path);

}
