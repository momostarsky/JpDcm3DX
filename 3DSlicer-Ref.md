### 3D Slicer 编译VTK 参考 

```
https://gitee.com/dhz1217/Slicer/blob/main/SuperBuild/External_VTK.cmake
```
```shell
ccmake ../vtk -DVTK_MODULE_ENABLE_VTK_SplineDrivenImageSlicer:BOOL=YES \
-DVTK_MODULE_ENABLE_VTK_ChartsCore:STRING=YES \
-DVTK_MODULE_ENABLE_VTK_ViewsContext2D:STRING=YES \
-DVTK_MODULE_ENABLE_VTK_RenderingContext2D:STRING=YES \
-DVTK_MODULE_ENABLE_VTK_RenderingContextOpenGL2:STRING=YES \
-DVTK_MODULE_ENABLE_VTK_GUISupportQt:STRING=YES \
-DVTK_GROUP_ENABLE_Qt:STRING=YES \
-DVTK_QT_VERSION:STRING=5 \
-DVTK_Group_Qt:BOOL=ON \
-DVTK_GROUP_ENABLE_Imaging:STRING=YES \
-DVTK_USE_MPI:BOOL=ON \
-DVTK_GROUP_ENABLE_MPI:STRING=YES \
-DVTK_GROUP_ENABLE_Rendering:STRING=YES \
-DVTK_GROUP_ENABLE_StandAlone:STRING=YES \
-DVTK_GROUP_ENABLE_Views:STRING=YES \
-DVTK_GROUP_ENABLE_WEB:STRING=YES \
-DVTK_MODULE_ENABLE_VTK_GUISupportQtOpenGL:STRING=YES \
-DModule_vtkRenderingFreeTypeFontConfig:BOOL=ON \
-DVTK_BUILD_TESTING:STRING=OFF \
-DVTK_MODULE_ENABLE_VTK_AcceleratorsVTKm:BOOL=NO \
-DVTK_MODULE_ENABLE_VTK_GUISupportQtQuick:BOOL=NO

```
### 3DSlicer   build
```shell
ccmake ../Slicer -DBUILD_TESTING=OFF -DSlicer_USE_SYSTEM_VTK=OFF  -DSlicer_RELEASE_TYPE=Stable -DSlicer_BUILD_DOCUMENTATION=ON
 
```

### ITK build
```text
ccmake ../ITK  -DBUILD_TESTING:BOOL=OFF \
      -DBUILD_EXAMPLES:BOOL=OFF \
      -DITK_BUILD_DEFAULT_MODULES:BOOL=ON \
      -DModule_ITKReview:BOOL=ON \
      -DModule_MGHIO:BOOL=ON \
      -DModule_ITKIOMINC:BOOL=ON \
      -DModule_IOScanco:BOOL=ON \
      -DModule_MorphologicalContourInterpolation:BOOL=ON \
      -DModule_GrowCut:BOOL=ON \
      -DModule_SimpleITKFilters:BOOL=ON \
      -DModule_GenericLabelInterpolator:BOOL=ON \
      -DModule_AdaptiveDenoising:BOOL=ON \
      -DBUILD_SHARED_LIBS:BOOL=ON \
      -DModule_ITKVtkGlue:BOOL=ON \
      -DModule_ITKDCMTK:BOOL=ON \
      -DModule_ITKIODCMTK:BOOL=ON \
      -DModule_ITKIOGDCM:BOOL=ON \
      -DITK_USE_SYSTEM_DCMTK:BOOL=OFF \
      -DITK_USE_SYSTEM_ZLIB:BOOL=ON 
```
 

