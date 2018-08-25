#pragma once

#include <map>
#include <string>
#include <vector>

namespace rhoban_utils {

/// A string table is a table in which each of the column is named
class StringTable {
public:
  typedef std::vector<std::string> Column;
  
  StringTable();
  StringTable(const std::vector<std::string> & column_names,
              const std::map<std::string, Column> & data);

  /// Presence of 'separator' in column content is not supported
  static StringTable buildFromFile(const std::string & file_path,
                                   char separator=',',
                                   bool has_header = true);

  /// Presence of 'separator' in column content is not supported
  static StringTable buildFromString(const std::string & str,
                                     char separator=',',
                                     bool has_header = true);

  size_t nbCols() const;
  size_t nbRows() const;

  const std::vector<std::string> & getColumnNames() const;

  /// Throws std::out_of_range if column_name does not exist
  const Column & getColumn(const std::string & column_name) const;

  /// Return a map with column_name as key and content as value
  std::map<std::string, std::string> getRow(size_t row) const;
  
private:
  std::vector<std::string> column_names;

  /// Stored by column_name
  std::map<std::string, Column> data;
};

}
