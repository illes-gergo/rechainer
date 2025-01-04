#include "./renderer.hpp"
#include "qdirectionallight.h"
#include "qentity.h"
#include "qorbitcameracontroller.h"
#include "qphongmaterial.h"
#include "qrandom.h"
#include "qrgb.h"
#include "qspheremesh.h"
#include "qtransform.h"
#include <vector>

RenderWindow::RenderWindow() {
  scene = new Qt3DCore::QEntity();
  light = new Qt3DRender::QDirectionalLight(scene);
  camController = new Qt3DExtras::QOrbitCameraController(scene);
  objects = new std::vector<Qt3DCore::QEntity *>();
#ifdef DEBUG
  randgen = new QRandomGenerator();
#endif

  light->setIntensity(1);
  light->setWorldDirection(QVector3D(-1, -1, -1));
  cameraEntity = this->camera();
  cameraEntity->lens()->setPerspectiveProjection(30.0f, 16.0f / 9.0f, 0.1f,
                                                 1000.0f);
  cameraEntity->setPosition(QVector3D(1, 1, 1));
  cameraEntity->setUpVector(QVector3D(0, 1, 0));
  cameraEntity->setViewCenter(QVector3D(0, 0, 0));
  camController->setCamera(cameraEntity);
  this->setRootEntity(scene);
  this->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
  atoms = new std::vector<Qt3DCore::QEntity>();
}

void RenderWindow::CreateSingleSphere(float x, float y, float z, float radius) {
  auto *sphere = new Qt3DCore::QEntity(scene);
  objects->push_back(sphere);
  auto *mesh = new Qt3DExtras::QSphereMesh;
  auto *transform = new Qt3DCore::QTransform();
  auto *material = new Qt3DExtras::QPhongMaterial();
  material->setDiffuse(QColor(QRgb(0xffffff)));
  transform->setTranslation(QVector3D(x, y, z));
  mesh->setRadius(radius);
  sphere->addComponent(mesh);
  sphere->addComponent(material);
  sphere->addComponent(transform);
}

void RenderWindow::viewScene() {
  #ifdef DEBUG
  CreateSingleSphere(randgen->generateDouble()*10, randgen->generateDouble()*10,
                     randgen->generateDouble()*10, randgen->generateDouble());
  #endif

  this->show();
}
