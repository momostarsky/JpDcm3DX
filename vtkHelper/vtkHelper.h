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

    // 模板函数，用于打印任意类型的数组
    template<typename T>
    static void PrintArray(const T *arr, int arrSize) {
        for (int i = 0; i < arrSize; ++i) {
            std::cout << arr[i] << ' ';
        }
        std::cout << std::endl;
    }

    static vtkImageData*  ReaderDicomImagesITKVTK(const char *folder );
private:
    vtkHelper() = default;

    ~vtkHelper() = default;

    vtkHelper(const vtkHelper &);


    void ReaderDicomImagesWithScalar(const char *dirName);
};


#endif //JPDCM3DX_VTKHELPER_H
