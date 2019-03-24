#include <gtest/gtest.h>
#include <rhoban_utils/logging/logger.h>

using namespace rhoban_utils;

LoggerLevel level = LoggerDebug;
Logger logger("logger", level);

/// This class is responsible of printing content to the terminal with some
/// colors, ex: red for error, green for succes.
TEST(logger, check_color)
{
  /// Make sure that the object is constructed correctly according
  /// to the compiling environment
#ifdef WIN32
  EXPECT_EQ(true, logger.colors);
#else
  EXPECT_EQ(isatty(STDERR_FILENO), logger.colors);
#endif
}

TEST(logger, print_colors)
{
  /// This method prints to stderr the first given parametre when the object is
  /// constructed, in ower case it's gonna print "logger"
  logger.prefix();
  /// This method based on a given level it prints to stderr a ANSI color, ex
  /// because the level is LoggerDebug the color must be blue
  logger.colorize(level);
  /// This method prints also to stderr the value "\x1b[0m" wich is The reset of
  /// the attributes, so the color will be back to normal
  logger.resetColor();
  logger.error("This Should be Red");
  logger.warning("This Should be Orange");
  logger.log("This should be White");
  logger.success("This Should be Green");
  logger.debug("This Should be blue");
  logger.colors = false;
  /// Because colors is false this logger.debug must print a white content not a blue
  logger.debug("This Should be white even it's blue");
  /// This assert is to make sur that the colors variable is set to false
  EXPECT_EQ(false, logger.colors);
}

/// We didnt Test the getTime methode because it's just a #ifdef WIN32

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
