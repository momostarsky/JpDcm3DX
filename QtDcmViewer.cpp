//
// Created by dhz on 24-10-9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_QtDcmViewer.h" resolved

#include <QPushButton>
#include "QtDcmViewer.h"
#include "ui_QtDcmViewer.h"
#include "vtkHelper/vtkHelper.h"

QtDcmViewer::QtDcmViewer() : ui(new Ui::QtDcmViewer), dicomLoader(new DicomLoader()) {

    this->ui->setupUi(this);
    this->Init();
}

QtDcmViewer::~QtDcmViewer() {
    delete ui;
    delete dicomLoader;
}

void QtDcmViewer::Init() {

    for (int i = 0; i < 3; ++i) {
        mResliceViewer[i] = vtkSmartPointer<vtkResliceImageViewer>::New();
        mResliceRenderWin[i] = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        mResliceViewer[i]->SetRenderWindow(mResliceRenderWin[i]);
    }

    ui->view1->setRenderWindow(mResliceRenderWin[0]);
    mResliceViewer[0]->SetupInteractor(mResliceRenderWin[0]->GetInteractor());

    ui->view2->setRenderWindow(mResliceRenderWin[1]);
    mResliceViewer[1]->SetupInteractor(mResliceRenderWin[1]->GetInteractor());

    ui->view3->setRenderWindow(mResliceRenderWin[2]);
    mResliceViewer[2]->SetupInteractor(mResliceRenderWin[2]->GetInteractor());

    for (const auto &i: mResliceViewer) {
        // make them all share the same reslice cursor object.
        i->SetResliceCursor(mResliceViewer[0]->GetResliceCursor());
    }

    mPlanePicker = vtkSmartPointer<vtkCellPicker>::New();
    mPlanePicker->SetTolerance(0.005);

    mPlaneRenderWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->view4->setRenderWindow(mPlaneRenderWin);
    mPlaneRender = vtkSmartPointer<vtkRenderer>::New();
    ui->view4->renderWindow()->AddRenderer(mPlaneRender);
    double planeBkColor[3] = {0.5, 0.5, 0.5};
    mPlaneRender->SetBackground(planeBkColor);

    vtkRenderWindowInteractor *iren = ui->view4->interactor();
    mPlaneProperty = vtkSmartPointer<vtkProperty>::New();
    for (int i = 0; i < 3; ++i) {
        mPlaneWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        mPlaneWidget[i]->SetInteractor(iren);
        mPlaneWidget[i]->SetPicker(mPlanePicker);
        double color[3] = {0.0, 0.0, 0.0};
        color[i] = 1.0;
        mPlaneWidget[i]->GetPlaneProperty()->SetColor(color);

        color[0] /= 4.0;
        color[1] /= 4.0;
        color[2] /= 4.0;
        mResliceViewer[i]->GetRenderer()->SetBackground(color);

        mPlaneWidget[i]->SetTexturePlaneProperty(mPlaneProperty);
        mPlaneWidget[i]->SetDefaultRenderer(mPlaneRender);

        // Make them all share the same color map.
        mResliceViewer[i]->SetLookupTable(mResliceViewer[0]->GetLookupTable());
        mPlaneWidget[i]->GetColorMap()->SetLookupTable(mResliceViewer[0]->GetLookupTable());
        mPlaneWidget[i]->SetColorMap(mResliceViewer[i]->GetResliceCursorWidget()
                                             ->GetResliceCursorRepresentation()->GetColorMap());
    }


    {
        dicomLoader->LoadDicom("/home/dhz/v4486");
        auto imgDimens = dicomLoader->GetDimension();
        auto spacing = dicomLoader->GetSpacing();
        auto orign = dicomLoader->GetOrigin();
        vtkHelper::PrintArray(imgDimens, 3);
        vtkHelper::PrintArray(spacing, 3);
        vtkHelper::PrintArray(orign, 3);
        //---------------readDicomData
        for (int i = 0; i < 3; ++i) {
            auto rep = vtkResliceCursorLineRepresentation::SafeDownCast(
                    mResliceViewer[i]->GetResliceCursorWidget()->GetRepresentation());
            rep->GetResliceCursorActor()->
                    GetCursorAlgorithm()->SetReslicePlaneNormal(i);

            mResliceViewer[i]->SetInputData(dicomLoader->GetImageData());

            mResliceViewer[i]->SetSliceOrientation(i);
            mResliceViewer[i]->SetResliceModeToAxisAligned();
        }

        for (int i = 0; i < 3; ++i) {
            mPlaneWidget[i]->RestrictPlaneToVolumeOn();
            mPlaneWidget[i]->TextureInterpolateOff();
            mPlaneWidget[i]->SetResliceInterpolateToLinear();
            mPlaneWidget[i]->SetInputData(dicomLoader->GetImageData());
            mPlaneWidget[i]->SetPlaneOrientation(i);
            mPlaneWidget[i]->SetSliceIndex(imgDimens[i] / 2);
            mPlaneWidget[i]->DisplayTextOff();
            mPlaneWidget[i]->SetWindowLevel(dicomLoader->GetColorWindow(), dicomLoader->GetColorLevel());
            mPlaneWidget[i]->On();
            mPlaneWidget[i]->InteractionOn();
        }

        for (int i = 0; i < 3; ++i) {
            mResliceViewer[i]->GetRenderer()->ResetCamera();
            mResliceRenderWin[i]->Render();
        }
        mPlaneRender->ResetCamera();
        mPlaneRenderWin->Render();

    }
    this->ui->view1->show();
    this->ui->view2->show();
    this->ui->view3->show();


}
