// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLStereoWidget.h>
#include <QVTKOpenGLNativeWidget.h>
#include <QVTKRenderWidget.h>
#include "vtkLogger.h"
#include <vtkAddonMathUtilities.h>
#include <vtkTeemNRRDReader.h>

int main(int argc, char **argv) {
    vtkLogger::Init(argc, argv);
    // Put every log message in "everything.log":
    vtkLogger::LogToFile("everything.log", vtkLogger::APPEND, vtkLogger::VERBOSITY_MAX);
    // needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKRenderWidget::defaultFormat());
    // QT Stuff
    QApplication app(argc, argv);
    vtkSmartPointer<vtkAddonMathUtilities> p= vtkSmartPointer<vtkAddonMathUtilities>::New();


    vtkLogF(INFO , "vtkAddonMathUtilities");
    p->PrintSelf(std::cout, vtkIndent(2));
    vtkLogF(INFO , "vtkTeemNRRDReader");
    vtkSmartPointer<vtkTeemNRRDReader> nddr= vtkSmartPointer<vtkTeemNRRDReader>::New();
    nddr->PrintSelf(std::cout ,vtkIndent(2));

    return QApplication::exec();
}
