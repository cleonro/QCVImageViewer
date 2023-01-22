cmake_minimum_required(VERSION 3.2)

project(QCVImageViewer LANGUAGES CXX)

message("Configure QCVImageViewer with cmake " ${CMAKE_VERSION} "...")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(VTK COMPONENTS        
        GUISupportQt
        RenderingQt
        InteractionImage
        IOGeometry
        InteractionStyle
REQUIRED)

include_directories(${VTK_INCLUDE_DIRS})

find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(OpenGL REQUIRED)

if(OPENGL_FOUND AND OPENGL_GLU_FOUND)
	message("OpenGL was found.")
endif()

find_package(OpenCV REQUIRED)

message("Found package OpenCV " ${OpenCV_VERSION} ".")

##activate and test C++11 standard
#if(UNIX)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#endif()
#file(WRITE
#      ${CMAKE_BINARY_DIR}/CMakeTemp/testC++11.cpp
#      "int main(){auto i = 0; return 0;}\n"
#    )
#try_compile(C++11_TEST ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/CMakeTemp/testC++11.cpp)
#message("C++11_TEST " ${C++11_TEST})

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

include_directories(${OpenCV_INCLUDE_DIRS})

set(SRCS 
		main.cpp 
		mainwindow.h mainwindow.cpp
                mainwindow2.h mainwindow2.cpp
		viewport.h viewport.cpp
		imagecontrols.h imagecontrols.cpp
                imagecontrols2.h imagecontrols2.cpp
                viewportbase.h viewportbase.cpp
                viewportopengl.h viewportopengl.cpp
                viewportsourcebase.h viewportsourcebase.cpp
                viewportsourcecamera.h viewportsourcecamera.cpp
                viewportcontroller.h viewportcontroller.cpp
                viewportvtk.h viewportvtk.cpp
                viewportsourcefile.h viewportsourcefile.cpp
                filterbase.h filterbase.cpp
                filterinfo.h filterinfo.cpp
                filterbc.h filterbc.cpp
                filterfr.h filterfr.cpp
                viewportsourcemovie.h viewportsourcemovie.cpp
                ffmpegtest.h ffmpegtest.cpp
	)
set(HDRS )
set(FRMS 
	mainwindow.ui
        mainwindow2.ui
	imagecontrols.ui
        imagecontrols2.ui
	)
set(RCS
        resource.qrc
        )
qt5_wrap_ui(FRMS_SRCS ${FRMS})
qt5_wrap_cpp(MOC_SRCS ${HDRS})
qt5_add_resources(UI_RESOURCES ${RCS})

source_group("Forms" FILES ${FRMS})
source_group("Headers" FILES ${HDRS})
source_group("MOC" FILES ${MOC_SRCS})

##message("Ffmpeg directory: " ${CMAKE_BINARY_DIR}/../../ffmpeg)

option(USE_CUSTOM_FFMPEG false "Decide if a custom build of ffmpeg is used.")
set(CUSTOM_FFMPEG_BUILD_DIR "" CACHE PATH "Custom ffmpeg build directory.")

if(${USE_CUSTOM_FFMPEG})
    include_directories(${CUSTOM_FFMPEG_BUILD_DIR}/include)
    link_directories(${CUSTOM_FFMPEG_BUILD_DIR}/build)
elseif(UNIX)
    include_directories(/opt/local/include)
    link_directories(/opt/local/lib)

    include_directories(/usr/include/ffmpeg)
endif()

find_library(FFMPEG_AVCODEC_LIBRARY NAMES avcodec libavcodec PATHS ${CUSTOM_FFMPEG_BUILD_DIR}/build)
message("FFMPEG_AVCODEC_LIBRARY: " ${FFMPEG_AVCODEC_LIBRARY})
find_library(FFMPEG_AVFORMAT_LIBRARY NAMES avformat libavformat PATHS ${CUSTOM_FFMPEG_BUILD_DIR}/build)
message("FFMPEG_AVFORMAT_LIBRARY: " ${FFMPEG_AVFORMAT_LIBRARY})
find_library(FFMPEG_AVUTIL_LIBRARY NAMES avutil libavutil PATHS ${CUSTOM_FFMPEG_BUILD_DIR}/build)
message("FFMPEG_AVUTIL_LIBRARY: " ${FFMPEG_AVUTIL_LIBRARY})
set(FFMPEG_LIBS ${FFMPEG_AVCODEC_LIBRARY} ${FFMPEG_AVFORMAT_LIBRARY} ${FFMPEG_AVUTIL_LIBRARY})

## TODO - test Windows build
##if(WIN32)
##    include_directories(${CMAKE_BINARY_DIR}/../../ffmpeg/ffmpeg_dev/include)
##    link_directories(${CMAKE_BINARY_DIR}/../../ffmpeg/ffmpeg_dev/lib)
##endif(WIN32)

add_executable(QCVImageViewer ${SRCS} ${FRMS_SRCS} ${MOC_SRCS} ${UI_RESOURCES})
set_target_properties(QCVImageViewer PROPERTIES
				RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(Qt5Libs Qt5::Widgets Qt5::OpenGL Qt5::Multimedia)

target_link_libraries(QCVImageViewer ${FFMPEG_LIBS})
target_link_libraries(QCVImageViewer ${Qt5Libs} ${OPENGL_LIBRARIES} ${OpenCV_LIBS} ${VTK_LIBRARIES})

