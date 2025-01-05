#include "./mainwindow.hpp"
#include "qabstractitemview.h"
#include "qboxlayout.h"
#include "qfiledialog.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include "renderer.hpp"
#include <cmath>
#include <iostream>
#include <string>

#define VDW_DIVISOR 4

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  loaded = false;
  this->setWindowTitle("ReChainer");

  // Widget creation
  load_file = new QPushButton("Load PDB");
  show_render = new QPushButton("Show Renderer");
  layout = new QVBoxLayout();
  table = new QTableWidget();
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  render_window = new RenderWindow();

  // Layout
  layout->addWidget(load_file);
  layout->addWidget(show_render);
  layout->addWidget(table);
  setLayout(layout);
  resize(800, 600);

  // Events
  connect(load_file, &QPushButton::clicked, this, &MainWindow::readSlot);
  connect(show_render, &QPushButton::clicked, render_window,
          &RenderWindow::viewScene);
}

void MainWindow::fillTable() {
  if (!loaded)
    return;
  table->clearContents();
  table->setRowCount(file->get_rescount());
  table->setColumnCount(4);
  table->setHorizontalHeaderItem(
      0, new QTableWidgetItem("Resiude sequence number"));
  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Residue name"));
  table->setHorizontalHeaderItem(2, new QTableWidgetItem("Number of atoms"));
  table->setHorizontalHeaderItem(3, new QTableWidgetItem("Chain identifier"));
  table->resizeColumnsToContents();

  for (int i = 0; i < file->get_rescount(); i++) {
    table->setItem(i, 0,
                   new QTableWidgetItem(
                       std::to_string(file->get_res(i).get_resseq()).c_str()));
    table->setItem(
        i, 1, new QTableWidgetItem(file->get_res(i).get_resname().c_str()));
    table->setItem(
        i, 2,
        new QTableWidgetItem(
            std::to_string(file->get_res(i).get_reccount()).c_str()));
    table->setItem(
        i, 3, new QTableWidgetItem(file->get_res(i).get_reschain().c_str()));
  }
}

void MainWindow::renderPDB() {
  double xc = 0, yc = 0, zc = 0;
  double xp = 0, yp = 0, zp = 0;
  double xm = 0, ym = 0, zm = 0, dist = 0;
  double radius = 0;
  std::vector<double> pos1, pos2;
  double count = 0;
  int ResCord1 = 0, AtomCord1 = 0;
  int ResCord2 = 0, AtomCord2 = 0;

  if (!loaded)
    return;

  auto resiudess = file->get_resvec();
  auto connectionss = file->get_connections();

  for (auto residue : resiudess) {
    for (auto atom : residue.get_atomvec()) {
      auto coords = atom.position();
      auto element = atom.get_symbol();
      int color = 0x808080;
      radius = 1.5;
      if (element == " C"){
        color = 0x00ff00;
        radius = 1.7;
      }
      if (element == " N") {
        color = 0x0000ff;
        radius = 1.55;
      }
      if (element == " O") {
        color = 0xff0000;
        radius = 1.52;
      }
      if (element == " H") {
        color = 0xffffff;
        radius = 1.2;
      }
      if (element == " S") {
        color = 0xffff00;
        radius = 1.8;
      }
      render_window->CreateSingleSphere(coords[0], coords[1], coords[2],
                                        radius / VDW_DIVISOR, color);
      xc += coords[0];
      yc += coords[1];
      zc += coords[2];
      count += 1;
    }
  }
  for (auto connection : connectionss) {
    ResCord1 = connection[0].ResCoord;
    ResCord2 = connection[1].ResCoord;
    AtomCord1 = connection[0].AtomCoord;
    AtomCord2 = connection[1].AtomCoord;
    pos1 = resiudess[ResCord1].get_atomvec()[AtomCord1].position();
    pos2 = resiudess[ResCord2].get_atomvec()[AtomCord2].position();
    xm = (pos1[0] + pos2[0]) / 2;
    ym = (pos1[1] + pos2[1]) / 2;
    zm = (pos1[2] + pos2[2]) / 2;
    dist = std::pow(std::pow(pos1[0] - pos2[0], 2) +
                        std::pow(pos1[1] - pos2[1], 2) +
                        std::pow(pos1[2] - pos2[2], 2),
                    0.5);
    xp = (pos2[0] - pos1[0]);
    yp = (pos2[1] - pos1[1]);
    zp = (pos2[2] - pos1[2]);
    render_window->CreateSingleCylinder(xm, ym, zm, dist, xp, yp, zp);
  }
  render_window->setCameraTarget(xc / count, yc / count, zc / count);
}

void MainWindow::readSlot() {
  file = new PDB(QFileDialog::getOpenFileName().toStdString());
  loaded = true;
  fillTable();
  table->update();
  renderPDB();
}
