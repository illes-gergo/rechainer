#include "./renderer.hpp"
#include "QVTKOpenGLNativeWidget.h"
#include "qboxlayout.h"
#include <qmessagebox.h>
#include <vtkActor.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkGlyph3DMapper.h>
#include <vtkPointData.h>
#include <vtkProp.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
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
  this->setWindowTitle("ReChaineR - 3D Viewer");
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

  if (file == nullptr){
    QMessageBox msgbox;
    msgbox.setText("No PDB file loaded for rendering!");
    msgbox.setWindowTitle("PDB File error");
    msgbox.exec();
    return;
  }

  unsigned char color[3] = {0, 0, 0};
  float size = 1;

  colors->SetNumberOfTuples(file->atomCount);

  sizes->SetNumberOfValues(file->atomCount);

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
  scaleArray->SetNumberOfTuples(file->connectionCount);
  double len;
  i = 0;
  for (auto pair : pairList) {
    x = (pair.first[0] - pair.second[0]);
    y = (pair.first[1] - pair.second[1]);
    z = (pair.first[2] - pair.second[2]);
    len = std::sqrt(x * x + y * y + z * z);
    scaleArray->SetTuple(i, (double[]){len, 1, 1});
    orientation->SetTuple(i, (double[]){x, y, z});
    i++;
  }


  glyph3D->Update();
  cyl_glyph->Update();

  this->show();
}

void RenderWindow::renderSlot() { demo(); }

void RenderWindow::setupVTK() {

  mainWidget->setRenderWindow(window.Get());

  colors->SetName("Colors");
  colors->SetNumberOfComponents(3);

  sizes->SetName("Scale");
  sizes->SetNumberOfComponents(1);

  orientation->SetName("Orientation");
  orientation->SetNumberOfComponents(3);

  sphere->SetRadius(0.45 / 1.7);
  sphere->SetThetaResolution(25);
  sphere->SetPhiResolution(25);

  line->SetPoint1(-0.5, 0, 0);
  line->SetPoint2(0.5, 0, 0);

  scaleArray->SetName("LengthScaling");
  scaleArray->SetNumberOfComponents(3);

  cylinder->SetInputConnection(line->GetOutputPort());
  cylinder->SetNumberOfSides(25);
  cylinder->SetRadius(0.10);
  cylinder->Update();

  data->SetPoints(points);
  data->GetPointData()->SetScalars(colors);
  data->GetPointData()->AddArray(sizes);
  cylinderData = vtkNew<vtkPolyData>();
  cylinderData->SetPoints(connectionCenters);
  cylinderData->GetPointData()->AddArray(scaleArray);
  cylinderData->GetPointData()->AddArray(orientation);
  
  glyph3D->SetInputData(data);
  cyl_glyph->SetInputData(cylinderData);

  glyph3D->SetCullingAndLOD(true);
  // glyph3D->SetLODColoring(true);
  glyph3D->SetNumberOfLOD(3);
  glyph3D->SetLODDistanceAndTargetReduction(1, 20, 0.3);
  glyph3D->SetLODDistanceAndTargetReduction(2, 40, 0.6);
  glyph3D->SetLODDistanceAndTargetReduction(3, 60, 0.9);
  glyph3D->SetSourceConnection(sphere->GetOutputPort());
  glyph3D->SetColorModeToDirectScalars();
  glyph3D->SetScaleModeToScaleByMagnitude();
  glyph3D->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Scale");


  cyl_glyph->SetCullingAndLOD(true);
  // cyl_glyph->SetLODColoring(true);
  cyl_glyph->SetNumberOfLOD(3);
  cyl_glyph->SetLODDistanceAndTargetReduction(1, 20, 0.30);
  cyl_glyph->SetLODDistanceAndTargetReduction(2, 40, 0.6);
  cyl_glyph->SetLODDistanceAndTargetReduction(3, 60, 0.9);
  cyl_glyph->SetSourceConnection(cylinder->GetOutputPort());
  cyl_glyph->SetOrientationArray("Orientation");
  cyl_glyph->SetScaleArray("LengthScaling");
  cyl_glyph->SetScaleModeToScaleByVectorComponents();
  
  actor->SetMapper(glyph3D);
  actor->GetProperty()->SetSpecular(1.0);
  actor->GetProperty()->SetSpecularPower(25.0);
  actor->GetProperty()->SetSpecularColor((double[]){1,1,1});
  c_actor->SetMapper(cyl_glyph);
  c_actor->GetProperty()->SetSpecular(1.0);
  c_actor->GetProperty()->SetSpecularPower(25.0);
  c_actor->GetProperty()->SetSpecularColor((double[]){1,1,1});

  light->SetLightTypeToHeadlight();
  light->SetPositional(true);

  renderer->AddActor(actor);
  renderer->AddActor(c_actor);
  renderer->AddLight(light);

  window->AddRenderer(renderer);
}
