//
// Created by dhz on 24-10-9.
//

#include <itkVTKImageToImageFilter.h>
#include <vtkDICOMToRAS.h>
#include "DicomLoader.h"
#include "../vtkHelper/vtkHelper.h"

DicomLoader::~DicomLoader() = default;

void DicomLoader::LoadDicomWithITK(const char *path) {
    std::cout << "Load DICOM With DICOMImageReader" << std::endl;

    vtkSmartPointer<vtkDICOMDirectory> dicomdir = vtkSmartPointer<vtkDICOMDirectory>::New();
    dicomdir->SetDirectoryName(path);
    dicomdir->SetScanDepth(3);     //检索文件夹深度
    dicomdir->IgnoreDicomdirOn();  //忽略索引文件
    dicomdir->RequirePixelDataOn();//忽略没有像素数据的文件
    dicomdir->ShowHiddenOff();     //忽略隐藏文件
    dicomdir->Update();
    int n = dicomdir->GetNumberOfSeries();
    if (n == 0) {
        std::cerr << "No DICOM images in directory!" << std::endl;
        return;
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
    vtkIdType nf = sortedFiles->GetNumberOfValues();

    double mSpacing[3]{0};
    {
        vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
        reader->SetDataByteOrderToLittleEndian();

        reader->SetFileName(sortedFiles->GetValue(nf - 1).c_str());
        reader->Update();
        auto mImagePositionPatient = reader->GetImagePositionPatient();
        auto mImageOrientationPatient = reader->GetImageOrientationPatient();
        auto xmSpacing = reader->GetPixelSpacing();
        for (int i = 0; i < 3; i++) {
            mSpacing[i] = xmSpacing[i];
        }
        vtkHelper::PrintArray3(mSpacing, "GetPixelSpacing");


        for (int i = 0; i < 3; i++) {
            ImagePositionPatient[i] = mImagePositionPatient[i];
            Spacing[i] = mSpacing[i];
        }
        for (int i = 0; i < 6; i++) {
            ImageOrientationPatient[i] = mImageOrientationPatient[i];
        }
        vtkHelper::PrintArray3(ImagePositionPatient, "imagePaiteintOritaint");
        vtkHelper::PrintArray6(ImageOrientationPatient, "imageOrientationPatient");

        RescaleSlope = reader->GetRescaleSlope();
        RescaleOffset = reader->GetRescaleOffset();
        vtkHelper::Print2Numbs(RescaleSlope, RescaleOffset, "Rescale");
        float *xdir = &ImageOrientationPatient[0];
        float *ydir = &ImageOrientationPatient[3];
        float zdir[3];
        vtkMath::Cross(xdir, ydir, zdir);
        UserMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
        for (int i = 0; i < 3; i++) {
            UserMatrix->Element[i][0] = xdir[i];
            UserMatrix->Element[i][1] = ydir[i];
            UserMatrix->Element[i][2] = zdir[i];
            UserMatrix->Element[i][3] = ImagePositionPatient[i];
        }
        vtkHelper::PrintMatrix4X4(UserMatrix, "UserMatrix");
    }

    auto reader = vtkSmartPointer<vtkITKArchetypeImageSeriesScalarReader>::New();
    reader->SetDICOMImageIOApproachToGDCM();
    reader->SetArchetype(sortedFiles->GetValue(nf - 1).c_str());
    for(int i = 0; i < nf; i++) {
        reader->AddFileName(sortedFiles->GetValue(i).c_str());
    }
    reader->SetSingleFile(0);
    reader->SetOutputScalarTypeToNative();
    reader->SetDesiredCoordinateOrientationToNative();
    reader->SetUseNativeOriginOn();
    reader->Update();
    reader->PrintSelf(std::cout , vtkIndent(2));

    vtkSmartPointer<vtkImageFlip> flipFilter = vtkSmartPointer<vtkImageFlip>::New();
    flipFilter->SetInputData(reader->GetOutput());
    flipFilter->SetFilteredAxes(1);  // 假设我们只需要在一个轴上翻转
    flipFilter->Update();



    m_imageData = vtkSmartPointer<vtkImageData>::New();
    m_imageData->DeepCopy(flipFilter->GetOutput());
    m_imageData->SetSpacing(mSpacing);
    m_imageData->SetOrigin(ImagePositionPatient[0], ImagePositionPatient[1],  ImagePositionPatient[2]);
    m_imageData->GetDimensions(this->Dimension);
    m_imageData->GetSpacing(this->Spacing);
    m_imageData->GetExtent(this->Extent);
    m_imageData->GetOrigin(this->Origin);
}

DicomLoader::DicomLoader() : m_imageData(nullptr) {

}

vtkSmartPointer<vtkImageData> DicomLoader::GetImageData() const {
    return m_imageData;
}

double DicomLoader::GetColorLevel() const {
    return WindowCenter;
}

double DicomLoader::GetColorWindow() const {
    return WindowWidth;
}

void DicomLoader::LoadDicom(const char *path) {
    std::cout << "Load DICOM With DICOMImageReader" << std::endl;

    vtkSmartPointer<vtkDICOMDirectory> dicomdir = vtkSmartPointer<vtkDICOMDirectory>::New();
    dicomdir->SetDirectoryName(path);
    dicomdir->SetScanDepth(3);     //检索文件夹深度
    dicomdir->IgnoreDicomdirOn();  //忽略索引文件
    dicomdir->RequirePixelDataOn();//忽略没有像素数据的文件
    dicomdir->ShowHiddenOff();     //忽略隐藏文件
    dicomdir->Update();
    int n = dicomdir->GetNumberOfSeries();
    if (n == 0) {
        std::cerr << "No DICOM images in directory!" << std::endl;
        return;
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
    vtkIdType nf = sortedFiles->GetNumberOfValues();

    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDataByteOrderToLittleEndian();

    reader->SetFileName(sortedFiles->GetValue(nf - 1).c_str());
    reader->Update();
    auto mImagePositionPatient = reader->GetImagePositionPatient();
    auto mImageOrientationPatient = reader->GetImageOrientationPatient();
    auto mSpacing = reader->GetPixelSpacing();
    vtkHelper::PrintArray3(mSpacing, "GetPixelSpacing");


    for (int i = 0; i < 3; i++) {
        ImagePositionPatient[i] = mImagePositionPatient[i];
        Spacing[i] = mSpacing[i];
    }
    for (int i = 0; i < 6; i++) {
        ImageOrientationPatient[i] = mImageOrientationPatient[i];
    }
    vtkHelper::PrintArray3(ImagePositionPatient, "imagePaiteintOritaint");
    vtkHelper::PrintArray6(ImageOrientationPatient, "imageOrientationPatient");

    RescaleSlope = reader->GetRescaleSlope();
    RescaleOffset = reader->GetRescaleOffset();
    vtkHelper::Print2Numbs(RescaleSlope, RescaleOffset, "Rescale");
    {
        // generate the matrix
        //https://vtkusers.public.kitware.narkive.com/bg2u5xzm/correct-coordinate-transformations-for-dicom-data-set



        float *xdir = &ImageOrientationPatient[0];
        float *ydir = &ImageOrientationPatient[3];
        float zdir[3];
        vtkMath::Cross(xdir, ydir, zdir);

        UserMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
        for (int i = 0; i < 3; i++) {
            UserMatrix->Element[i][0] = xdir[i];
            UserMatrix->Element[i][1] = ydir[i];
            UserMatrix->Element[i][2] = zdir[i];
            UserMatrix->Element[i][3] = ImagePositionPatient[i];
        }
        vtkHelper::PrintMatrix4X4(UserMatrix, "UserMatrix");
    }

    vtkSmartPointer<vtkDICOMReader> DICOMreader = vtkSmartPointer<vtkDICOMReader>::New();
    DICOMreader->SetDataByteOrderToLittleEndian();
    DICOMreader->SetMemoryRowOrderToFileNative();

    DICOMreader->SortingOn();                        //图像排序
    DICOMreader->SetFileNames(sortedFiles);
    DICOMreader->Update();
    ImageProperties = vtkSmartPointer<vtkMedicalImageProperties>::New();
    ImageProperties->DeepCopy(DICOMreader->GetMedicalImageProperties());


    PatientMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    PatientMatrix->DeepCopy(DICOMreader->GetPatientMatrix());
    vtkHelper::PrintMatrix4X4(PatientMatrix, "patientMatrix");


    auto metadata = DICOMreader->GetMetaData();
    //(0028,1050) Window Center (WL)：存储窗位的值。
    WindowCenter = metadata->Get(DC::WindowCenter).AsInt();
    //(0028,1051) Window Width (WW)：存储窗宽的值。
    WindowWidth = metadata->Get(DC::WindowWidth).AsInt();

    vtkHelper::Print2Numbs(WindowWidth, WindowCenter, "WW/WC");

//// 创建一个新的 vtkImageReslice 对象
//    vtkSmartPointer<vtkImageReslice> reslice =  vtkSmartPointer<vtkImageReslice>::New();
//    reslice->SetInputConnection(DICOMreader->GetOutputPort());
//    reslice->SetOutputDimensionality(3);
//    reslice->SetInterpolationModeToLinear();
//
//



    // 创建一个变换矩阵来翻转 Y 轴
//    vtkSmartPointer<vtkMatrix4x4> matrix =  vtkSmartPointer<vtkMatrix4x4>::New();
//    matrix->Identity();
//    matrix->SetElement(1, 1, 1);  // 翻转 Y 轴
//    matrix->SetElement(1, 3, DICOMreader->GetOutput()->GetBounds()[3]);  // 调整原点
//    reslice->SetResliceAxes(matrix);
//    reslice->Update();



//    vtkNew<vtkDICOMToRAS> converter;
//    converter->SetInputData(DICOMreader->GetOutput());
//    converter->SetPatientMatrix(PatientMatrix);
//    converter->SetAllowColumnReordering(true);
//    converter->SetAllowRowReordering(true );
//    converter->UpdateMatrix();
//    vtkMatrix4x4 *matrix = converter->GetRASMatrix();
//    converter->Update();

    vtkSmartPointer<vtkImageFlip> flipZFilter =
            vtkSmartPointer<vtkImageFlip>::New();
    flipZFilter->SetInputConnection(DICOMreader->GetOutputPort());
//    flipZFilter->SetFilteredAxis(1);  // 1 Y轴 ，2 表示 Z 轴
    flipZFilter->Update();
    m_imageData = vtkSmartPointer<vtkImageData>::New();
    m_imageData->DeepCopy(flipZFilter->GetOutput());
    m_imageData->SetSpacing(mSpacing);
    m_imageData->SetOrigin(ImagePositionPatient[0], ImagePositionPatient[1], ImagePositionPatient[2]);
    m_imageData->GetDimensions(this->Dimension);
    m_imageData->GetSpacing(this->Spacing);
    m_imageData->GetExtent(this->Extent);
    m_imageData->GetOrigin(this->Origin);



// 设置 Direction
//    double directionMatrixElems[9] = {0};
//
//    DICOMreader->GetDataDirection(directionMatrixElems);

//    vtkSmartPointer<vtkMatrix4x4> directionMatrix =  vtkSmartPointer<vtkMatrix4x4>::New();
//    for (int i = 0; i < 3; ++i) {
//        for (int j = 0; j < 3; ++j) {
//            directionMatrix->SetElement(i,j , directionMatrixElems[i*3+j]);
//        }
//    }
//    m_imageData->SetDirectionMatrix(directionMatrixElems);
//
//    auto bounds = m_imageData->GetBounds();
//    double orginz[3] = {0};
//    for (int i = 0; i < 3; i++) {
//        orginz[i] = bounds[i * 2];
//    }
//    vtkHelper::PrintArray3(orginz, "bounds");


}
