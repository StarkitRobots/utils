#include <gtest/gtest.h>
#include <rhoban_utils/logging/term_display.h>
#include <string>

using namespace std;

/// This class is responsible of a bunch of printing related to colors

TEST(term_display, term_display_print_all)
{
  char str[80];
  // This set color variable to true, this variable is for to check if we can
  // print color on terminal or not
  term_enable_colors();
  term_set_color("\n \t This Should be followed by \"m\" :  \n", NULL);
  // This will print a sperator
  term_separator();
  term_set_color("\n \t This Should be followed by \"m\"  : \n ", str);
  cout << str;
  // This will print a sperator "----------------------------"
  term_separator();
  char print_n_time = 'P';
  // This should print 20 'P'
  print_n_times(20, print_n_time);
  // This should print  "-- This is a subtitle --"
  char sub_title[30] = "\t \nThis is a subtitle";
  term_subtitle(sub_title);
  // This should print "- This is a title -"
  char title[20] = "\t \nThis is a title";
  term_title(title);
}

// We didnt test ClearScreen because it's related to Windows

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
