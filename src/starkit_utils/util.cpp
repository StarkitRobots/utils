/*************************************************
 * Publicly released by Starkit System, August 2012
 *             www.starkit-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/

#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include <cerrno>
#include <iomanip>
#include <sstream>

#include "starkit_utils/util.h"

using namespace std;

namespace starkit_utils
{
vector<string>& split(const string& s, char delim, vector<string>& elems)
{
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim))
  {
    elems.push_back(item);
  }
  return elems;
}

void file_put_contents(std::string path, std::string contents)
{
  std::ofstream ofile(path.c_str());

  if (ofile)
  {
    ofile << contents;
    ofile.close();
  }
}

void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
  if (from.empty())
    return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos)
  {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();  // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}

bool strContains(const std::string& str, const std::string& other)
{
  return str.find(other) != std::string::npos;
}

bool file_exists(const std::string& name)
{
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

std::string getBaseName(const std::string& path)
{
  std::vector<std::string> elems;
  split(path, '/', elems);
  return elems[elems.size() - 1];
}

std::string getDirName(const std::string& path)
{
  std::vector<std::string> elems;
  split(path, '/', elems);
  // If there is no prefix, return "./"
  if (elems.size() < 2)
  {
    return "./";
  }
  // Otherwise, concatenate elements  // If there
  if (elems.size() < 2)
    return "./";

  std::ostringstream oss;
  for (size_t idx = 0; idx < elems.size() - 1; idx++)
  {
    oss << elems[idx] << "/";
  }
  return oss.str();
}

}  // namespace starkit_utils
