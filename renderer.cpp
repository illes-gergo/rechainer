#include "./renderer.hpp"
#include "qentity.h"
#include "qspheremesh.h"

RenderWindow::RenderWindow() {
  scene = new Qt3DCore::QEntity();
  atoms = new std::vector<Qt3DCore::QEntity>();
  this->setRootEntity(scene);
}

void RenderWindow::CreateSingleSphere() {
  Qt3DCore::QEntity *sphere = new Qt3DCore::QEntity(scene);
  sphere->addComponent(new Qt3DExtras::QSphereMesh());
}

void RenderWindow::viewScene() {
  CreateSingleSphere();
  this->show();
}
