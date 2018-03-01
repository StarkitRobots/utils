/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
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

#include "rhoban_utils/util.h"

using namespace std;

namespace rhoban_utils
{

std::string slurpFile(const std::string& path)
{
  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw std::runtime_error("rhoban_utils::slurpFile: Failed to open file '" + path + "'");
}

list<string> &split_list(const string &s, char delim, list<string> &elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

string system_time() {
  time_t t = time(0);   // get time now
  struct tm * now = localtime( & t );
  stringstream str;
  str << (now->tm_year + 1900) << '-' 
       << (now->tm_mon + 1) << '-'
       <<  now->tm_mday
       << "-" << now->tm_hour
       << "-" << now->tm_min
       << "-" << now->tm_sec;
  return str.str();
}

bool endsWith(std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

vector<string> getLines(const string &header)
{
    vector<string> lines;
    string line = "";
    for (unsigned int pos=0; pos<header.length(); pos++) {
        if (header[pos] == '\r' || header[pos] == '\n') {
            if (line != "") {
                lines.push_back(line);
            }
            line = "";
        } else {
            line += header[pos];
        }
    }
    return lines;
}

std::string camelize(std::string input)
{
#ifndef MSVC
    std::string output;
    bool caps = false;

    for (unsigned int k=0; k<input.size(); k++) {
        if (std::isspace(input[k])) {
            caps = true;
        } else {
            if (caps) {
                caps = false;
                output += toupper(input[k]);
            } else {
                output += input[k];
            }
        }
    }

    return output;
#else
	throw std::exception("Unimplemented");
#endif
}

string today()
{
	time_t tt;
	time(&tt);
	auto ttt = localtime(&tt);
	char result[80];
	strftime(result, 80, "%Y_%m_%d", ttt);
	return string(result);
}

std::string file_get_contents(std::string path)
{
    std::ifstream ifs(path.c_str());
    std::string content((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));
    return content;
}

void file_put_contents(std::string path, std::string contents)
{
    std::ofstream ofile(path.c_str());

    if (ofile) {
        ofile << contents;
        ofile.close();
    }   
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

bool strContains(const std::string &str, const std::string &other) {
    return str.find(other) != std::string::npos;
}

bool file_exists (const std::string& name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

std::string getBaseName(const std::string & path)
{
  std::vector<std::string> elems;
  split(path,'/',elems);
  return elems[elems.size() - 1];
}

std::string getDirName(const std::string & path)
{
  std::vector<std::string> elems;
  split(path,'/',elems);
  // If there is no prefix, return "./"
  if (elems.size() < 2) {
    return "./";
  }
  // Otherwise, concatenate elements  // If there 
  if (elems.size() < 2) return "./";

  std::ostringstream oss;
  for (size_t idx = 0; idx < elems.size() - 1; idx++) {
    oss << elems[idx] << "/";
  }
  return oss.str();
}

}
