#include <gtest/gtest.h>
#include <starkit_utils/io_tools.h>
#include <starkit_utils/util.h>

#define _USE_MATH_DEFINES
#include <cmath>

/*******************************************************
 * Test of file2string
 */

TEST(file2string, testSuccess)
{
  // create dynamically a file
  const std::string absoluteFilePathStr = "/tmp/pfe.txt";
  const char* absoluteFilePath = absoluteFilePathStr.c_str();
  remove(absoluteFilePath);

  std::string testContent = "Hello world";
  starkit_utils::file_put_contents(absoluteFilePath, testContent);

  std::string readContent = starkit_utils::file2string(absoluteFilePathStr);

  EXPECT_EQ(readContent, testContent);
}

TEST(file2string, testFail)
{
  // create dynamically a file
  std::string testContent = "Hello world";
  const std::string absoluteFilePathStr = "zuhfpiufhs.sdùqip";

  try
  {
    starkit_utils::file2string(absoluteFilePathStr);
  }
  catch (std::runtime_error error)
  {
    return;
  }
  EXPECT_TRUE(false);
}

/*******************************************************
 * Test of writeInt and readInt
 */

TEST(write_read_int, testSuccessPos)
{
  std::stringstream stream;
  int writtenInteger = 5;
  int readInteger;
  EXPECT_EQ(sizeof(int), starkit_utils::writeInt(stream, writtenInteger));
  EXPECT_EQ(sizeof(int), starkit_utils::readInt(stream, readInteger));
  EXPECT_EQ(readInteger, writtenInteger);
}

TEST(write_read_int, testSuccessNeg)
{
  std::stringstream stream;
  int writtenInteger = -5;
  int readInteger;
  EXPECT_EQ(sizeof(int), starkit_utils::writeInt(stream, writtenInteger));
  EXPECT_EQ(sizeof(int), starkit_utils::readInt(stream, readInteger));
  EXPECT_EQ(readInteger, writtenInteger);
}

/*******************************************************
 * Test of writeDouble and readDouble
 */

TEST(write_read_double, testSuccessPos)
{
  std::stringstream stream;
  double writtenDouble = M_PI;
  double readDouble;
  EXPECT_EQ(sizeof(double), starkit_utils::writeDouble(stream, writtenDouble));
  EXPECT_EQ(sizeof(double), starkit_utils::readDouble(stream, readDouble));
  EXPECT_EQ(readDouble, writtenDouble);
}

TEST(write_read_double, testSuccessNeg)
{
  std::stringstream stream;
  double writtenDouble = -1 * M_PI;
  double readDouble;
  EXPECT_EQ(sizeof(double), starkit_utils::writeDouble(stream, writtenDouble));
  EXPECT_EQ(sizeof(double), starkit_utils::readDouble(stream, readDouble));
  EXPECT_EQ(readDouble, writtenDouble);
}

/*******************************************************
 * Test of writeIntArray and readIntArray
 */

TEST(write_read_int_array, testSuccess)
{
  std::stringstream stream;
  unsigned int arraySize = 5;
  int writtenIntegers[arraySize] = { 0, -1, -3, 54698, 78 };
  int readIntegers[arraySize] = {};

  try
  {
    EXPECT_EQ(sizeof(int) * arraySize, starkit_utils::writeIntArray(stream, writtenIntegers, arraySize));
    EXPECT_EQ(sizeof(int) * arraySize, starkit_utils::readIntArray(stream, readIntegers, arraySize));
  }
  catch (...)
  {  // error if we pass here
    EXPECT_TRUE(false);
  }

  for (unsigned int i = 0; i < arraySize; i++)
  {
    EXPECT_EQ(readIntegers[i], writtenIntegers[i]);
  }
}

/*******************************************************
 * Test of writeDoubleArray and readDoubleArray
 */

TEST(write_read_double_array, testSuccess)
{
  std::stringstream stream;
  unsigned int arraySize = 5;
  double writtenDoubles[arraySize] = { 0.99754, -1 * M_PI, M_PI, 54698.56, 78.0 };
  double readDoubles[arraySize] = {};

  try
  {
    EXPECT_EQ(sizeof(double) * arraySize, starkit_utils::writeDoubleArray(stream, writtenDoubles, arraySize));
    EXPECT_EQ(sizeof(double) * arraySize, starkit_utils::readDoubleArray(stream, readDoubles, arraySize));
  }
  catch (...)
  {  // error if we pass here
    EXPECT_TRUE(false);
  }

  for (unsigned int i = 0; i < arraySize; i++)
  {
    EXPECT_EQ(readDoubles[i], writtenDoubles[i]);
  }
}

/*******************************************************
 * Test of write and read (with template)
 */

TEST(write_read, testSuccessIntPos)
{
  std::stringstream stream;
  int writtenInteger = 0;
  int readInteger;
  EXPECT_EQ(sizeof(int), starkit_utils::write<int>(stream, writtenInteger));
  EXPECT_EQ(sizeof(int), starkit_utils::read<int>(stream, &readInteger));
  EXPECT_EQ(readInteger, writtenInteger);
}

TEST(write_read, testSuccessIntNeg)
{
  std::stringstream stream;
  int writtenInteger = -89520;
  int readInteger;
  EXPECT_EQ(sizeof(int), starkit_utils::write<int>(stream, writtenInteger));
  EXPECT_EQ(sizeof(int), starkit_utils::read<int>(stream, &readInteger));
  EXPECT_EQ(readInteger, writtenInteger);
}

TEST(write_read, testSuccessDoublePos)
{
  std::stringstream stream;
  double writtenDouble = M_PI;
  double readDouble;
  EXPECT_EQ(sizeof(double), starkit_utils::write<double>(stream, writtenDouble));
  EXPECT_EQ(sizeof(double), starkit_utils::read<double>(stream, &readDouble));
  EXPECT_EQ(readDouble, writtenDouble);
}

TEST(write_read, testSuccessDoubleNeg)
{
  std::stringstream stream;
  double writtenDouble = -1 * M_PI;
  double readDouble;
  EXPECT_EQ(sizeof(double), starkit_utils::write<double>(stream, writtenDouble));
  EXPECT_EQ(sizeof(double), starkit_utils::read<double>(stream, &readDouble));
  EXPECT_EQ(readDouble, writtenDouble);
}

/*******************************************************
 * Test of writeArray and readArray
 */

TEST(write_read_array, testSuccessInt)
{
  std::stringstream stream;
  unsigned int arraySize = 5;
  int writtenIntegers[arraySize] = { 0, -1, -3, 54698, 78 };
  int readIntegers[arraySize] = {};

  try
  {
    EXPECT_EQ(sizeof(int) * arraySize, starkit_utils::writeArray<int>(stream, arraySize, writtenIntegers));
    EXPECT_EQ(sizeof(int) * arraySize, starkit_utils::readArray<int>(stream, arraySize, readIntegers));
  }
  catch (...)
  {  // error if we pass here
    EXPECT_TRUE(false);
  }

  for (unsigned int i = 0; i < arraySize; i++)
  {
    EXPECT_EQ(readIntegers[i], writtenIntegers[i]);
  }
}

/*******************************************************
 * Test of writeDoubleArray and readDoubleArray
 */

TEST(write_read_array, testSuccessDouble)
{
  std::stringstream stream;
  unsigned int arraySize = 5;
  double writtenDoubles[arraySize] = { 0.99754, -1 * M_PI, M_PI, 54698.56, 78.0 };
  double readDoubles[arraySize] = {};

  try
  {
    EXPECT_EQ(sizeof(double) * arraySize, starkit_utils::writeArray<double>(stream, arraySize, writtenDoubles));
    EXPECT_EQ(sizeof(double) * arraySize, starkit_utils::readArray<double>(stream, arraySize, readDoubles));
  }
  catch (...)
  {  // error if we pass here
    EXPECT_TRUE(false);
  }

  for (unsigned int i = 0; i < arraySize; i++)
  {
    EXPECT_EQ(readDoubles[i], writtenDoubles[i]);
  }
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
