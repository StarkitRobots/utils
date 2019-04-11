#include "starkit_utils/tables/double_table.h"

#include "starkit_utils/io_tools.h"
#include "starkit_utils/util.h"
#include "starkit_utils/tables/string_table.h"

namespace starkit_utils
{
DoubleTable::DoubleTable()
{
}

DoubleTable::DoubleTable(const std::vector<std::string>& column_names_, const Eigen::MatrixXd& data_)
  : column_names(column_names_), data(data_)
{
  if (column_names.size() != (size_t)data.cols())
  {
    throw std::runtime_error(DEBUG_INFO + std::to_string(column_names.size()) + " column names provided and " +
                             std::to_string(data.cols()) + " columns in data");
  }
  // TODO: add test on matching number of columns with data
  int idx = 0;
  for (const std::string& col_name : column_names)
  {
    column_indices[col_name] = idx++;
  }
}

DoubleTable DoubleTable::buildFromFile(const std::string& file_path, char separator, bool has_header)
{
  return buildFromString(file2string(file_path), separator, has_header);
}

DoubleTable DoubleTable::buildFromString(const std::string& str, char separator, bool has_header)
{
  StringTable str_table = StringTable::buildFromString(str, separator, has_header);
  return buildFromStringTable(str_table);
}

DoubleTable DoubleTable::buildFromStringTable(const StringTable& str_table)
{
  int rows = str_table.nbRows();
  int cols = str_table.nbCols();
  const std::vector<std::string>& column_names = str_table.getColumnNames();

  Eigen::MatrixXd data(rows, cols);
  for (int col_idx = 0; col_idx < cols; col_idx++)
  {
    StringTable::Column col = str_table.getColumn(column_names[col_idx]);
    for (int row_idx = 0; row_idx < rows; row_idx++)
    {
      try
      {
        data(row_idx, col_idx) = std::stod(col[row_idx]);
      }
      catch (const std::invalid_argument& exc)
      {
        throw std::invalid_argument(DEBUG_INFO + " content at (" + std::to_string(row_idx) + "," +
                                    std::to_string(col_idx) + ") is not a valid number'" + col[row_idx] + "'");
      }
      catch (const std::out_of_range& exc)
      {
        throw std::out_of_range(DEBUG_INFO + " content at (" + std::to_string(row_idx) + "," + std::to_string(col_idx) +
                                ") is out of double range'" + col[row_idx] + "'");
      }
    }
  }
  return DoubleTable(column_names, data);
}

size_t DoubleTable::nbCols() const
{
  return data.cols();
}

size_t DoubleTable::nbRows() const
{
  return data.rows();
}

const std::vector<std::string>& DoubleTable::getColumnNames() const
{
  return column_names;
}

Eigen::VectorXd DoubleTable::getColumn(const std::string& column_name) const
{
  return data.col(column_indices.at(column_name));
}

Eigen::VectorXd DoubleTable::getRow(size_t row) const
{
  if (row >= nbRows())
  {
    throw std::out_of_range(DEBUG_INFO + "asking for row " + std::to_string(row) + " while table has " +
                            std::to_string(nbRows()) + " rows (indices start at 0)");
  }
  return data.row(row).transpose();
}

double DoubleTable::getValue(const std::string& col_name, size_t row) const
{
  return getColumn(col_name)(row);
}

}  // namespace starkit_utils
