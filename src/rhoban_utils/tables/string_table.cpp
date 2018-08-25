#include "rhoban_utils/tables/string_table.h"

#include "rhoban_utils/io_tools.h"
#include "rhoban_utils/util.h"

namespace rhoban_utils {

StringTable::StringTable() {}

StringTable::StringTable(const std::vector<std::string> & column_names_,
                         const std::map<std::string,Column> & data_)
  : column_names(column_names_), data(data_)
{
}

StringTable StringTable::buildFromFile(const std::string & file_path,
                                       char separator,
                                       bool has_header) {
  return buildFromString(file2string(file_path), separator, has_header);
}

StringTable StringTable::buildFromString(const std::string & str,
                                         char separator,
                                         bool has_header) {
  // Convert string to lines
  std::vector<std::string> lines;
  split(str, '\n', lines);
  // If there are no lines throw error
  if (lines.size() == 0) {
    throw std::runtime_error(DEBUG_INFO + " no lines in string");
  }
  // Find out column names or generate them
  std::vector<std::string> column_names;
  int first_line_idx = 0;
  if (has_header) {
    first_line_idx = 1;
    split(lines[0], separator, column_names);
  } else {
    // Number of columns is defined by first line columns
    std::vector<std::string> tmp;
    split(lines[0], separator, tmp);
    for (size_t col = 0; col < tmp.size(); col++) {
      column_names.push_back("col_" + std::to_string(col));
    }
  }
  // Fill content
  std::map<std::string, Column> data;
  for (const std::string & col : column_names) {
    data[col] = std::vector<std::string>();
  }

  for (size_t line_idx = first_line_idx; line_idx < lines.size(); line_idx++) {
    const std::string & line = lines[line_idx];
    std::vector<std::string> values;
    split(line, separator, values);
    if (values.size() != column_names.size()) {
      throw std::runtime_error(DEBUG_INFO +
                               " invalid row " + std::to_string(line_idx) +
                               ": expecting " + std::to_string(column_names.size()) +
                               " columns, " + std::to_string(values.size()) + " received");
    }
    for (size_t col = 0; col < values.size(); col++) {
      data[column_names[col]].push_back(values[col]);
    }
  }
  return StringTable(column_names, data);
}

size_t StringTable::nbCols() const {
  return column_names.size();
}

size_t StringTable::nbRows() const {
  return getColumn(column_names[0]).size();
}

const std::vector<std::string> & StringTable::getColumnNames() const {
  return column_names;
}

const StringTable::Column & StringTable::getColumn(const std::string & column_name) const {
  return data.at(column_name);
}

std::map<std::string, std::string> StringTable::getRow(size_t row) const {
  if (row >= nbRows()) {
    throw std::out_of_range(DEBUG_INFO + "asking for row " + std::to_string(row) +
                            " while table has " + std::to_string(nbRows()) +
                            " rows (indices start at 0)");
  }
  std::map<std::string, std::string> result;
  for (const std::string col_name : column_names) {
    result[col_name] = data.at(col_name)[row];
  }
  return result;
}

}
