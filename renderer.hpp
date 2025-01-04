#pragma once
#include "qdirectionallight.h"
#include "qenvironmentlight.h"
#include "qpointlight.h"
#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DInput>
#include <Qt3DRender>
#include <vector>

class RenderWindow : public Qt3DExtras::Qt3DWindow {

private:
  Qt3DCore::QEntity *scene;
  std::vector<Qt3DCore::QEntity> *atoms;
  Qt3DRender::QDirectionalLight *light;
  Qt3DRender::QCamera *cameraEntity;
  Qt3DExtras::QFirstPersonCameraController *camController;

public:
  RenderWindow();
  void CreateSingleSphere();

public slots:
  void viewScene();
};
