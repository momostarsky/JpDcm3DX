//
// Created by dhz on 24-10-9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_QtDcmViewer.h" resolved

#include <QPushButton>
#include <vtkDICOMToRAS.h>
#include "QtDcmViewer.h"
#include "ui_QtDcmViewer.h"
#include "vtkHelper/vtkHelper.h"

QtDcmViewer::QtDcmViewer() : ui(new Ui::QtDcmViewer), dicomLoader(new DicomLoader()) {

    this->ui->setupUi(this);
    this->Init();

    auto _ = connect(ui->btnLoadDicom, &QPushButton::clicked, this, &QtDcmViewer::onLoadDicom);


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
        callback[i] = vtkSmartPointer<vtkSliceCallback>::New();
        mFlipFilter[i] = vtkSmartPointer<vtkImageFlip>::New();
    }
    mPlanePicker = vtkSmartPointer<vtkCellPicker>::New();
    mPlanePicker->SetTolerance(0.005);

    mPlaneRenderWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    mPlaneRender = vtkSmartPointer<vtkRenderer>::New();
    double planeBkColor[3] = {0.5, 0.5, 0.5};
    mPlaneRender->SetBackground(planeBkColor);
    mPlaneProperty = vtkSmartPointer<vtkProperty>::New();
    for (int i = 0; i < 3; ++i) {
        mPlaneWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();

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


    }
    for (auto &i: cornerActor) {
        i = vtkSmartPointer<vtkCornerAnnotation>::New();
        // i->SetDisplayPosition(5, 5);
        i->GetTextProperty()->SetColor(1.0, 0.72, 0.0);

    }
    cornerActor[vtkHelper::SLICE_ORIENTATION_YZ_Sagittal]->SetText(vtkCornerAnnotation::LowerLeft, u8"SLICE_ORIENTATION_YZ_Sagittal");
    cornerActor[vtkHelper::SLICE_ORIENTATION_XZ_Coronal]->SetText(vtkCornerAnnotation::LowerLeft, u8"SLICE_ORIENTATION_XZ_Coronal");
    cornerActor[vtkHelper::SLICE_ORIENTATION_XY_Axial]->SetText(vtkCornerAnnotation::LowerLeft, u8"SLICE_ORIENTATION_XY_Axial");
    cornerActor[vtkHelper::ThreeD]->SetText(vtkCornerAnnotation::LowerLeft, u8"3D");

    for (auto &i: slicerActor) {
        i = vtkSmartPointer<vtkCornerAnnotation>::New();
        // i->SetDisplayPosition(5, 5);
        i->GetTextProperty()->SetColor(1.0, 0.72, 0.0);

    }

    //////////////////////////////
//    for (auto &i: textActor) {
//        i = vtkSmartPointer<vtkTextActor>::New();
//       // i->SetDisplayPosition(5, 5);
//        i->GetTextProperty()->SetFontSize(14);
//        i->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
//        i->GetTextProperty()->SetFontFile(QString("./Fonts/simhei.ttf").toUtf8());
//        i->GetTextProperty()->SetJustificationToLeft(); // 左对齐
//        i->GetTextProperty()->SetFontFamilyToTimes(); // 左对齐
//        i->GetTextProperty()->SetVerticalJustificationToBottom(); // 底部对齐
//        i->GetPositionCoordinate()->SetValue(0.05, 0.05, 0); // 设置坐标值，使其位于左下角
//    }
//    textActor[0]->SetInput(u8"Sagittal-矢状位");
//    textActor[0]->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
//    textActor[1]->SetInput(u8"SLICE_ORIENTATION_XZ_Coronal-冠状位");
//    textActor[1]->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
//    textActor[2]->SetInput(u8"SLICE_ORIENTATION_XY_Axial-轴状位（横截面）");
//    textActor[2]->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
//    textActor[3]->SetInput(u8"3D");
//    textActor[3]->GetTextProperty()->SetColor(1.0, 1.0, 1.0);


//    for (auto &i: peopleInforTextActor) {
//        i = vtkSmartPointer<vtkTextActor>::New();
//        i->GetTextProperty()->SetFontSize(14);
//        i->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
//        i->GetTextProperty()->SetFontFile(QString("./Fonts/simhei.ttf").toUtf8());
////        i->SetInput(reader->GetPatientName());
//    }
//    peopleInforTextActor[0]->GetTextProperty()->SetColor(1, 1, 0);
//    peopleInforTextActor[0]->SetDisplayPosition(5, ui->view1->height() - 20);
//
//    peopleInforTextActor[1]->GetTextProperty()->SetColor(1, 1, 0);
//    peopleInforTextActor[1]->SetDisplayPosition(5, ui->view2->height() - 20);
//
//    peopleInforTextActor[2]->GetTextProperty()->SetColor(1, 1, 0);
//    peopleInforTextActor[2]->SetDisplayPosition(5, ui->view3->height() - 20);
//
//    peopleInforTextActor[3]->GetTextProperty()->SetColor(1, 1, 0);
//    peopleInforTextActor[3]->SetDisplayPosition(5, ui->view4->height() - 20);


}

void QtDcmViewer::onLoadDicom() {
    ui->view1->setRenderWindow(mResliceRenderWin[0]);
    mResliceViewer[0]->SetupInteractor(mResliceRenderWin[0]->GetInteractor());

    ui->view2->setRenderWindow(mResliceRenderWin[1]);
    mResliceViewer[1]->SetupInteractor(mResliceRenderWin[1]->GetInteractor());

    ui->view3->setRenderWindow(mResliceRenderWin[2]);
    mResliceViewer[2]->SetupInteractor(mResliceRenderWin[2]->GetInteractor());

    ui->view4->setRenderWindow(mPlaneRenderWin);

    ui->view4->renderWindow()->AddRenderer(mPlaneRender);


    for (const auto &i: mResliceViewer) {
        // make them all share the same reslice cursor object.
        i->SetResliceCursor(mResliceViewer[0]->GetResliceCursor());
    }

    vtkRenderWindowInteractor *iren = ui->view4->interactor();
    mPlaneProperty = vtkSmartPointer<vtkProperty>::New();
    for (const auto &i: mPlaneWidget) {

        i->SetInteractor(iren);

    }


    {
        dicomLoader->LoadDicomWithITK("/home/dhz/v4486");

//        for (int i = 0; i < 3; i++) {
////            if (i == vtkHelper::SLICE_ORIENTATION_XY_Axial) {
////                mFlipFilter[i]->SetFilteredAxes(1);
////            } else {
////                mFlipFilter[i]->SetFilteredAxes(2);
////            }
//            mFlipFilter[i]->SetInputData(dicomLoader->GetImageData());
//            mFlipFilter[i]->Update();
//        }

        auto imgDimens = dicomLoader->GetDimension();
        auto spacing = dicomLoader->GetSpacing();
        auto orign = dicomLoader->GetOrigin();
        vtkHelper::PrintArray3(imgDimens, "Dimension");
        vtkHelper::PrintArray3(spacing, "Spacing");
        vtkHelper::PrintArray3(orign, "Origin");
        auto imageData = dicomLoader->GetImageData();
        //---------------readDicomData
        for (int i = 0; i < 3; ++i) {
            auto rep = vtkResliceCursorLineRepresentation::SafeDownCast(
                    mResliceViewer[i]->GetResliceCursorWidget()->GetRepresentation());
            rep->GetResliceCursorActor()->
                    GetCursorAlgorithm()->SetReslicePlaneNormal(i);

            mResliceViewer[i]->GetRenderer()->AddViewProp(cornerActor[i]);
            mResliceViewer[i]->GetRenderer()->AddViewProp(slicerActor[i]);
            mResliceViewer[i]->SetInputData( imageData );
            mResliceViewer[i]->SetSliceOrientation(i);
            mResliceViewer[i]->SetResliceModeToAxisAligned();
            mResliceViewer[i]->GetRenderer()->ResetCamera();
            vtkCamera* cam = mResliceViewer[i]->GetRenderer()->GetActiveCamera()  ;
            if(cam)
            {
                switch ( mResliceViewer[i]->GetSliceOrientation()) {
                    case  vtkHelper::SLICE_ORIENTATION_XY_Axial:
                        cam->SetFocalPoint(0,0,0);
                        cam->SetPosition(0,0,1);
                        cam->SetViewUp(0,1,0);
                        break;
                    case vtkHelper::SLICE_ORIENTATION_XZ_Coronal:
                        cam->SetFocalPoint(0,0,0);
                        cam->SetPosition(0,1,0);
                        cam->SetViewUp(0,0,-1);
                        break;
                    case vtkHelper::SLICE_ORIENTATION_YZ_Sagittal:
                        cam->SetFocalPoint(0,0,0);
                        cam->SetPosition(1,0,0);
                        cam->SetViewUp(0,0,-1);
                        break;
                }
            }

        }

        for (int i = 0; i < 3; ++i) {

//            if(i ==  vtkHelper::SLICE_ORIENTATION_XY_Axial){
//                mFlipFilter[i]->SetFilteredAxes(1);
//            } else   {
//                mFlipFilter[i]->SetFilteredAxes(2);
//            }


            mPlaneWidget[i]->RestrictPlaneToVolumeOn();
            mPlaneWidget[i]->TextureInterpolateOff();
            mPlaneWidget[i]->SetResliceInterpolateToLinear();
            mPlaneWidget[i]->SetInputData( imageData );
            mPlaneWidget[i]->SetPlaneOrientation(i);
            mPlaneWidget[i]->SetSliceIndex(imgDimens[i] / 2);
            mPlaneWidget[i]->DisplayTextOff();
            mPlaneWidget[i]->SetWindowLevel(1500, 300);
            mPlaneWidget[i]->On();
            mPlaneWidget[i]->InteractionOn();
        }

        for (int i = 0; i < 3; ++i) {
            // Make them all share the same color map.
            mResliceViewer[i]->SetLookupTable(mResliceViewer[0]->GetLookupTable());
            mPlaneWidget[i]->GetColorMap()->SetLookupTable(mResliceViewer[0]->GetLookupTable());
            mPlaneWidget[i]->SetColorMap(mResliceViewer[i]->GetResliceCursorWidget()
                                                 ->GetResliceCursorRepresentation()->GetColorMap());

            mResliceViewer[i]->GetRenderer()->ResetCamera();


            mResliceViewer[i]->SetSlice((imgDimens[i] / 2) + 1);
            auto index = std::to_string(mResliceViewer[i]->GetSlice());
            slicerActor[i]->SetText(vtkCornerAnnotation::LowerRight, index.c_str());

            callback[i]->SetAnnotation(slicerActor[i]);
            mResliceViewer[i]->AddObserver(vtkResliceImageViewer::SliceChangedEvent, callback[i]);
            mResliceViewer[i]->GetRenderer()->ResetCamera();


            mResliceRenderWin[i]->Render();
        }


        mPlaneRender->AddViewProp(cornerActor[3]);
        mPlaneRender->ResetCamera();

        mPlaneRenderWin->Render();

    }


    this->ui->view1->show();
    this->ui->view2->show();
    this->ui->view3->show();
}
