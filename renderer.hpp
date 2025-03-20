#pragma once
#include <vtkGlyph3DMapper.h>
#include <vtkOpenGLGlyph3DMapper.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkCylinderSource.h>
#include <vtkGlyph3D.h>


#include <vtkActor.h>
#include <vtkCubeSource.h>
#include <vtkGlyph3D.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkFloatArray.h>
#include <vtkType.h>
#include <vtkArrowSource.h>
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>


#include <QApplication>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>

#include <cmath>
#include <cstdlib>

#include "./pdb_handler.hpp"

class RenderWindow : public QWidget{

private:
  vtkNew<vtkSphereSource> sphere;
  vtkNew<vtkLineSource> line;
  vtkNew<vtkTubeFilter> cylinder;
  vtkNew<vtkUnsignedCharArray> colors;
  vtkNew<vtkFloatArray> sizes;
  vtkNew<vtkPoints> points;
  vtkNew<vtkPoints> connectionCenters;
  vtkNew<vtkDoubleArray> orientation;
  vtkNew<vtkDoubleArray> scaleArray;
  vtkNew<vtkPolyData> data;
  vtkNew<vtkPolyData> cylinderData;
  vtkNew<vtkOpenGLGlyph3DMapper> glyph3D;
  vtkNew<vtkOpenGLGlyph3DMapper> cyl_glyph;
  vtkNew<vtkActor> actor;
  vtkNew<vtkActor> c_actor;
  vtkNew<vtkRenderer> renderer;

public:
  RenderWindow(PDBFile *file);
  PDBFile *file;
  QVTKOpenGLNativeWidget* mainWidget;
  void demo();
  void setupVTK();
  void getColor(ATOM *atom, unsigned char color[3], float *scale);

  // VTK stuff
  
  vtkNew<vtkGenericOpenGLRenderWindow> window;

public slots:
  void renderSlot();
};
