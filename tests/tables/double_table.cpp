#include <gtest/gtest.h>

#include "starkit_utils/tables/double_table.h"

using namespace starkit_utils;

TEST(DoubleTable, emptyBuilder)
{
  DoubleTable table;
  EXPECT_EQ(table.nbCols(), 0);
  EXPECT_EQ(table.nbRows(), 0);
  EXPECT_EQ(table.getColumnNames().size(), 0);
}

TEST(DoubleTable, initialization)
{
  std::vector<std::string> column_names = { "col_1", "col_2" };
  Eigen::MatrixXd data(3, 2);

  DoubleTable table(column_names, data);
  EXPECT_EQ(table.nbCols(), 2);
  EXPECT_EQ(table.nbRows(), 3);
  EXPECT_EQ(table.getColumnNames()[0], "col_1");
  EXPECT_EQ(table.getColumnNames()[1], "col_2");
}

TEST(DoubleTable, unorderedColumns)
{
  std::vector<std::string> column_names = { "colZ", "colA", "colB" };
  Eigen::MatrixXd data(1, 3);
  data(0, 0) = 1;
  data(0, 1) = 2;
  data(0, 2) = 3;

  DoubleTable table(column_names, data);
  EXPECT_EQ(table.nbCols(), 3);
  EXPECT_EQ(table.nbRows(), 1);
  EXPECT_EQ(table.getColumnNames()[0], "colZ");
  EXPECT_EQ(table.getColumnNames()[1], "colA");
  EXPECT_EQ(table.getColumnNames()[2], "colB");
  EXPECT_EQ(table.getValue("colZ", 0), 1);
  EXPECT_EQ(table.getValue("colA", 0), 2);
  EXPECT_EQ(table.getValue("colB", 0), 3);
}

TEST(DoubleTable, buildFromString)
{
  std::string str = "col1,col2,col3\n0.0,0.5,1.0\n1.0,1.5,2.0";
  DoubleTable table = DoubleTable::buildFromString(str);
  EXPECT_EQ(table.nbCols(), 3);
  EXPECT_EQ(table.nbRows(), 2);
  std::vector<std::string> column_names = table.getColumnNames();
  EXPECT_EQ(column_names[0], "col1");
  EXPECT_EQ(column_names[1], "col2");
  EXPECT_EQ(column_names[2], "col3");
  for (size_t row = 0; row < table.nbRows(); row++)
  {
    for (size_t col = 0; col < table.nbCols(); col++)
    {
      double expected_value = row * 1 + col * 0.5;
      std::string col_name = column_names[col];
      EXPECT_EQ(table.getColumn(col_name)(row), expected_value);
      EXPECT_EQ(table.getRow(row)(col), expected_value);
      EXPECT_EQ(table.getValue(col_name, row), expected_value);
    }
  }
}
TEST(DoubleTable, buildFromFile)
{
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
