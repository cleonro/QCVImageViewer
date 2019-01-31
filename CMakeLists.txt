project(QCVImageViewer)
  
cmake_minimum_required(VERSION 2.8)

message("Configure QCVImageViewer with cmake " ${CMAKE_VERSION} "...")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(VTK COMPONENTS
        ##vtkCommonCore

        ##vtkGUISupportQt
        vtkGUISupportQtOpenGL

        ##vtkInteractionStyle
        vtkInteractionImage
        ##vtkInteractionWidgets

        ##vtkRenderingCore
        ##vtkRenderingQt
        ##vtkRenderingOpenGL
        ##vtkRenderingFreeTypeOpenGL
        ##vtkRenderingVolume

        ####vtkRenderingVolumeOpenGL2

        vtkIOGeometry
        ##vtkIOLegacy
        ##vtkIOMINC

        ####vtkIOXML

        ##vtkFiltersModeling
        ##vtkParallelCore
        ##vtkFiltersParallel

        ##vtkImagingMath
        ##vtkImagingMorphological
        ##vtkImagingColor
        ##vtkImagingStatistics
REQUIRED)
include(${VTK_USE_FILE})
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
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()
file(WRITE 
      ${CMAKE_BINARY_DIR}/CMakeTemp/testC++11.cpp
      "int main(){auto i = 0; return 0;}\n"
    )
try_compile(C++11_TEST ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/CMakeTemp/testC++11.cpp)
message("C++11_TEST " ${C++11_TEST})

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

add_executable(QCVImageViewer ${SRCS} ${FRMS_SRCS} ${MOC_SRCS} ${UI_RESOURCES})
set_target_properties(QCVImageViewer PROPERTIES
				RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
qt5_use_modules(QCVImageViewer Widgets OpenGL Multimedia)
target_link_libraries(QCVImageViewer ${OPENGL_LIBRARIES} ${OpenCV_LIBS} ${VTK_LIBRARIES})
