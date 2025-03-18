#include "./renderer.hpp"
#include "QVTKOpenGLNativeWidget.h"
#include "qboxlayout.h"
#include <vtkUnsignedCharArray.h>
#include <vtkUnsignedShortArray.h>

RenderWindow::RenderWindow(PDBFile *file) : QWidget(nullptr) {
  this->file = file;
  mainWidget = new QVTKOpenGLNativeWidget();
  QVBoxLayout *layout = new QVBoxLayout();

  setupVTK();

  layout->addWidget(mainWidget);
  this->setLayout(layout);
  this->setBaseSize(800, 600);
  this->resize(800, 600);
}

void ::RenderWindow::getColor(ATOM *atom, unsigned char color[3], float *scale) {
  char *element = atom->element;
  if (element[0] == ' ' && element[1] == 'C') {
    color[0] = 0;
    color[1] = 255;
    color[2] = 0;
    *scale=1.7;
  } else if (element[0] == ' ' && element[1] == 'N') {
    color[0] = 0;
    color[1] = 0;
    color[2] = 255;
    *scale=1.55;
  } else if (element[0] == ' ' && element[1] == 'O') {
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
    *scale=1.52;
  } else if (element[0] == ' ' && element[1] == 'H') {
    color[0] = 255;
    color[1] = 255;
    color[2] = 255;
    *scale=1.2;
  } else if (element[0] == 'Z' && element[1] == 'N') {
    color[0] = 96;
    color[1] = 96;
    color[2] = 96;
    *scale=1.0;
  } else if (element[0] == ' ' && element[1] == 'P') {
    color[0] = 196;
    color[1] = 128;
    color[2] = 0;
    *scale=1.8;
  } else if (element[0] == ' ' && element[1] == 'S') {
    color[0] = 255;
    color[1] = 255;
    color[2] = 0;
    *scale=1.8;
  } else {
    color[0] = 255;
    color[1] = 0;
    color[2] = 255;
    *scale = 10;
  }
}

void RenderWindow::demo() {

  unsigned char color[3] = {0, 0, 0};
  float size = 1;
  vtkNew<vtkSphereSource> sphere;
  sphere->SetRadius(0.3/1.7);
  sphere->SetThetaResolution(10);
  sphere->SetPhiResolution(10);

  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetName("Colors");
  colors->SetNumberOfComponents(3);
  colors->SetNumberOfTuples(file->atomCount);

  vtkNew<vtkFloatArray> sizes;
  sizes->SetName("Scale");
  sizes->SetNumberOfComponents(1);
  sizes->SetNumberOfValues(file->atomCount);

  vtkNew<vtkPoints> points;
  points->SetNumberOfPoints(file->atomCount);
  int i = 0;
  for (auto pair : file->atoms) {
    auto data = pair.second;
    getColor(&data, color, &size);
    points->SetPoint(i, data.x, data.y, data.z);
    colors->SetTypedTuple(i, color);
    sizes->SetValue(i,size);
    i++;
  }

  vtkNew<vtkPolyData> data;
  data->SetPoints(points);
  data->GetPointData()->SetScalars(colors);
  data->GetPointData()->AddArray(sizes);

  vtkNew<vtkGlyph3D> glyph3D;
  glyph3D->SetSourceConnection(sphere->GetOutputPort());
  glyph3D->SetInputData(data);
  glyph3D->SetColorModeToColorByScalar();
  glyph3D->SetScaleModeToScaleByScalar();
  glyph3D->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Scale");
  glyph3D->Update();

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputConnection(glyph3D->GetOutputPort());
  mapper->ScalarVisibilityOn();

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);

  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(actor);

  window->AddRenderer(renderer);

  this->show();
}

void RenderWindow::renderSlot() { demo(); }

void RenderWindow::setupVTK() { mainWidget->setRenderWindow(window.Get()); }
