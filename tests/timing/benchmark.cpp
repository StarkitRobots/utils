#include <gtest/gtest.h>
#include "rhoban_utils/timing/time_stamp.h"
#include "rhoban_utils/timing/benchmark.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std::chrono;
namespace rhoban_utils{
const std::string  name="BenchmarkName";
Benchmark * benchmarkF;

//test benchmark default values
TEST(BenchmarkTest,testValues){
    benchmarkF = new Benchmark(NULL, name);

    EXPECT_EQ(benchmarkF->elapsedTicks,0);
    EXPECT_EQ(benchmarkF->nbIterations,0);
    EXPECT_EQ(benchmarkF->children.count(name),0);

    benchmarkF->endSession();
}

//test benchmark values when starting session
TEST(BenchmarkTest,testStartEndSession){
    //start session
    benchmarkF = new Benchmark(NULL, name);

    //end session
    benchmarkF->endSession();

    EXPECT_TRUE(benchmarkF->elapsedTicks>0);
    EXPECT_EQ(benchmarkF->nbIterations,1);

    int sess1=benchmarkF->elapsedTicks;
    benchmarkF->startSession();

    EXPECT_EQ(benchmarkF->nbIterations,1);

    benchmarkF->endSession();

    EXPECT_EQ(benchmarkF->nbIterations,2);
    EXPECT_GT(benchmarkF->elapsedTicks,sess1);
}

//test benchmark add children 
TEST(BenchmarkTest,testOpenSubOrBenchmark){
    benchmarkF = new Benchmark(NULL, name);

    EXPECT_EQ(benchmarkF->children.count("DefaultbechmarkName"),0);
    EXPECT_TRUE(benchmarkF->current==0);

    benchmarkF->open("DefaultbechmarkName");
    benchmarkF->children.insert(std::pair<std::string, Benchmark *>("DefaultbechmarkName",benchmarkF->current));
    
    EXPECT_TRUE(benchmarkF->current != 0);
    EXPECT_EQ(benchmarkF->children.count("DefaultbechmarkName"),1);
    EXPECT_TRUE(benchmarkF->children.at("DefaultbechmarkName")==benchmarkF->current);
    }

//should throw exception if benchmarkname dosen't exist
TEST(BenchmarkTest,testCloseSubOrBenchmark1){
    benchmarkF = new Benchmark(NULL, name);
    std::ofstream myfile;
    const char  *nameBenchmark="BenchmarkNotExist";
    
    try {
    benchmarkF->close(nameBenchmark,false,-1,myfile);
    } catch (...) {
        EXPECT_TRUE(true);
    }
}

//should throw exception if benchmarkname  != "DefaultbechmarkName"
TEST(BenchmarkTest,testCloseSubOrBenchmark2){
    benchmarkF = new Benchmark(NULL, name);
    benchmarkF->open("DefaultbechmarkName");
    std::ofstream OutputFile;
    const char  *nameBenchmark="BenchmarkNotExist";
   
    try {
    benchmarkF->close(nameBenchmark,false,-1,OutputFile);
    } catch (...) {
        EXPECT_TRUE(true);
    }
}

//should not throw exception && should not create file result path/file.txt if the
//second parameter is false
TEST(BenchmarkTest,testCloseSubOrBenchmark3){
    benchmarkF = new Benchmark(NULL, name);
    benchmarkF->open("TestbechmarkName");
    benchmarkF->current->name="TestbechmarkName";
    const char  *nameBenchmark="TestbechmarkName";
   
    std::ofstream OutputFile("path/file.txt");
    benchmarkF->close(nameBenchmark,false,-1,OutputFile);
    std::ifstream inputFile("path/file.txt");
    
    EXPECT_FALSE(inputFile);
}
//should not throw exception && should create file on result_benchmark repertory;
//the results should contain the Name of current benchmark and number of his iterations

TEST(BenchmarkTest,testCloseSubOrBenchmark4){
    benchmarkF = new Benchmark(NULL, name);
    benchmarkF->open("DefaultbechmarkName");
    benchmarkF->current->name="DefaultbechmarkName";
    const std::string testFilePath = "result_benchmark/"+benchmarkF->current->name+".txt";
    
    std::string filePath = __FILE__;
    std::string currentDirPath = filePath.substr(0, filePath.rfind("/"));
    std::string absoluteTestFilePath=currentDirPath + "/" + testFilePath;
    std::ofstream OutputFile(absoluteTestFilePath);
    const char  *nameBenchmark="DefaultbechmarkName";
    benchmarkF->close(nameBenchmark,true,-1,OutputFile);
    std::ifstream infile(absoluteTestFilePath);
   
    //file shouls exist
    EXPECT_TRUE(infile);
   
    std::string line;
    std::string lineFile="";
    while (infile >> line) {
    lineFile+=line+" ";
    }
   
    EXPECT_NE(lineFile.find("DefaultbechmarkName") , std::string::npos);
    EXPECT_NE(lineFile.find("1 iterations") , std::string::npos);
    //delete file
    remove(absoluteTestFilePath.c_str());
}

//should write the header of csv file : depth,name,father,time
TEST(BenchmarkTest,testPrintHeader){
    benchmarkF = new Benchmark(NULL, name);
    const std::string testFilePath = "result_benchmark/testPrintHeader.csv";
    
    std::string filePath = __FILE__;
    std::string currentDirPath = filePath.substr(0, filePath.rfind("/"));
    std::string absoluteTestFilePath=currentDirPath + "/" + testFilePath;
    std::ofstream OutputFile(absoluteTestFilePath);
    benchmarkF->printCSVHeader(OutputFile);
    std::ifstream infile (absoluteTestFilePath);
    
    //file should exist
    EXPECT_TRUE(infile);
    
    std::string line;
    std::vector<std::string> words;
    while (infile >> line) {
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token,',')) {
            words.push_back(token);
        }
    }
    
    EXPECT_EQ(words.at(0),"depth");
    EXPECT_EQ(words.at(1),"name");
    EXPECT_EQ(words.at(2),"father");
    EXPECT_EQ(words.at(3),"time");
}
//depth=1 , benchmark without father (defaultNAmeFather="unkonwn")
//should write information about benchmark without father (default name of father is unkown")
//result in result_benchmark/testPrintNoFatherB.csv file (without childrens)
TEST(BenchmarkTest,testPrintCsv1){
    benchmarkF = new Benchmark(NULL, name);
    const std::string testFilePath = "result_benchmark/testPrintNoFatherB.csv";
    
    std::string filePath = __FILE__;
    std::string currentDirPath = filePath.substr(0, filePath.rfind("/"));
    std::string absoluteTestFilePath=currentDirPath + "/" + testFilePath;
    std::ofstream OutputFile(absoluteTestFilePath);
    benchmarkF->printCSVHeader(OutputFile);
    benchmarkF->printCSV(OutputFile,1,2);
    std::ifstream infile (absoluteTestFilePath);
    
    //file should exist
    EXPECT_TRUE(infile);
    
    //1,BenchmarkName,unknown,0
    std::string line;
    std::vector<std::string> words;
    while (infile >> line) {
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token,',')) {
            words.push_back(token);
        }
    }
    
    EXPECT_EQ(words.at(5),"BenchmarkName");
    EXPECT_EQ(words.at(6),"unknown");
    EXPECT_EQ(words.at(7),"0");
}

//should write information about benchmark and his father
//result in result_benchmark/testPrintFatherB.csv file (without childrens)
TEST(BenchmarkTest,testPrintCsv2){
    benchmarkF = new Benchmark(NULL, name);
    const std::string  nameFather="BenchmarkFatherName";
    Benchmark * benchmarkFather=new Benchmark(NULL, nameFather);
    benchmarkF->father=benchmarkFather;
    
    const std::string testFilePath = "result_benchmark/testPrintFatherB.csv";
    std::string filePath = __FILE__;
    std::string currentDirPath = filePath.substr(0, filePath.rfind("/"));
    std::string absoluteTestFilePath=currentDirPath + "/" + testFilePath;
    std::ofstream OutputFile(absoluteTestFilePath);
    benchmarkF->printCSVHeader(OutputFile);
    benchmarkF->printCSV(OutputFile,1,2);
    std::ifstream infile (absoluteTestFilePath);
    
    //file should exist
    EXPECT_TRUE(infile);
    
    //1,BenchmarkName,unknown,0
    std::string line;
    std::vector<std::string> words;
    while (infile >> line) {
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token,',')) {
            words.push_back(token);
        }
    }
    
    EXPECT_EQ(words.at(5),"BenchmarkName");
    EXPECT_EQ(words.at(6),"BenchmarkFatherName");
    EXPECT_EQ(words.at(7),"0");
}

//should write information about benchmark and his father and his childrens(2children child1 && child2)
//result in result_benchmark/testPrintFatherB.csv file (without childrens)
TEST(BenchmarkTest,testPrintCsv3){
    benchmarkF = new Benchmark(NULL, name);
    const std::string  nameFather="BenchmarkFatherName";
    
    Benchmark * benchmarkFather=new Benchmark(NULL, nameFather);
    benchmarkF->father=benchmarkFather;
    Benchmark * benchmarkChild1=new Benchmark(NULL,"child1");
    Benchmark * benchmarkChild2=new Benchmark(NULL,"child2");
    benchmarkF->children.insert(std::pair<std::string, Benchmark *>(benchmarkChild1->name,benchmarkChild1));
    benchmarkF->children.insert(std::pair<std::string, Benchmark *>(benchmarkChild2->name,benchmarkChild2));
    
    const std::string testFilePath = "result_benchmark/testPrintFatherWithChild.csv";
    std::string filePath = __FILE__;
    std::string currentDirPath = filePath.substr(0, filePath.rfind("/"));
    std::string absoluteTestFilePath=currentDirPath + "/" + testFilePath;
    std::ofstream OutputFile(absoluteTestFilePath);
    benchmarkF->printCSVHeader(OutputFile);
    benchmarkF->printCSV(OutputFile,1,2);
    std::ifstream infile (absoluteTestFilePath);
    
    //file should exist
    EXPECT_TRUE(infile);
    
    //1,BenchmarkName,unknown,0
    std::string line;
    std::vector<std::string> words;
    while (infile >> line) {
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token,',')) {
            words.push_back(token);
        }
    }
    
    EXPECT_EQ(words.at(5),"BenchmarkName");
    EXPECT_EQ(words.at(6),"BenchmarkFatherName");
    EXPECT_EQ(words.at(7),"0");
    EXPECT_EQ(words.at(9),"child1");
    EXPECT_EQ(words.at(13),"child2");
    }

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
