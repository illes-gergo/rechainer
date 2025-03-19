#include "./renderer.hpp"
#include "QVTKOpenGLNativeWidget.h"
#include "qboxlayout.h"
#include <vtkDoubleArray.h>
#include <vtkGlyph3DMapper.h>
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
    *scale = 1.7;
  } else if (element[0] == ' ' && element[1] == 'N') {
    color[0] = 0;
    color[1] = 0;
    color[2] = 255;
    *scale = 1.55;
  } else if (element[0] == ' ' && element[1] == 'O') {
    color[0] = 255;
    color[1] = 0;
    color[2] = 0;
    *scale = 1.52;
  } else if (element[0] == ' ' && element[1] == 'H') {
    color[0] = 255;
    color[1] = 255;
    color[2] = 255;
    *scale = 1.2;
  } else if (element[0] == 'Z' && element[1] == 'N') {
    color[0] = 96;
    color[1] = 96;
    color[2] = 96;
    *scale = 1.39;
  } else if (element[0] == ' ' && element[1] == 'P') {
    color[0] = 196;
    color[1] = 128;
    color[2] = 0;
    *scale = 1.8;
  } else if (element[0] == ' ' && element[1] == 'S') {
    color[0] = 255;
    color[1] = 255;
    color[2] = 0;
    *scale = 1.8;
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
  sphere->SetRadius(0.45 / 1.7);
  sphere->SetThetaResolution(10);
  sphere->SetPhiResolution(10);

  vtkNew<vtkLineSource> line;
  line->SetPoint1(-0.5, 0, 0);
  line->SetPoint2(0.5, 0, 0);
  vtkNew<vtkTubeFilter> cylinder;
  cylinder->SetInputConnection(line->GetOutputPort());
  cylinder->SetNumberOfSides(10);
  cylinder->SetRadius(0.10);
  cylinder->Update();

  vtkNew<vtkUnsignedCharArray> colors;
  colors->SetName("Colors");
  colors->SetNumberOfComponents(3);
  colors->SetNumberOfTuples(file->atomCount);

  vtkNew<vtkFloatArray> sizes;
  sizes->SetName("Scale");
  sizes->SetNumberOfComponents(1);
  sizes->SetNumberOfValues(file->atomCount);

  vtkNew<vtkPoints> points;
  vtkNew<vtkPoints> connectionCenters;
  points->SetNumberOfPoints(file->atomCount);
  int i = 0;
  for (auto pair : file->atoms) {
    auto data = pair.second;
    getColor(&data, color, &size);
    points->SetPoint(i, data.x, data.y, data.z);
    colors->SetTypedTuple(i, color);
    sizes->SetValue(i, size);
    i++;
  }
  vtkNew<vtkDoubleArray> orientation;
  orientation->SetName("Orientation");
  orientation->SetNumberOfComponents(3);
  orientation->SetNumberOfTuples(file->connectionCount);

  uniquePairList pairList = file->getUniquePairs();

  connectionCenters->SetNumberOfPoints(file->connectionCount);
  i = 0;
  double x, y, z;
  for (auto pair : pairList) {
    x = (pair.first[0] + pair.second[0]) / 2;
    y = (pair.first[1] + pair.second[1]) / 2;
    z = (pair.first[2] + pair.second[2]) / 2;
    connectionCenters->SetPoint(i, x, y, z);
    i++;
  }

  vtkNew<vtkDoubleArray> scaleArray;
  scaleArray->SetName("LengthScaling");
  scaleArray->SetNumberOfComponents(3);
  scaleArray->SetNumberOfTuples(file->connectionCount);
  double len;
  i = 0;
  for (auto pair : pairList) {
    x = (pair.first[0] - pair.second[0]);
    y = (pair.first[1] - pair.second[1]);
    z = (pair.first[2] - pair.second[2]);
    len = std::sqrt(x*x+y*y+z*z);
    scaleArray->SetTuple(i,(double[]){len,1,1});
    orientation->SetTuple(i, (double[]){x, y, z});
    i++;
  }

  vtkNew<vtkPolyData> data;
  data->SetPoints(points);
  data->GetPointData()->SetScalars(colors);
  data->GetPointData()->AddArray(sizes);
  vtkNew<vtkPolyData> cylinderData;
  cylinderData->SetPoints(connectionCenters);
  cylinderData->GetPointData()->AddArray(scaleArray);
  cylinderData->GetPointData()->AddArray(orientation);

  vtkNew<vtkGlyph3DMapper> glyph3D;
  glyph3D->SetSourceConnection(sphere->GetOutputPort());
  glyph3D->SetInputData(data);
  glyph3D->SetColorModeToDirectScalars();
  glyph3D->SetScaleModeToScaleByMagnitude();
  glyph3D->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Scale");
  glyph3D->Update();

  vtkNew<vtkGlyph3DMapper> cyl_glyph;
  cyl_glyph->SetSourceConnection(cylinder->GetOutputPort());
  cyl_glyph->SetInputData(cylinderData);
  cyl_glyph->SetOrientationArray("Orientation");
  cyl_glyph->SetScaleArray("LengthScaling");
  cyl_glyph->SetScaleModeToScaleByVectorComponents();
  cyl_glyph->Update();

  //vtkNew<vtkPolyDataMapper> mapper;
  //mapper->SetInputConnection(glyph3D->GetOutputPort());
  //mapper->ScalarVisibilityOn();

  //vtkNew<vtkPolyDataMapper> c_mapper;
  //c_mapper->SetInputConnection(cyl_glyph->GetOutputPort());
  //c_mapper->Update();

  vtkNew<vtkActor> actor;
  actor->SetMapper(glyph3D);
  vtkNew<vtkActor> c_actor;
  c_actor->SetMapper(cyl_glyph);

  vtkNew<vtkRenderer> renderer;
  renderer->AddActor(actor);
  renderer->AddActor(c_actor);

  window->AddRenderer(renderer);

  this->show();
}

void RenderWindow::renderSlot() { demo(); }

void RenderWindow::setupVTK() { mainWidget->setRenderWindow(window.Get()); }
