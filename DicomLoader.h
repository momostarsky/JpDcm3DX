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

private:

    vtkSmartPointer<vtkImageData> m_imageData{};
    double ColorLevel = 0;
    double ColorWindow = 0;
    double Origin[3] = {0};
    double Spacing[3] = {0};
    int Dimension[3] = {0};
};


#endif //JPDCM3DY_DICOMLOADER_H
