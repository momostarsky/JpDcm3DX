//
// Created by dhz on 24-10-9.
//

#ifndef JPDCM3DY_QTDCMVIEWER_H
#define JPDCM3DY_QTDCMVIEWER_H

#include <QMainWindow>
#include "../BaseHeader.h"
#include "DicomLoader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QtDcmViewer; }
QT_END_NAMESPACE

class QtDcmViewer : public QMainWindow {
Q_OBJECT

public:
    explicit QtDcmViewer();

    ~QtDcmViewer() override;
protected:
    void Init();
protected:
    vtkSmartPointer<vtkResliceImageViewer> mResliceViewer[3];
    vtkSmartPointer<vtkResliceCursorWidget> mResliceCursorWidget[3];
    vtkSmartPointer<vtkResliceCursorRepresentation> mRep[3];
    vtkSmartPointer<vtkRenderWindow> mResliceRenderWin[3];
    vtkSmartPointer<vtkDistanceWidget> DistanceWidget[3];

    vtkSmartPointer<vtkCellPicker> mPlanePicker;
    vtkSmartPointer<vtkImagePlaneWidget> mPlaneWidget[3];
    vtkSmartPointer<vtkRenderWindow> mPlaneRenderWin;
    vtkSmartPointer<vtkRenderer> mPlaneRender;
    vtkSmartPointer<vtkProperty> mPlaneProperty;

    DicomLoader *dicomLoader;


private:
    Ui::QtDcmViewer *ui;
};


#endif //JPDCM3DY_QTDCMVIEWER_H
