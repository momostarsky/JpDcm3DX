//
// Created by dhz on 24-10-11.
//

#ifndef JPDCM3DY_VTKSLICECALLBACK_H
#define JPDCM3DY_VTKSLICECALLBACK_H

#include "BaseHeader.h"

class vtkSliceCallback : public vtkCommand {
public:
    static vtkSliceCallback *New() {
        return new vtkSliceCallback;
    }

    void Execute(vtkObject *caller, unsigned long eventId, void *callData) override {
        auto *viewer = dynamic_cast<vtkResliceImageViewer *>(caller);
        std::cout << "Slice index changed to " << viewer->GetSlice() << std::endl;
        auto index = std::to_string((viewer->GetSlice() + 1));
        CAnnotation->SetText(vtkCornerAnnotation::LowerRight, index.c_str());
        viewer->Render();

    }

    void SetAnnotation(const vtkSmartPointer<vtkCornerAnnotation>& annotation) {
        this->CAnnotation = annotation;
    }

protected:
    vtkSmartPointer<vtkCornerAnnotation> CAnnotation;

};


#endif //JPDCM3DY_VTKSLICECALLBACK_H
