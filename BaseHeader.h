//
// Created by dhz on 24-9-30.
//

#ifndef JPDCM3DX_BASEHEADER_H
#define JPDCM3DX_BASEHEADER_H

#include  <vtkLogger.h>
#include  <vtkAddonMathUtilities.h>
#include  <vtkTeemNRRDReader.h>
#include  <vtkITKArchetypeImageSeriesScalarReader.h>
#include  <vtkSegmentationConverter.h>
#include  <vtkDICOMReader.h>
#include  <vtkDICOMDirectory.h>
#include  <vtkDICOMItem.h>
#include  <vtkImageChangeInformation.h>
#include  <vtkImageData.h>
#include  <vtkImageMapToWindowLevelColors.h>
#include  <vtkInformation.h>
#include  <vtkStreamingDemandDrivenPipeline.h>
#include  <vtkImageActor.h>
#include  <vtkRenderWindow.h>
#include  <vtkRenderer.h>
#include  <vtkRenderWindowInteractor.h>
#include  <vtkInteractorStyleImage.h>
#include  <vtkGenericOpenGLRenderWindow.h>
#include  <vtkProperty.h>
#include  <vtkResliceCursorThickLineRepresentation.h>
#include  <vtkResliceImageViewer.h>
#include  <vtkImageData.h>
#include  <vtkBoundedPlanePointPlacer.h>
#include  <vtkCellPicker.h>
#include  <vtkCommand.h>
#include  <vtkDICOMImageReader.h>
#include  <vtkDistanceRepresentation.h>
#include  <vtkDistanceRepresentation2D.h>
#include  <vtkDistanceWidget.h>
#include  <vtkImageMapToWindowLevelColors.h>
#include  <vtkImageSlabReslice.h>
#include  <vtkInteractorStyleImage.h>
#include  <vtkLookupTable.h>
#include  <vtkPlane.h>
#include  <vtkPlaneSource.h>
#include  <vtkPointHandleRepresentation2D.h>
#include  <vtkProperty.h>
#include  <vtkRenderWindowInteractor.h>
#include  <vtkResliceCursor.h>
#include  <vtkResliceCursorActor.h>
#include  <vtkResliceCursorLineRepresentation.h>
#include  <vtkResliceCursorPolyDataAlgorithm.h>
#include  <vtkResliceCursorThickLineRepresentation.h>
#include  <vtkResliceCursorWidget.h>
#include  <vtkResliceImageViewer.h>
#include  <vtkResliceImageViewerMeasurements.h>
#include  <vtkGenericOpenGLRenderWindow.h>
#include  <vtkRenderWindow.h>
#include  <vtkRenderer.h>
#include  <vtkStringArray.h>
#include  <vtkDICOMImageReader.h>
#include  <vtkDICOMMetaDataAdapter.h>
#include  <vtkDICOMReader.h>
#include  <vtkDICOMDirectory.h>
#include  <vtkDICOMItem.h>
#include  <vtkDICOMMetaData.h>
#include  <vtkMatrix3x3.h>
#include  <vtkImageFlip.h>
#include  <vtkNamedColors.h>
#include  <vtkDistanceWidget.h>
#include  <vtkImagePlaneWidget.h>
#include  <vtkResliceImageViewer.h>
#include  <vtkResliceImageViewerMeasurements.h>
#include  <vtkSmartPointer.h>
#include  <QMainWindow>
#include  <vtkGenericOpenGLRenderWindow.h>
#include  <vtkResliceCursorRepresentation.h>
#include  <vtkResliceCursorLineRepresentation.h>
#include  <vtkCellPicker.h>
#include  <vtkResliceCursorThickLineRepresentation.h>
#include  <vtkTextActor.h>
#include  <vtkTextProperty.h>
#include  <vtkResliceImageViewer.h>
#include  <vtkResliceCursorRepresentation.h>
#include  <vtkImagePlaneWidget.h>
#include  <vtkDistanceWidget.h>
#include  <vtkSmartPointer.h>
#include  <vtkCellPicker.h>
#include  <vtkTextActor.h>
#include  <vtkCornerAnnotation.h>
#include  <vtkMedicalImageProperties.h>
#include  <vtkCamera.h>

#include  <itkImage.h>
#include  <itkGDCMImageIO.h>
#include  <itkGDCMSeriesFileNames.h>
#include  <itkImageToVTKImageFilter.h>
#include  <itkFlipImageFilter.h>
#include  <itkDCMTKImageIO.h>
#include  <itkImageSeriesReader.h>
#include  <itkImageFileWriter.h>
#include  <SimpleITK.h>
#include  <sitkImageOperators.h>
#include  <iostream>

using namespace std;
namespace sitk = itk::simple;

#endif //JPDCM3DX_BASEHEADER_H
