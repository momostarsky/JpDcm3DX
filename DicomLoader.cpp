//
// Created by dhz on 24-10-9.
//

#include <itkVTKImageToImageFilter.h>
#include "DicomLoader.h"
#include "../vtkHelper/vtkHelper.h"

DicomLoader::~DicomLoader() = default;

void DicomLoader::LoadDicomWithITK(const char *path) {
    using PixelType = signed short;
    using ImageType3D = itk::Image<PixelType, 3>;
    using ImageIoType = itk::GDCMImageIO;
    using NamesGeneratorType = itk::GDCMSeriesFileNames;
    using ReaderType3D = itk::ImageSeriesReader<ImageType3D>;
    using ConnectorType3D = itk::ImageToVTKImageFilter<ImageType3D>;
    using MetaDataStringType = itk::MetaDataObject<std::string>;

    ReaderType3D::Pointer ImageReader3D = ReaderType3D::New();
    ImageIoType::Pointer dicomIO = ImageIoType::New();
    ImageReader3D->SetImageIO(dicomIO);


    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
    nameGenerator->SetDirectory(path);

    using FilenamesContainer = std::vector<std::string>;
    FilenamesContainer fileNames = nameGenerator->GetInputFileNames();
    ImageReader3D->SetFileNames(fileNames);
    ImageReader3D->ForceOrthogonalDirectionOff();


    ImageReader3D->Update();



//    using FilterType = itk::ChangeInformationImageFilter<ImageType3D>;
//    FilterType::Pointer filter = FilterType::New();
//    filter->SetInput(ImageReader3D->GetOutput());
//    ImageType3D::PointType::VectorType translation;
//    //更改坐标原点
//    translation[0] = 0;
//    translation[1] = 0;
//    translation[2] = 0;
//    ImageType3D::PointType origin = ImageReader3D->GetOutput()->GetOrigin();
//    //origin += translation;
//    filter->SetOutputOrigin(translation);
//    filter->ChangeOriginOn();
//    filter->Update();
//    filter->UpdateOutputInformation();

    //原文链接：https://blog.csdn.net/weixin_45547159/article/details/127102202



    ConnectorType3D::Pointer ImageConnector3D = ConnectorType3D::New();
    ImageConnector3D->SetInput(ImageReader3D->GetOutput());

    ImageConnector3D->Update();


    const itk::MetaDataDictionary &dictionary = dicomIO->GetMetaDataDictionary();
    auto itr = dictionary.Begin();
    auto end = dictionary.End();
    std::string entryId = "0028|1050";
    auto tagItr = dictionary.Find(entryId);
    if (tagItr != end) {
        MetaDataStringType::ConstPointer entryvalue =
                dynamic_cast<const MetaDataStringType *> (tagItr->second.GetPointer());
        if (entryvalue) {
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            WindowCenter = stod(tagvalue);
        }
    }

    entryId = "0028|1051";
    tagItr = dictionary.Find(entryId);
    if (tagItr != end) {
        MetaDataStringType::ConstPointer entryvalue =
                dynamic_cast<const MetaDataStringType *> (tagItr->second.GetPointer());
        if (entryvalue) {
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            WindowWidth = stod(tagvalue);
        }
    }

    /*
     *
     * https://www.cnblogs.com/qwcbeyond/archive/2011/03/31/2001557.html
        VTK uses the computer graphics standard of having the origin
             at the upper left of a screen

        ITK uses the mathematical standard of having the origin on
             the lower left.

        Note however that all this is irrelevant since ITK does
        provide any visualization functionalities. At the end
        what matters is how all the elements map to physical space.
        VTK是左上角 ITK是左下角。。。 这就是问题的来源了

        This is a common problem. VTK and ITK use different conventions. ITK
        uses image convention where the first pixel in memory if shown in the
        upper left of the displayed image. VTK uses computer graphics
        convention where the first pixel in memory is shown in the lower left
        of the displayed image.

        The ITK convention conforms to radiology convention.

        You can use vtkImageFlip to flip the vtk images about the y axis.

        There are other differences. VTK images do not honor the orientation
        of DICOM images while ITK images do.

        如何解决呢，使用vtkImageFlip这个类，把整个volume按Y轴反转就行了。。。。如此简单啊

        vtkSmartPointer <vtkImageFlip> imageflip =vtkSmartPointer<vtkImageFlip>::New();
         imageflip->SetInput(image);
         imageflip->SetFilteredAxes(1);//Y轴

     */
    vtkSmartPointer<vtkImageFlip> imageflip = vtkSmartPointer<vtkImageFlip>::New();
    imageflip->SetInputData(ImageConnector3D->GetOutput());
    imageflip->SetFilteredAxes(1);////y轴为1，x轴为0，z轴为2；
    imageflip->Update();

//    vtkSmartPointer<vtkImageFlip> imageflip2 = vtkSmartPointer<vtkImageFlip>::New();
//    imageflip2->SetInputData(imageflip->GetOutput());
//    imageflip2->SetFilteredAxes(1);////y轴为1，x轴为0，z轴为2；
//    imageflip2->Update();

    m_imageData = imageflip->GetOutput();
    m_imageData->GetDimensions(this->Dimension);

//    double temp[3];
//    m_imageData->GetOrigin(temp);
    m_imageData->GetSpacing(this->Spacing);
//    m_imageData->SetOrigin(-temp[0], -temp[1], temp[2]);
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
//    for (vtkIdType ix = 0; ix < nf; ix++) {
//        std::cout << sortedFiles->GetValue(ix) << std::endl;
//    }

    vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    reader->SetDataByteOrderToLittleEndian();
    reader->SetFileName(sortedFiles->GetValue(nf - 1).c_str());
    reader->Update();
    auto mImagePositionPatient = reader->GetImagePositionPatient();
    auto mImageOrientationPatient = reader->GetImageOrientationPatient();

    vtkHelper::PrintArray3(mImagePositionPatient, "imagePaiteintOritaint");
    vtkHelper::PrintArray3(mImageOrientationPatient, "imageOrientationPatient");

    for (int i = 0; i < 3; i++) {
        ImagePositionPatient[i] = mImagePositionPatient[i];
        ImageOrientationPatient[i] = mImageOrientationPatient[i];
    }
    RescaleSlope = reader->GetRescaleSlope();
    RescaleOffset = reader->GetRescaleOffset();
    vtkHelper::Print2Numbs(RescaleSlope, RescaleOffset, "Rescale");
    {
        // generate the matrix


        float *xdir = &ImageOrientationPatient[0];
        float *ydir = &ImageOrientationPatient[3];
        float zdir[3];
        vtkMath::Cross(xdir, ydir, zdir);

        userMatrix =  vtkSmartPointer<vtkMatrix4x4>::New();
        for (int i = 0; i < 3; i++)
        {
            userMatrix->Element[i][0] = xdir[i];
            userMatrix->Element[i][1] = ydir[i];
            userMatrix->Element[i][2] = zdir[i];
            userMatrix->Element[i][3] = ImagePositionPatient[i];
        }
        vtkHelper::PrintMatrix4X4(userMatrix, "userMatrix");
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


    m_imageData = vtkSmartPointer<vtkImageData>::New();
    m_imageData->DeepCopy(DICOMreader->GetOutput());
    m_imageData->SetOrigin(-mImagePositionPatient[0], -mImagePositionPatient[1], mImagePositionPatient[2]);
    m_imageData->GetDimensions(this->Dimension);
    m_imageData->GetSpacing(this->Spacing);
    m_imageData->GetOrigin(this->Origin);
    m_imageData->GetExtent(this->Extent);






}
