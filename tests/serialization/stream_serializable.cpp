#include <gtest/gtest.h>
#include <rhoban_utils/serialization/stream_serializable.h>
#include "rhoban_utils/io_tools.h"
#include <string>
#include <fstream>   
#include <iostream>
#include <stdexcept>

using std::ios;
using namespace rhoban_utils;
using namespace std;
const std::string file_name = "saves.txt";
const std::string file_name_load = "load.txt";
void create_file()
{
    fstream fs;
    fs.open(file_name, ios::out);
    fs.close();
}

class stream_serializable_inheritance : public StreamSerializable
{
private : int data;
public:

int getClassID() const 
{
     return 0;
}
int writeInternal(std::ostream & out) const
{
  int bytes_written = 0;
  bytes_written += rhoban_utils::write<int>(out, data);
  return bytes_written;
}
int read(std::istream & in)
{
(void) in;
this->setData(30);
return 13;
}
void setData(int d){data = d;}
int getData(){return data;}
};

stream_serializable_inheritance stream;
stream_serializable_inheritance stream_load;

TEST(stream_serializable, getClassID){
	EXPECT_EQ(0,stream.getClassID());
}

TEST(stream_serializable, writeInternal){
	stream.setData(5);
	int result = stream.writeInternal(cout);
	EXPECT_EQ(4,result);
}

TEST(stream_serializable, write){
	stream.setData(10);
	int result = stream.write(cout);
	EXPECT_EQ(8,result);
}
TEST(stream_serializable, save){
	create_file();
	EXPECT_EQ(4,stream.save(file_name,false));
	EXPECT_EQ(8,stream.save(file_name,true));
}

TEST(stream_serializable, load_catch_nofile){
        remove("saves.txt");
	EXPECT_THROW(stream.load(file_name),std::runtime_error);
	
}
TEST(stream_serializable, load){
	EXPECT_EQ(10,stream.getData());
        stream.save(file_name_load,true);
        stream_load.load(file_name_load);
	EXPECT_EQ(30,stream_load.getData());
	
}
TEST(stream_serializable, read){
	create_file();
	std::ifstream in(file_name, std::ios::binary);
	  if (!in) {
	    std::ostringstream oss;
	    oss << "Failed to open '" << file_name << "' for binary writing";
	    throw std::runtime_error(oss.str());
	  }

	EXPECT_EQ(13,stream.read(in));	
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
