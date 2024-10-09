//
// Created by dhz on 24-9-30.
//

#ifndef JPDCM3DX_VTKHELPER_H
#define JPDCM3DX_VTKHELPER_H

#include "../BaseHeader.h"

class vtkHelper {

public:
    static void PrintMatrix4X4(const vtkMatrix4x4 *matrix4X4, const char *flagName);

    static int  ReaderDicomImages(const char * dirName);
    static int  ReaderDicomImagesWithSITK(const char * dirName);
    static void ReaderDicomImagesWithScalar(const char * dirName);
    static void ReaderDicomImagesITKVTK(const char *folder);
private:
    vtkHelper() = default;

    ~vtkHelper() = default;

    vtkHelper(const vtkHelper &);


};


#endif //JPDCM3DX_VTKHELPER_H
