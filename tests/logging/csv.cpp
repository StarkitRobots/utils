#include <gtest/gtest.h>
#include <starkit_utils/logging/csv.h>
#include <fstream>
#include <iostream>
#include <string>

using std::ios;
using namespace std;
using namespace starkit_utils;

const std::string file_name = "test.csv";

bool is_empty(std::ifstream& pFile)
{
  return pFile.peek() == std::ifstream::traits_type::eof();
}

bool CheckWord(char* search)
{
  int offset;
  string line;
  ifstream file;
  file.open(file_name);

  if (file.is_open())
  {
    while (!file.eof())
    {
      getline(file, line);
      if ((unsigned)(offset = line.find(search, 0)) != (unsigned)string::npos)
      {
        cout << ":) found '" << search << " \n\n" << line << endl;
        return true;
      }
      else
      {
        cout << ":( Oups couldn't found '" << search << " \n\n" << line << endl;
      }
    }
    file.close();
  }
  else
  {
    cout << "Unable to open this file." << endl;
  }

  return false;
}

void create_csv(std::string name)
{
  fstream fs;
  fs.open(name, ios::out);
  fs.close();
}

TEST(csv, CheckWord)
{
  create_csv(file_name);
  CSV* csv = new CSV();
  /// open, open a file
  csv->open(file_name);
  /// push add the given string to an array to be written
  csv->push("Adding New Word !", 0);
  /// newLine is responsible of writting what's in the array also if the file
  /// has no header it produce one
  csv->newLine();
  /// close, close a file
  csv->close();
  char word[40] = "# 0: Adding New Word !";
  EXPECT_EQ(true, CheckWord(word));
  /// In the csv classe there is a protected method called "produceHeader", we
  /// can test it by searching the "#" in the file because this méthode is the
  /// only one who can write the "#" symbole
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
