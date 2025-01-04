#pragma once
#include "qdirectionallight.h"
#include "qentity.h"
#include "qenvironmentlight.h"
#include "qorbitcameracontroller.h"
#include "qpointlight.h"
#include "qrandom.h"
#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DInput>
#include <Qt3DRender>
#include <vector>

#ifndef DEBUG
//#define DEBUG
#endif

class RenderWindow : public Qt3DExtras::Qt3DWindow {

private:
  Qt3DCore::QEntity *scene;
  std::vector<Qt3DCore::QEntity> *atoms;
  Qt3DRender::QDirectionalLight *light;
  Qt3DRender::QCamera *cameraEntity;
  Qt3DExtras::QOrbitCameraController *camController;
  std::vector<Qt3DCore::QEntity *> *objects;

#ifdef DEBUG
  QRandomGenerator *randgen;
#endif
public:
  RenderWindow();
  void CreateSingleSphere(float x, float y, float z, float radius);

public slots:
  void viewScene();
};
