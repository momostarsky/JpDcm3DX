//
// Created by dhz on 24-9-30.
//


#include <itkDCMTKSeriesFileNames.h>
#include <itkNumericSeriesFileNames.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkImageFlip.h>
#include <itkFlipImageFilter.h>

#include "vtkHelper.h"

void vtkHelper::ReaderDicomImagesITKVTK(const char *folder) {

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
    nameGenerator->SetDirectory(folder);

    using FilenamesContainer = std::vector<std::string>;
    FilenamesContainer fileNames = nameGenerator->GetInputFileNames();
    ImageReader3D->SetFileNames(fileNames);
    ImageReader3D->ForceOrthogonalDirectionOff();

    try {
        ImageReader3D->Update();
    }
    catch (itk::ExceptionObject &ex) {
        std::cout << ex << std::endl;
        return;
    }
    auto visualizing = ImageReader3D->GetOutput();

    visualizing->Print(std::cout);


    ConnectorType3D::Pointer ImageConnector3D = ConnectorType3D::New();
    ImageConnector3D->SetInput(visualizing);
    try {
        ImageConnector3D->Update();
    }
    catch (itk::ExceptionObject &ex) {
        std::cout << ex << std::endl;
        return;
    }

    double ColorLevel = 0;
    double ColorWindow = 0;

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



//    auto   origin = visualizing->GetOrigin();

    vtkSmartPointer<vtkImageFlip> imageflip = vtkSmartPointer<vtkImageFlip>::New();
    imageflip->SetInputData(ImageConnector3D->GetOutput());
    imageflip->SetFilteredAxes(1);//Y轴
    imageflip->Update();

    //
//https://stackoverflow.com/questions/16356331/how-to-correct-the-orientation-of-the-image-with-itk-vtk
//    deally you should re-orient your camera in VTK so that it is suited for medical image visualization. (The default camera in VTK uses the computer graphics conventions).
//
//    If you want a quick hack, you can copy-paste the following code in ITK:
//



    vtkSmartPointer<vtkImageMapToWindowLevelColors> imgToColors = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
    imgToColors->SetInputData(imageflip->GetOutput());
    imgToColors->SetLevel(ColorLevel);
    imgToColors->SetWindow(ColorWindow);
    imgToColors->Update();


    vtkAlgorithm *alg = imgToColors->GetInputAlgorithm();
    vtkImageData *data = imgToColors->GetOutput();
    vtkInformation *outInfo = alg->GetOutputInformation(0);
    data->PrintSelf(std::cout, vtkIndent(2));
//    outInfo->PrintSelf(std::cout, vtkIndent(2));

    int *w_ext = outInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());

    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->SetInputData(data);
    int slice = (w_ext[4] + w_ext[5]) / 2;
    actor->SetDisplayExtent(w_ext[0], w_ext[1], w_ext[2], w_ext[3], slice, slice);

    // w_ext: 0, 480, 0, 480, 0, 480
    cout << "w_ext: " << w_ext[0] << ", " << w_ext[1] << ", " << w_ext[2] << ", " << w_ext[3]
         << ", " << w_ext[4] << ", " << w_ext[5] << endl;

    vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
    ren->AddActor(actor);

    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(ren);

    vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);
    iren->Initialize();

    vtkSmartPointer<vtkInteractorStyleImage> iStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    iren->SetInteractorStyle(iStyle);

    ren->AddActor(actor);
    renWin->SetSize(800, 800);
    renWin->Render();
    ren->ResetCamera();
    renWin->Render();
    iren->Start();


}

void vtkHelper::PrintMatrix4X4(const vtkMatrix4x4 *matrix4X4, const char *flagName) {
    vtkLogF(INFO, "Begin %s#", flagName);
    auto data = matrix4X4->GetData();
    for (int i = 0; i < 4; ++i) {
        vtkLogF(INFO, "%15.6f,%15.6f,%15.6f,%15.6f", data[4 * i], data[4 * i + 1], data[4 * i + 2],
                data[4 * i + 3]);
    }

    vtkLogF(INFO, "#End %s", flagName);
}

int vtkHelper::ReaderDicomImages(const char *dirName) {


    itk::DCMTKSeriesFileNames::SetGlobalWarningDisplay(false);

    using PixelType = signed short;
    constexpr unsigned int Dimension = 3;
    using ImageType = itk::Image<PixelType, Dimension>;
    using NamesGeneratorType = itk::DCMTKSeriesFileNames;
    auto nameGenerator = NamesGeneratorType::New();
    nameGenerator->SetUseSeriesDetails(true);
    nameGenerator->AddSeriesRestriction("0008|0021");
    nameGenerator->SetDirectory(dirName);


    using SeriesIdContainer = std::vector<std::string>;
    const SeriesIdContainer &seriesUID = nameGenerator->GetSeriesUIDs();
    auto seriesItr = seriesUID.begin();
    auto seriesEnd = seriesUID.end();


    const std::string &seriesIdentifier = *seriesItr;


    std::cout << "\nReading: ";
    std::cout << seriesIdentifier << std::endl;
    using FileNamesContainer = std::vector<std::string>;
    FileNamesContainer fileNames = nameGenerator->GetFileNames(seriesIdentifier);

    std::cout << "Number of files found: " << fileNames.size() << std::endl;
    using ReaderType = itk::ImageSeriesReader<ImageType>;
    auto reader = ReaderType::New();
    using ImageIOType = itk::DCMTKImageIO;
    auto dicomIO = ImageIOType::New();
    reader->SetImageIO(dicomIO);
    reader->SetFileNames(fileNames);
    reader->ForceOrthogonalDirectionOff(); // properly read CTs with gantry tilt
    reader->UpdateLargestPossibleRegion();

    reader->Print(std::cout);


    return EXIT_SUCCESS;
}

void vtkHelper::ReaderDicomImagesWithScalar(const char *dirName) {
    vtkNew<vtkDICOMDirectory> dicomdir;
    dicomdir->SetDirectoryName(dirName);
    dicomdir->RequirePixelDataOff();
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
        return;
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
}

int vtkHelper::ReaderDicomImagesWithSITK(const char *dirName) {
    std::cout << "Reading Dicom directory: " << dirName << std::endl;

    // Read the Dicom image series
    sitk::ImageSeriesReader reader;
    const std::vector<std::string> dicom_names = sitk::ImageSeriesReader::GetGDCMSeriesFileNames(dirName);
    reader.SetFileNames(dicom_names);

    sitk::Image image = reader.Execute();

    std::vector<unsigned int> size = image.GetSize();
    std::vector<double> spacing = image.GetSpacing();
    std::vector<double> origin = image.GetOrigin();
    std::cout << "Image size: " << size[0] << " " << size[1] << " " << size[2] << std::endl;
    std::cout << "Image spacing: " << spacing[0] << " " << spacing[1] << " " << spacing[2] << std::endl;
    std::cout << "Image origin: " << origin[0] << " " << origin[1] << " " << origin[2] << std::endl;

    auto pItkImage = image.GetITKBase();
    pItkImage->Print(std::cout);

    itk::Image<int, 3>::Pointer pItkImage3D = dynamic_cast<itk::Image<int, 3> *>(pItkImage);

    pItkImage3D->Print(std::cout);

    return 0;
}
