cmake_minimum_required(VERSION 2.8)
project(starkit_utils)

find_package(catkin REQUIRED COMPONENTS
  jsoncpp
  eigen
)

find_package(Threads REQUIRED)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES starkit_utils
  CATKIN_DEPENDS jsoncpp eigen
)

include_directories(include ${catkin_INCLUDE_DIRS})

option(STARKIT_UTILS_BUILD_EXAMPLES "Building examples" OFF)

#Enable C++17
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")

#Enable Weird option linker
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

#List Sources sub source directories
set(SOURCES_DIRECTORIES
  src/starkit_utils
  src/starkit_utils/control
  src/starkit_utils/history
  src/starkit_utils/logging
  src/starkit_utils/nominal
  src/starkit_utils/serialization
  src/starkit_utils/sockets
  src/starkit_utils/spline
  src/starkit_utils/tables
  src/starkit_utils/threading
  src/starkit_utils/timing
  src/starkit_utils/stats
)

#Include Sources sub sources
foreach (DIRECTORY ${SOURCES_DIRECTORIES})
    message(STATUS "-- Sources: Adding ${DIRECTORY}/")
    include (${DIRECTORY}/Sources.cmake)
    set (PREFIXED_SOURCES)
    foreach (SOURCE ${SOURCES})
        set (PREFIXED_SOURCES ${PREFIXED_SOURCES} ${DIRECTORY}/${SOURCE})
    endforeach (SOURCE)
    set (ALL_SOURCES ${ALL_SOURCES} ${PREFIXED_SOURCES})
endforeach (DIRECTORY)


add_library (starkit_utils ${ALL_SOURCES})
target_link_libraries(starkit_utils ${catkin_LIBRARIES} Threads::Threads)

if (STARKIT_UTILS_BUILD_EXAMPLES)
  add_executable(json_serialization_example examples/json_serialization.cpp)
  target_link_libraries(json_serialization_example starkit_utils ${catkin_LIBRARIES} Threads::Threads)
  add_executable(json_factory_example examples/json_factory.cpp)
  target_link_libraries(json_factory_example starkit_utils ${catkin_LIBRARIES} Threads::Threads)
endif()

enable_testing()
 
set(TESTS
  angle
  control/bang_bang
  control/control
  control/pid
  control/variation_bound
  history/history
  logging/csv
  logging/logger
  logging/term_display
  # nominal/nominal not tested -> considered experimental
  serialization/stream_serializable
  serialization/json_serializable
  spline/function
  spline/poly_spline
  tables/double_table
  tables/string_table
  # threading/condition not tested -> adapter for condition API
  threading/multi_core
  # threading/mutex not tested -> adapter for mutex API
  # threading/thread not tested -> adapter for thread API
  timing/chrono
#  timing/benchmark # TO be fixed: private variables
  timing/time_stamp
  timing/elapse_tick
  timing/sleep
  util
)

if (CATKIN_ENABLE_TESTING)
  # create an executable for each test
  foreach(test ${TESTS})
    string(REPLACE "/" "_" id ${test})
    catkin_add_gtest(${id} tests/${test}.cpp)
    target_link_libraries(${id} starkit_utils)
  endforeach(test)
endif()

