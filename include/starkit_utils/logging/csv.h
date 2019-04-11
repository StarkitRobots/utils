#ifndef _STARKIT_CSV_H
#define _STARKIT_CSV_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>

namespace starkit_utils
{
class CSV
{
public:
  CSV();

  void open(std::string);
  void push(std::string column, double value);
  void newLine();
  void close();

protected:
  std::ofstream ofs;
  bool header;
  std::map<std::string, int> columns;
  std::map<int, std::string> columnIndexes;
  std::map<int, double> values;

  void produceHeader();
};
}  // namespace starkit_utils

#endif
