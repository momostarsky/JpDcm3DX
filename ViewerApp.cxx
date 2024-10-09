// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLStereoWidget.h>
#include <QVTKOpenGLNativeWidget.h>
#include <itkFactoryRegistration.h>
#include "BaseHeader.h"
#include "QtDcmViewer.h"
#include "vtkHelper/vtkHelper.h"

int main(int argc, char **argv) {

    itk::itkFactoryRegistration();
    vtkLogger::Init(argc, argv);
    // Put every log message in "everything.log":
    vtkLogger::LogToFile("everything.log", vtkLogger::APPEND, vtkLogger::VERBOSITY_MAX);
    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
//    vtkHelper::ReaderDicomImagesITKVTK("/home/dhz/v4486");
    // QT Stuff
    QApplication app(argc, argv);

    QtDcmViewer myQtVTKRenderWindows;
    myQtVTKRenderWindows.show();
    return QApplication::exec();
}

