#include <gtest/gtest.h>
#include <stdlib.h>
#include "rhoban_utils/timing/chrono.h"
#include <sys/timeb.h>
#include <time.h>
#include <string>
using namespace rhoban_utils;

//test time_sec and time_usec of gettimeofday value
TEST(chrono,gettimeofday){
  t_chrono tv;
  tv.tv_sec=0;
  tv.tv_usec=0;

  int val=gettimeofday(&tv,NULL);
  EXPECT_EQ(val,0);
  //time sec should be > then usec
  EXPECT_GT(tv.tv_sec,tv.tv_usec); 
}

//test time_sec and time_usec of two gettimeofday values using sleep thread
TEST(chrono,gettimeofdayOfTwoValues){

  t_chrono tv1,tv2;
  int val1=gettimeofday(&tv1,NULL);
  int milisec = 400; 

  //use sleep thread 
  struct timespec req = {0,0};
  req.tv_sec = 0;
  req.tv_nsec = milisec * 1000000L;
  nanosleep(&req, (struct timespec *)NULL);
  int val2=gettimeofday(&tv2,NULL);

  EXPECT_EQ(val1,0);
  EXPECT_EQ(val2,0);
  //tv1 & tv2 should be > 0
  EXPECT_GT(tv1.tv_usec,0);
  EXPECT_GT(tv2.tv_usec,0);
}

//test reset time 
TEST(chrono,chronoReset){
  Chrono c;

  EXPECT_GT(c.getTime(),0);
  double cTime=c.getTime();
  c.reset();
  //current value "c.getTime()" should be < then recent value
  EXPECT_LT(c.getTime(),cTime);
}

//test usec,msec and sec time values 
TEST(chrono,chrono_sec_msec_usec){
    t_chrono tv1;
    tv1.tv_sec=0;
    tv1.tv_usec=0;

    //calculate sec & msec & usec values
     long int usecval=chrono_sec(&tv1);
     long int msecval=chrono_msec(&tv1);
     long int secval=chrono_usec(&tv1);
     long int valdiv1=msecval / 1000;
     long int valdiv2=secval / 1000;

     //chrono_msec(val) should be equal to chrono_usec(val)/1000
     //chrono_sec(val) should be equal to chrono_msec(val)/1000
     EXPECT_EQ(usecval,valdiv1);
     EXPECT_EQ(msecval,valdiv2);
     EXPECT_GT(secval,msecval);
     EXPECT_GT(msecval,usecval);
}

//test getTime 
TEST(chrono,getTime1){
  Chrono c;

  int milisec = 10; 
  struct timespec req = {0,0};
  req.tv_sec = 0;
  req.tv_nsec = milisec * 1000000L;
  nanosleep(&req, (struct timespec *)NULL);

  //getTimeMsec should be > then getTimeUsec
  EXPECT_LT(c.getTimeMsec(),c.getTimeUsec());
}

//test getTime of two values using sleep thread
TEST(chrono,TestTwovaluesGetTime){
  Chrono c1;
  int milisec = 50; 

  struct timespec req = {0,0};
  req.tv_sec = 0;
  req.tv_nsec = milisec * 1000000L;
  nanosleep(&req, (struct timespec *)NULL);
  Chrono c2;
  
  //c1.getTime() should be != then c2.getTime()
  EXPECT_NE(std::to_string(c1.getTime()),std::to_string(c2.getTime()));
}

int main(int argc, char **argv){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
