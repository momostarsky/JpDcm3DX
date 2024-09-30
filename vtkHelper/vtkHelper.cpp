//
// Created by dhz on 24-9-30.
//

#include "vtkHelper.h"

void vtkHelper::PrintMatrix4X4(const vtkMatrix4x4 *matrix4X4, const char *flagName) {
    vtkLogF(INFO, "Begin %s#", flagName);
    auto data = matrix4X4->GetData();
    for (int i = 0; i < 4; ++i) {
        vtkLogF(INFO, "%15.6f,%15.6f,%15.6f,%15.6f", data[4 * i], data[4 * i + 1], data[4 * i + 2],
                data[4 * i + 3]);
    }

    vtkLogF(INFO, "#End %s", flagName);
}
