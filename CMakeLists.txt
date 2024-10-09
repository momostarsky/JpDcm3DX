cmake_minimum_required(VERSION 3.8...3.12 FATAL_ERROR)
project(JpDcm3DY)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})


find_package(DCMTK REQUIRED)
if (DCMTK_FOUND)
    message(STATUS "DCMTK found version ${DCMTK_VERSION}")
    message(STATUS "DCMTK_LIBRARIES：${DCMTK_LIBRARIES}")

else ()
    message(FATAL_ERROR "DCMTK not found")
endif ()


find_package(VTK REQUIRED)

find_package(VTKm REQUIRED)
message("Found VTKm_ENABLE_RENDERING： ${VTKm_ENABLE_RENDERING}")

if ("ON" STREQUAL "${VTKm_ENABLE_RENDERING}")
    message(STATUS "VTK-m  built with rendering on.")
endif ()


find_package(ITK 5.0 REQUIRED)
message(STATUS "ITK_USE_FILE: ${ITK_USE_FILE}")
include(${ITK_USE_FILE})


find_package("Qt5" COMPONENTS Widgets)
if (NOT TARGET "Qt5::Widgets")
    message("Skipping example: ${Qt5_NOT_FOUND_MESSAGE}")
    return()
endif ()


#
# Teem
#
set(Teem_DIR "/opt/teem/lib")
include(FindTeem)
find_package(Teem REQUIRED)
message(STATUS "Teem_INCLUDE_DIRS:${Teem_INCLUDE_DIRS}")
#message(STATUS  "Teem_USE_FILE:${Teem_USE_FILE}")
message(STATUS "Teem_LIBRARIES:${Teem_LIBRARIES}")


find_package(SimpleITK REQUIRED)

#
#
#


add_subdirectory(vtkAddon)
set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/vtkAddon")
include_directories(${vtkAddon_INCLUDE_DIRS})


add_subdirectory(vtkTeem)
set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/vtkTeem")
include_directories(${vtkTeem_INCLUDE_DIRS})


add_subdirectory(ITKFactoryRegistration)
set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/ITKFactoryRegistration")
include_directories(${ITKFactoryRegistration_INCLUDE_DIRS})

add_subdirectory(vtkITK)
set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/vtkITK")
include_directories(${vtkITK_INCLUDE_DIRS})

add_subdirectory(vtkSegmentationCore)
set(LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:${CMAKE_CURRENT_SOURCE_DIR}/vtkSegmentationCore")
include_directories(${vtkSegmentationCore_INCLUDE_DIRS})

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Set your files and resources here
set(SRCS ViewerApp.cxx
        BaseHeader.h
        vtkHelper/vtkHelper.cpp
        vtkHelper/vtkHelper.h
        QtDcmViewer.cpp
        QtDcmViewer.h
        QtDcmViewer.ui
        DicomLoader.cpp
        DicomLoader.h

)

set(MRMLCore_SRCS
        3DSlicer/Core/vtkCodedEntry.cxx
        3DSlicer/Core/vtkEventBroker.cxx
        3DSlicer/Core/vtkDataFileFormatHelper.cxx
        3DSlicer/Core/vtkMRMLI18N.cxx
        3DSlicer/Core/vtkMRMLI18N.h
        3DSlicer/Core/vtkMRMLMeasurement.cxx
        3DSlicer/Core/vtkMRMLStaticMeasurement.cxx
        3DSlicer/Core/vtkMRMLLogic.cxx
        3DSlicer/Core/vtkMRMLAbstractLayoutNode.cxx
        3DSlicer/Core/vtkMRMLAbstractViewNode.cxx
        3DSlicer/Core/vtkMRMLCameraNode.cxx
        3DSlicer/Core/vtkMRMLClipModelsNode.cxx
        3DSlicer/Core/vtkMRMLColorNode.cxx
        3DSlicer/Core/vtkMRMLColors.cxx
        3DSlicer/Core/vtkMRMLColorTableNode.cxx
        3DSlicer/Core/vtkMRMLColorTableStorageNode.cxx
        3DSlicer/Core/vtkMRMLCrosshairNode.cxx
        3DSlicer/Core/vtkMRMLDiffusionTensorDisplayPropertiesNode.cxx
        3DSlicer/Core/vtkMRMLDiffusionWeightedVolumeDisplayNode.cxx
        3DSlicer/Core/vtkMRMLDiffusionWeightedVolumeNode.cxx
        3DSlicer/Core/vtkMRMLDisplayNode.cxx
        3DSlicer/Core/vtkMRMLDisplayableNode.cxx
        3DSlicer/Core/vtkMRMLFolderDisplayNode.cxx
        3DSlicer/Core/vtkMRMLHierarchyNode.cxx
        3DSlicer/Core/vtkMRMLHierarchyStorageNode.cxx
        3DSlicer/Core/vtkMRMLDisplayableHierarchyNode.cxx
        3DSlicer/Core/vtkMRMLInteractionNode.cxx
        3DSlicer/Core/vtkMRMLLabelMapVolumeDisplayNode.cxx
        3DSlicer/Core/vtkMRMLLabelMapVolumeNode.cxx
        3DSlicer/Core/vtkMRMLLinearTransformNode.cxx
        3DSlicer/Core/vtkMRMLLinearTransformSequenceStorageNode.cxx
        3DSlicer/Core/vtkMRMLLinearTransformSequenceStorageNode.h
        3DSlicer/Core/vtkMRMLMarkupsStorageNode.cxx
        3DSlicer/Core/vtkMRMLMessageCollection.cxx
        3DSlicer/Core/vtkMRMLModelDisplayNode.cxx
        3DSlicer/Core/vtkMRMLModelHierarchyNode.cxx
        3DSlicer/Core/vtkMRMLModelNode.cxx
        3DSlicer/Core/vtkMRMLModelStorageNode.cxx
        3DSlicer/Core/vtkMRMLNode.cxx
        3DSlicer/Core/vtkMRMLParser.cxx
        3DSlicer/Core/vtkMRMLPlotChartNode.cxx
        3DSlicer/Core/vtkMRMLPlotSeriesNode.cxx
        3DSlicer/Core/vtkMRMLPlotViewNode.cxx
        3DSlicer/Core/vtkMRMLProceduralColorNode.cxx
        3DSlicer/Core/vtkMRMLProceduralColorStorageNode.cxx
        3DSlicer/Core/vtkMRMLROIListNode.cxx
        3DSlicer/Core/vtkMRMLROINode.cxx
        3DSlicer/Core/vtkMRMLScalarVolumeDisplayNode.cxx
        3DSlicer/Core/vtkMRMLScalarVolumeNode.cxx
        3DSlicer/Core/vtkMRMLScene.cxx
        3DSlicer/Core/vtkMRMLSceneViewNode.cxx
        3DSlicer/Core/vtkMRMLSceneViewStorageNode.cxx
        3DSlicer/Core/vtkMRMLScriptedModuleNode.cxx
        3DSlicer/Core/vtkMRMLScriptedModuleNode.h
        3DSlicer/Core/vtkMRMLSegmentationNode.cxx
        3DSlicer/Core/vtkMRMLSegmentationNode.h
        3DSlicer/Core/vtkMRMLSegmentationDisplayNode.cxx
        3DSlicer/Core/vtkMRMLSegmentationDisplayNode.h
        3DSlicer/Core/vtkMRMLSegmentationStorageNode.cxx
        3DSlicer/Core/vtkMRMLSegmentationStorageNode.h
        3DSlicer/Core/vtkMRMLSequenceNode.cxx
        3DSlicer/Core/vtkMRMLSequenceNode.h
        3DSlicer/Core/vtkMRMLSequenceStorageNode.cxx
        3DSlicer/Core/vtkMRMLSequenceStorageNode.h
        3DSlicer/Core/vtkMRMLSelectionNode.cxx
        3DSlicer/Core/vtkMRMLSliceCompositeNode.cxx
        3DSlicer/Core/vtkMRMLSliceDisplayNode.cxx
        3DSlicer/Core/vtkMRMLSliceNode.cxx
        3DSlicer/Core/vtkMRMLSnapshotClipNode.cxx
        3DSlicer/Core/vtkMRMLStorableNode.cxx
        3DSlicer/Core/vtkMRMLStorageNode.cxx
        3DSlicer/Core/vtkMRMLSubjectHierarchyConstants.h
        3DSlicer/Core/vtkMRMLSubjectHierarchyNode.cxx
        3DSlicer/Core/vtkMRMLSubjectHierarchyNode.h
        3DSlicer/Core/vtkMRMLSubjectHierarchyLegacyNode.cxx
        3DSlicer/Core/vtkMRMLSubjectHierarchyLegacyNode.h
        3DSlicer/Core/vtkMRMLTableNode.cxx
        3DSlicer/Core/vtkMRMLTableStorageNode.cxx
        3DSlicer/Core/vtkMRMLTableSQLiteStorageNode.cxx
        3DSlicer/Core/vtkMRMLTableViewNode.cxx
        3DSlicer/Core/vtkMRMLTextNode.cxx
        3DSlicer/Core/vtkMRMLTextStorageNode.cxx
        3DSlicer/Core/vtkMRMLTransformNode.cxx
        3DSlicer/Core/vtkMRMLTransformStorageNode.cxx
        3DSlicer/Core/vtkMRMLTransformDisplayNode.cxx
        3DSlicer/Core/vtkMRMLTransformableNode.cxx
        3DSlicer/Core/vtkMRMLTranslator.h
        3DSlicer/Core/vtkMRMLUnitNode.cxx
        3DSlicer/Core/vtkMRMLVectorVolumeDisplayNode.cxx
        3DSlicer/Core/vtkMRMLViewNode.cxx
        3DSlicer/Core/vtkMRMLVolumeArchetypeStorageNode.cxx
        3DSlicer/Core/vtkMRMLVolumeDisplayNode.cxx
        3DSlicer/Core/vtkMRMLGlyphableVolumeDisplayNode.cxx
        3DSlicer/Core/vtkMRMLGlyphableVolumeSliceDisplayNode.cxx
        3DSlicer/Core/vtkMRMLVolumeHeaderlessStorageNode.cxx
        3DSlicer/Core/vtkMRMLVolumeNode.cxx
        3DSlicer/Core/vtkMRMLVolumeSequenceStorageNode.cxx
        3DSlicer/Core/vtkMRMLVolumeSequenceStorageNode.h
        3DSlicer/Core/vtkObservation.cxx
        3DSlicer/Core/vtkObserverManager.cxx
        3DSlicer/Core/vtkMRMLLayoutNode.cxx

        3DSlicer/Core/vtkCacheManager.cxx
        3DSlicer/Core/vtkDataIOManager.cxx
        3DSlicer/Core/vtkDataTransfer.cxx
        3DSlicer/Core/vtkMRMLBSplineTransformNode.cxx
        3DSlicer/Core/vtkMRMLGridTransformNode.cxx
        3DSlicer/Core/vtkPermissionPrompter.cxx
        3DSlicer/Core/vtkURIHandler.cxx
        3DSlicer/Core/vtkTagTableCollection.cxx
        3DSlicer/Core/vtkTagTable.cxx
        3DSlicer/Core/vtkMRMLdGEMRICProceduralColorNode.cxx
        3DSlicer/Core/vtkMRMLPETProceduralColorNode.cxx
        # Note: when you add new node types, add
        # register code to the vtkMRMLScene constructor
)


set(MRMLCore_SRCS
        ${MRMLCore_SRCS}
        3DSlicer/Core/vtkMRMLDiffusionImageVolumeNode.cxx
        3DSlicer/Core/vtkMRMLDiffusionTensorVolumeDisplayNode.cxx
        3DSlicer/Core/vtkMRMLDiffusionTensorVolumeNode.cxx
        3DSlicer/Core/vtkMRMLDiffusionTensorVolumeSliceDisplayNode.cxx
        3DSlicer/Core/vtkMRMLNRRDStorageNode.cxx
        3DSlicer/Core/vtkMRMLTensorVolumeNode.cxx
        3DSlicer/Core/vtkMRMLVectorVolumeNode.cxx
        3DSlicer/Core/vtkMRMLStreamingVolumeNode.cxx
        3DSlicer/Core/vtkMRMLVolumeArchetypeStorageNode.cxx
)


set_source_files_properties(
        3DSlicer/Core/vtkMRMLCoreTestingUtilities.cxx
        WRAP_EXCLUDE
)

# Abstract/pure virtual classes

set_source_files_properties(
        3DSlicer/Core/vtkMRMLNode.cxx
        3DSlicer/Core/vtkMRMLVolumeNode.cxx
        3DSlicer/Core/vtkMRMLTransformNode.cxx
        3DSlicer/Core/vtkMRMLDisplayNode.cxx
        3DSlicer/Core/vtkMRMLDisplayableNode.cxx
        3DSlicer/Core/vtkMRMLVolumeDisplayNode.cxx
        ABSTRACT
)

string(REGEX REPLACE "\\.cxx" ".h" MRMLCore_SRCS_HEADERS "${MRMLCore_SRCS}")
source_group("Header Files" FILES ${MRMLCore_SRCS_HEADERS})


# CMAKE_AUTOMOC in ON so the MocHEADERS will be automatically wrapped.
add_executable(${PROJECT_NAME} ${SRCS} ${MRMLCore_SRCS})

target_include_directories(${PROJECT_NAME} PUBLIC ${DCMTK_INCLUDE_DIRS} ${ITK_INCLUDE_DIRS} ${Teem_INCLUDE_DIRS})
message(STATUS "ITK Libs :${${PROJECT_NAME}_ITK_COMPONENTS}")
target_link_libraries(${PROJECT_NAME}
        PRIVATE "Qt5::Widgets"
        #        /opt/dcmtk-3.6.6/lib/liboficonv.so
        ${VTK_LIBRARIES} ${VTKm_LIBRARIES} ${ITK_LIBRARIES} ${Teem_LIBRARIES} ${SimpleITK_LIBRARIES}
        vtkAddon vtkTeem ITKFactoryRegistration vtkITK vtkSegmentationCore)
vtk_module_autoinit(
        TARGETS ${PROJECT_NAME}
        MODULES ${VTK_LIBRARIES})
