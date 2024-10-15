//
// Created by dhz on 24-9-30.
//

#ifndef JPDCM3DX_VTKHELPER_H
#define JPDCM3DX_VTKHELPER_H

#include "../BaseHeader.h"


class vtkHelper {

public:
    // 打印 vtkMatrix4x4 类型的矩阵
    static void PrintMatrix4X4(const vtkMatrix4x4 *matrix4X4, const char *flagName) {
        vtkLogF(INFO, "Begin %s#", flagName);
        auto data = matrix4X4->GetData();
        for (int i = 0; i < 4; ++i) {
            vtkLogF(INFO, "%15.6f,%15.6f,%15.6f,%15.6f", data[4 * i], data[4 * i + 1], data[4 * i + 2],
                    data[4 * i + 3]);
        }

        vtkLogF(INFO, "#End %s", flagName);
    }

    template<typename T>
    static void Print2Numbs(const T n1, const T n2, const char *flagName) {
        std::cout << flagName << "#: " << n1 << "," << n2 << std::endl;
    }

    // 模板函数，用于打印任意类型的数组
    template<typename T>
    static void PrintArray3(const T *arr, const char *flagName) {
        std::cout << flagName << "#: " << arr[0] << "," << arr[1] << "," << arr[2] << std::endl;

    }

    template<typename T>
    static void PrintArray6(const T *arr, const char *flagName) {
        std::cout << flagName << "#: " << arr[0] << "," << arr[1] << "," << arr[2] << ",\n";
        std::cout << flagName << "#: " << arr[3] << "," << arr[4] << "," << arr[5] << std::endl;

    }

    static void SetImageOriginFromDICOMTags(vtkImageData *imageData,
                                            const float *imagePositionPatient, const float *imageOrientationPatient);

    static vtkImageData *ReaderDicomImagesITKVTK(const char *folder);

    static void SetupCamera(vtkCamera *camera, const vtkSmartPointer<vtkMatrix4x4> &patientMatrix,
                            int viewType);
    typedef int ViewType;
    static constexpr ViewType Saggital = 0;
    static constexpr ViewType Coronal = 1;
    static constexpr ViewType Axial = 2;
    static constexpr ViewType ThreeD = 3;


private:
    vtkHelper() = default;

    ~vtkHelper() = default;

    vtkHelper(const vtkHelper &);


    static void ReaderDicomImagesWithScalar(const char *dirName);


};


#endif //JPDCM3DX_VTKHELPER_H
