#pragma once

#include "./pdb_handler.hpp"
#include <QVTKOpenGLNativeWidget.h>
#include <qwidget.h>
#include <vtkLight.h>
#include <vtkLineSource.h>
#include <vtkOpenGLGlyph3DMapper.h>
#include <vtkPSphereSource.h>
#include <vtkTubeFilter.h>

class RenderWindow : public QWidget{

private:
  vtkNew<vtkPSphereSource> sphere;
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
  vtkNew<vtkLight> light;

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
