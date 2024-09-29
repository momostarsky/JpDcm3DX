cmake_minimum_required(VERSION 3.8...3.12 FATAL_ERROR)
project(JpDcm3DX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

find_package(VTK REQUIRED )

find_package(VTKm REQUIRED)
message("Found VTKm_ENABLE_RENDERING： ${VTKm_ENABLE_RENDERING}")

if ("ON"  STREQUAL  "${VTKm_ENABLE_RENDERING}")
  message(STATUS "VTK-m  built with rendering on.")
endif()


find_package(ITK 5.0  REQUIRED)
message(STATUS  "ITK_USE_FILE: ${ITK_USE_FILE}")
include(${ITK_USE_FILE})


find_package("Qt5" COMPONENTS Widgets)
if (NOT TARGET "Qt5::Widgets")
  message("Skipping example: ${Qt5_NOT_FOUND_MESSAGE}")
  return ()
endif ()


#
# Teem
#
set(Teem_DIR "/opt/teem/lib")
include(FindTeem)
find_package(Teem REQUIRED)
message(STATUS  "Teem_INCLUDE_DIRS:${Teem_INCLUDE_DIRS}")
message(STATUS  "Teem_USE_FILE:${Teem_USE_FILE}")
message(STATUS  "Teem_LIBRARIES:${Teem_LIBRARIES}")

#
#
#



add_subdirectory(vtkAddon)
set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/vtkAddon")
include_directories(${vtkAddon_INCLUDE_DIRS})


add_subdirectory(vtkTeem)
set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/vtkTeem")
include_directories(${vtkTeem_INCLUDE_DIRS})



# Set your files and resources here
set(SRCS QtVTKRenderWindowsApp.cxx)


# CMAKE_AUTOMOC in ON so the MocHEADERS will be automatically wrapped.
add_executable(JpDcm3DX  ${SRCS})

target_include_directories(JpDcm3DX PUBLIC ${ITK_INCLUDE_DIRS}   ${Teem_INCLUDE_DIRS})
message(STATUS "ITK Libs :${${PROJECT_NAME}_ITK_COMPONENTS}")
target_link_libraries(JpDcm3DX
  PRIVATE   "Qt5::Widgets"   ${VTK_LIBRARIES} ${VTKm_LIBRARIES}   ${ITK_LIBRARIES} ${Teem_LIBRARIES} vtkAddon vtkTeem)
vtk_module_autoinit(
  TARGETS JpDcm3DX
  MODULES ${VTK_LIBRARIES} )
