file(GLOB mytest_source_files *.cpp)
add_executable(mytest ${mytest_source_files} ../FileUtility.cpp)
include_directories("..")

find_package( OpenCV REQUIRED ts)
target_link_libraries( mytest ${OpenCV_LIBS} )
