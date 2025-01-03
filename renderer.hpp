#pragma once
#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DInput>
#include <Qt3DRender>
#include <vector>

class RenderWindow : public Qt3DExtras::Qt3DWindow {

private:
  Qt3DCore::QEntity *scene;
  std::vector<Qt3DCore::QEntity> *atoms;

public:
  RenderWindow();
  void CreateSingleSphere();

public slots:
  void viewScene();
};
