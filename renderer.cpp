#include "./renderer.hpp"
#include "qcamera.h"
#include "qcylindermesh.h"
#include "qdirectionallight.h"
#include "qentity.h"
#include "qorbitcameracontroller.h"
#include "qphongmaterial.h"
#include "qrandom.h"
#include "qrgb.h"
#include "qspheremesh.h"
#include "qtransform.h"
#include "qvector3d.h"
#include <iostream>
#include <vector>

RenderWindow::RenderWindow() {
  cameraEntity = this->camera();
  scene = new Qt3DCore::QEntity();
  light_entity = new Qt3DCore::QEntity(cameraEntity);
  light = new Qt3DRender::QPointLight(light_entity);
  light_entity->addComponent(light);
  camController = new Qt3DExtras::QOrbitCameraController(cameraEntity);
  AtomicBallz = new std::vector<Qt3DCore::QEntity *>();
  CylindricShafts = new std::vector<Qt3DCore::QEntity *>();
#ifdef DEBUG
  randgen = new QRandomGenerator();
#endif

  cameraEntity->lens()->setPerspectiveProjection(50.0f, 16.0f / 9.0f, 0.01f,
                                                 1000.0f);
  light->setIntensity(1);
  light->setColor("white");
  cameraEntity->setPosition(QVector3D(10, 0, 0));
  cameraEntity->setUpVector(QVector3D(0, 1, 0));
  cameraEntity->setViewCenter(QVector3D(0, 0, 0));
  camController->setCamera(cameraEntity);
  this->defaultFrameGraph()->setClearColor(QColor(QRgb(0x0d0d0f)));
  atoms = new std::vector<Qt3DCore::QEntity>();
  this->setRootEntity(scene);
}

void RenderWindow::CreateSingleSphere(float x, float y, float z, float radius,
                                      int color) {
  auto *sphere = new Qt3DCore::QEntity(scene);
  AtomicBallz->push_back(sphere);
  auto *mesh = new Qt3DExtras::QSphereMesh;
  auto *transform = new Qt3DCore::QTransform();
  auto *material = new Qt3DExtras::QPhongMaterial();
  material->setDiffuse(QColor(QRgb(color)));
  transform->setTranslation(QVector3D(x, y, z));
  mesh->setRadius(radius);
  sphere->addComponent(mesh);
  sphere->addComponent(material);
  sphere->addComponent(transform);
}

void RenderWindow::viewScene() {
#ifdef DEBUG
  CreateSingleSphere(randgen->generateDouble() * 10,
                     randgen->generateDouble() * 10,
                     randgen->generateDouble() * 10, randgen->generateDouble());
#endif

  this->show();
}

void RenderWindow::setCameraTarget(double x, double y, double z) {
  this->cameraEntity->setViewCenter(QVector3D(x, y, z));
}
void RenderWindow::CreateSingleCylinder(float x, float y, float z, float length,
                                        float anglex,float angley,float anglez, float radius) {
  auto *cylinder = new Qt3DCore::QEntity(scene);
  auto *mesh = new Qt3DExtras::QCylinderMesh();
  auto *transform = new Qt3DCore::QTransform();
  auto *material = new Qt3DExtras::QPhongMaterial();
  mesh->setRadius(radius);
  mesh->setLength(length);
  transform->setTranslation(QVector3D(x,y,z));
  transform->setRotation(QQuaternion::rotationTo(QVector3D(0,1,0),QVector3D(anglex,angley,anglez)));

  cylinder->addComponent(mesh);
  cylinder->addComponent(material);
  cylinder->addComponent(transform);
}
