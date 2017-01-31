将FindOpenCV.cmake放在C:\Program Files (x86)\CMake\share\cmake-3.4\Modules目录下。

原来的FindOpenCV.cmake有bug，一是没有定义OpenCV_DIR，二是没有正确地配置OPENCV_LIB_COMPONENTS和CVLIB_SUFFIX，find_library也用得不对，导致找不到库文件。