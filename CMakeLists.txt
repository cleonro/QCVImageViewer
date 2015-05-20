project(QCVImageViewer)
  
cmake_minimum_required(VERSION 2.8)

message("Configure QCVImageViewer with cmake " ${CMAKE_VERSION} "...")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(OpenGL REQUIRED)

if(OPENGL_FOUND AND OPENGL_GLU_FOUND)
	message("OpenGL was found.")
endif()

find_package(OpenCV REQUIRED)

message("Found package OpenCV " ${OpenCV_VERSION} ".")

##activate and test C++11 standard
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()
file(WRITE 
      ${CMAKE_BINARY_DIR}/CMakeTemp/testC++11.cpp
      "int main(){auto i = 0; return 0;}\n"
    )
try_compile(C++11_TEST ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/CMakeTemp/testC++11.cpp)
message("C++11_TEST " ${C++11_TEST})

include_directories(${OpenCV_INCLUDE_DIRS})

set(SRCS 
		main.cpp 
		mainwindow.h mainwindow.cpp
		viewport.h viewport.cpp
		imagecontrols.h imagecontrols.cpp
	)
set(HDRS )
set(FRMS 
	mainwindow.ui
	imagecontrols.ui
	)
qt5_wrap_ui(FRMS_SRCS ${FRMS})
qt5_wrap_cpp(MOC_SRCS ${HDRS})

source_group("Forms" FILES ${FRMS})
source_group("Headers" FILES ${HDRS})
source_group("MOC" FILES ${MOC_SRCS})

add_executable(QCVImageViewer ${SRCS} ${FRMS_SRCS} ${MOC_SRCS})
set_target_properties(QCVImageViewer PROPERTIES
				RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
qt5_use_modules(QCVImageViewer Widgets OpenGL)
target_link_libraries(QCVImageViewer ${OPENGL_LIBRARIES} ${OpenCV_LIBS})
