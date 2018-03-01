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

namespace rhoban_utils
{

typedef unsigned char ui8;
typedef unsigned short ui16;
typedef unsigned int ui32;
typedef unsigned int uint;

/**
 * Return the content of the whole file 'effectively'
 * cf: http://insanecoding.blogspot.jp/2011/11/how-to-read-in-file-in-c.html
 */
std::string slurpFile(const std::string& path);

std::vector<std::string> &split(const std::string &s, char delim,
				std::vector<std::string> &elems);
std::list<std::string> &split_list(const std::string &s, char delim,
				   std::list<std::string> &elems);

template<typename T>
inline std::string join(const T &tab, std::string delimiter) {
    std::ostringstream oss;
    typename T::const_iterator it; 

    bool notOver;
    it = tab.begin();
    do {
        oss << (*it);
        it++;
        notOver = (it != tab.end());
        if (notOver) {
            oss << delimiter;
        }   
    } while (notOver);

    return oss.str();
}

std::string system_time();

bool endsWith(std::string const &fullString, std::string const &ending);

std::vector<std::string> getLines(const std::string &header);

std::string today();

std::string camelize(std::string input);

std::string file_get_contents(std::string path);
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
