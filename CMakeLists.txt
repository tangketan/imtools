PROJECT(plot_sample)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

message(STATUS "opencv dir: " $ENV{OPENCV_DIR})
FIND_PACKAGE( OpenCV REQUIRED )

# Declare the target (an executable)
ADD_EXECUTABLE(plot_sample main_plot.cpp pgm.cpp FileUtility.cpp)

TARGET_LINK_LIBRARIES(plot_sample ${OpenCV_LIBS})

#MESSAGE(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")

add_subdirectory(unittest)
add_subdirectory(learn_CvLevMarq)


