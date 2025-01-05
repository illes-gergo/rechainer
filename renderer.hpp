#pragma once
#include "qdirectionallight.h"
#include "qentity.h"
#include "qenvironmentlight.h"
#include "qorbitcameracontroller.h"
#include "qpointlight.h"
#include "qrandom.h"
#include "qvector3d.h"
#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DInput>
#include <Qt3DRender>
#include <vector>

#ifndef DEBUG
// #define DEBUG
#endif

class RenderWindow : public Qt3DExtras::Qt3DWindow {

private:
  Qt3DCore::QEntity *scene, *light_entity;
  Qt3DRender::QPointLight *light;
  std::vector<Qt3DCore::QEntity> *atoms;
  Qt3DRender::QCamera *cameraEntity;
  Qt3DExtras::QOrbitCameraController *camController;
  std::vector<Qt3DCore::QEntity *> *objects;

#ifdef DEBUG
  QRandomGenerator *randgen;
#endif
public:
  RenderWindow();
  void CreateSingleSphere(float x, float y, float z, float radius, int color=0xffffff);
  void setCameraTarget(double x, double y, double z);

public slots:
  void viewScene();
};
