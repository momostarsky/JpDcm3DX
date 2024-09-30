//
// Created by dhz on 24-9-30.
//

#ifndef JPDCM3DX_VTKHELPER_H
#define JPDCM3DX_VTKHELPER_H

#include "../BaseHeader.h"

class vtkHelper {

public:
    static void PrintMatrix4X4(const vtkMatrix4x4 *matrix4X4, const char *flagName);

private:
    vtkHelper() = default;

    ~vtkHelper() = default;

    vtkHelper(const vtkHelper &);
};


#endif //JPDCM3DX_VTKHELPER_H
