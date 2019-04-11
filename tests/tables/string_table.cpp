#include <gtest/gtest.h>

#include "starkit_utils/tables/string_table.h"

using namespace starkit_utils;

TEST(StringTable, emptyBuilder)
{
  StringTable table;
  EXPECT_EQ(table.nbCols(), 0);
  EXPECT_EQ(table.nbRows(), 0);
  EXPECT_EQ(table.getColumnNames().size(), 0);
}

TEST(StringTable, initialization)
{
  std::vector<std::string> column_names = { "col_1", "col_2" };
  std::map<std::string, StringTable::Column> data = { { "col_1", { "e(1;1)", "e(2;1)", "e(3;1)}" } },
                                                      { "col_2", { "e(1;2)", "e(2;2)", "e(3;2)}" } } };

  StringTable table(column_names, data);
  EXPECT_EQ(table.nbCols(), 2);
  EXPECT_EQ(table.nbRows(), 3);
  EXPECT_EQ(table.getColumnNames()[0], "col_1");
  EXPECT_EQ(table.getColumnNames()[1], "col_2");
}

TEST(StringTable, unorderedColumns)
{
  std::vector<std::string> column_names = { "colZ", "colA", "colB" };
  std::map<std::string, StringTable::Column> data = { { "colZ", {} }, { "colA", {} }, { "colB", {} } };

  StringTable table(column_names, data);
  EXPECT_EQ(table.nbCols(), 3);
  EXPECT_EQ(table.nbRows(), 0);
  EXPECT_EQ(table.getColumnNames()[0], "colZ");
  EXPECT_EQ(table.getColumnNames()[1], "colA");
  EXPECT_EQ(table.getColumnNames()[2], "colB");
}

TEST(StringTable, buildFromString)
{
  std::string str = "col1,col2,col3\ne(1;1),e(1;2),e(1;3)\ne(2;1),e(2;2),e(2;3)";
  StringTable table = StringTable::buildFromString(str);
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
      std::string expected_str = "e(" + std::to_string(row + 1) + ";" + std::to_string(col + 1) + ")";
      std::string col_name = column_names[col];
      EXPECT_EQ(table.getColumn(col_name)[row], expected_str);
      EXPECT_EQ(table.getRow(row).at(col_name), expected_str);
    }
  }
}
TEST(StringTable, buildFromFile)
{
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
