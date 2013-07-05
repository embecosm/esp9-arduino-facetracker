

cmake_minimum_required(VERSION 2.8)
project( FaceDetect )
find_package( OpenCV REQUIRED )
add_executable( techbitarFaceDetection techbitarFaceDetection.cpp )
target_link_libraries( techbitarFaceDetection ${OpenCV_LIBS} )
