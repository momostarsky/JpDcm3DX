//
// Created by dhz on 24-10-9.
//

#ifndef JPDCM3DY_DICOMLOADER_H
#define JPDCM3DY_DICOMLOADER_H

#include "BaseHeader.h"

class DicomLoader {

public:
    DicomLoader();

    ~DicomLoader();

public:
    void LoadDicomWithITK(const char *path);

    void LoadDicom(const char *path);

    [[nodiscard]] vtkSmartPointer<vtkImageData> GetImageData() const;

    [[nodiscard]] double GetColorLevel() const;

    [[nodiscard]] double GetColorWindow() const;

    [[nodiscard]]const double *GetOrigin() const {
        return &Origin[0];
    }

    [[nodiscard]] const double *GetSpacing() const {
        return &Spacing[0];
    }

    [[nodiscard]] const int *GetDimension() const {
        return &Dimension[0];
    }

    [[nodiscard]] const float *GetImagePositionPatient() const {
        return &ImagePositionPatient[0];
    }

    [[nodiscard]] const float *GetImageOrientationPatient() const {
        return &ImageOrientationPatient[0];
    }

    [[nodiscard]] vtkSmartPointer<vtkMatrix4x4> GetPatientMatrix() const {
        return PatientMatrix;
    }
    [[nodiscard]] vtkSmartPointer<vtkMedicalImageProperties> GetMedicalImageProperties() const {
        return ImageProperties;
    }
private:

    vtkSmartPointer<vtkImageData> m_imageData{};


    /*@
     * (0028,1050) Window Center (WL)：存储窗位的值。
     */
    double WindowCenter = 0;
    /*@
     * (0028,1051) Window Width (WW)：存储窗宽的值。
     */
    double WindowWidth = 0;

    double RescaleSlope = 0;
    double RescaleOffset = 0;
    double Origin[3] = {0};
    double Spacing[3] = {0};
    int Dimension[3] = {0};
    float ImagePositionPatient[3] = {0};
    float ImageOrientationPatient[3] = {0};
    vtkSmartPointer<vtkMatrix4x4> PatientMatrix{};
    vtkSmartPointer<vtkMedicalImageProperties>  ImageProperties{};

};


#endif //JPDCM3DY_DICOMLOADER_H
