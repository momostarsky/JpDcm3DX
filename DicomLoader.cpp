//
// Created by dhz on 24-10-9.
//

#include "DicomLoader.h"

DicomLoader::~DicomLoader() = default;

void DicomLoader::LoadDicom(const char *path) {
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
            ColorLevel = stod(tagvalue);
        }
    }

    entryId = "0028|1051";
    tagItr = dictionary.Find(entryId);
    if (tagItr != end) {
        MetaDataStringType::ConstPointer entryvalue =
                dynamic_cast<const MetaDataStringType *> (tagItr->second.GetPointer());
        if (entryvalue) {
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            ColorWindow = stod(tagvalue);
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

    m_imageData = imageflip->GetOutput();
    m_imageData->GetDimensions(this->Dimension);

    double temp[3];
    m_imageData->GetOrigin(temp);
    m_imageData->GetSpacing(this->Spacing);
    m_imageData->SetOrigin(-temp[0], -temp[1], temp[2]);
    m_imageData->GetOrigin(this->Origin);

}

DicomLoader::DicomLoader() : m_imageData(nullptr) {

}

vtkSmartPointer<vtkImageData> DicomLoader::GetImageData() const {
    return m_imageData;
}

double DicomLoader::GetColorLevel() const {
    return ColorLevel;
}

double DicomLoader::GetColorWindow() const {
    return ColorWindow;
}
