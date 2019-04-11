#pragma once

#include <map>
#include <string>
#include <vector>

#include <Eigen/Core>

namespace starkit_utils
{
class StringTable;

/// Each column has a name and all the data are composed exclusively of numbers
class DoubleTable
{
  DoubleTable();
  DoubleTable(const std::string& column_names, const Eigen::MatrixXd& data);

  /// Presence of 'separator' in column content is not supported
  static DoubleTable buildFromFile(const std::string& file_path, char separator = ',', bool has_header = true);

  /// Presence of 'separator' in column content is not supported
  static DoubleTable buildFromString(const std::string& str, char separator = ',', bool has_header = true);

  /// Presence of 'separator' in column content is not supported
  static DoubleTable buildFromStringTable(const StringTable& table);

  size_t nbCols() const;
  size_t nbRows() const;

  /// Throws std::out_of_range if column_name does not exist
  Eigen::VectorXd getColumn(const std::string& column_name) const;

  /// Return a map with column_name as key and content as value
  Eigen::VectorXd getRow(int row) const;

private:
  std::vector<std::string> column_names;

  /// The name of the columns in order
  std::map<std::string, int> column_indices;

  /// The content of the table
  Eigen::MatrixXd data;
};

}  // namespace starkit_utils
