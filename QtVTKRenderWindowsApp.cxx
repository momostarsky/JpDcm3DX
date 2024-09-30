// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLStereoWidget.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QVTKRenderWidget.h>
#include "BaseHeader.h"
#include "vtkHelper/vtkHelper.h"


int main(int argc, char **argv) {
    vtkLogger::Init(argc, argv);
    // Put every log message in "everything.log":
    vtkLogger::LogToFile("everything.log", vtkLogger::APPEND, vtkLogger::VERBOSITY_MAX);
    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKRenderWidget::defaultFormat());
    // QT Stuff
    QApplication app(argc, argv);

    vtkNew<vtkDICOMDirectory> dicomdir;
    dicomdir->SetDirectoryName("/home/dhz/v4486");
    dicomdir->RequirePixelDataOff();
    dicomdir->Update();

    int n = dicomdir->GetNumberOfSeries();
    if (n == 0) {
        std::cerr << "No DICOM images in directory!" << std::endl;
        return EXIT_FAILURE;
    }

    int firstStudy = 0;
    // Get information related to the patient study
    vtkDICOMItem patient = dicomdir->GetPatientRecordForStudy(firstStudy);
    vtkDICOMItem study = dicomdir->GetStudyRecord(firstStudy);


    // Iterate through all the series in this study.
    int j1 = dicomdir->GetFirstSeriesForStudy(firstStudy);
    //  int j2 = dicomdir->GetLastSeriesForStudy(i);
    // get some of the series attributes as a vtkDICOMItem
    vtkDICOMItem series = dicomdir->GetSeriesRecord(j1);
    // get all the files in the series
    vtkStringArray *sortedFiles = dicomdir->GetFileNamesForSeries(j1);


    auto volumeReader = vtkSmartPointer<vtkITKArchetypeImageSeriesScalarReader>::New();
    volumeReader->SetArchetype(sortedFiles->GetValue(0).c_str());
    for (int i = 0; i < sortedFiles->GetNumberOfValues(); i++) {
        volumeReader->AddFileName(sortedFiles->GetValue(i).c_str());
    }



    volumeReader->SetSingleFile(0);
    volumeReader->SetOutputScalarTypeToNative();
    volumeReader->SetDesiredCoordinateOrientationToNative();
    volumeReader->SetUseNativeOriginOn();
    volumeReader->Update();
    vtkHelper::PrintMatrix4X4(volumeReader->GetRasToIjkMatrix(), "VolumeReader GetRasToIjkMatrix");

    auto imageChangeInformation = vtkSmartPointer<vtkImageChangeInformation>::New();
    imageChangeInformation->SetInputConnection(volumeReader->GetOutputPort());
    imageChangeInformation->SetOutputSpacing(1, 1, 1);
    imageChangeInformation->SetOutputOrigin(0, 0, 0);
    imageChangeInformation->Update();


    auto imageData = imageChangeInformation->GetOutputDataObject(0);
    if (!imageData->IsA("vtkImageData")) {
        vtkLogF(INFO, "ImageData is vtkImageData");
        return EXIT_FAILURE;
    }
    auto pImg = (vtkImageData *) imageData;


    int imageDims[3];
    double origin[3] = {0.0};
    double spacing[3] = {1.0};
    int extentInfo[6] = {0};
    pImg->GetDimensions(imageDims);
    pImg->GetOrigin(origin);
    pImg->GetSpacing(spacing);
    pImg->GetExtent(extentInfo);

    vtkLogF(INFO, "Dimensions: %d,%d,%d", imageDims[0], imageDims[1], imageDims[2]);
    vtkLogF(INFO, "    Origin: %f,%f,%f", origin[0], origin[1], origin[2]);
    vtkLogF(INFO, "   Spacing: %f,%f, %f", spacing[0], spacing[1], spacing[2]);
    vtkLogF(INFO, "    Extent: %d,%d,%d,%d,%d,%d", extentInfo[0], extentInfo[1], extentInfo[2],
            extentInfo[3], extentInfo[4], extentInfo[5]);
    return QApplication::exec();
}

