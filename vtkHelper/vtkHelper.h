//
// Created by dhz on 24-9-30.
//

#ifndef JPDCM3DX_VTKHELPER_H
#define JPDCM3DX_VTKHELPER_H

#include "../BaseHeader.h"


class vtkHelper {

public:

    //轴状面
    static constexpr double Mpr_Axial[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
    //冠状面
    static constexpr double Mpr_Coronal[16] = {
            1, 0, 0, 0,
            0, 0, -1, 0,
            0, 1, 0, 0,
            0, 0, 0, 1};
    //矢状面
    static constexpr double Mpr_Sagittal[16] = {
            0, 0, 1, 0,
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 1};

//    ————————————————
//
//    版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
//
//    原文链接：https://blog.csdn.net/q610098308/article/details/128796964


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


    static constexpr int  SLICE_ORIENTATION_YZ_Sagittal = 0;
    static constexpr int  SLICE_ORIENTATION_XZ_Coronal = 1;
    static constexpr int  SLICE_ORIENTATION_XY_Axial = 2;
    static constexpr int  ThreeD = 3;


private:
    vtkHelper() = default;

    ~vtkHelper() = default;

    vtkHelper(const vtkHelper &);


    static void ReaderDicomImagesWithScalar(const char *dirName);


};


#endif //JPDCM3DX_VTKHELPER_H
