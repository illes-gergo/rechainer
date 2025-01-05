#include "./mainwindow.hpp"
#include "qabstractitemview.h"
#include "qboxlayout.h"
#include "qfiledialog.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include "renderer.hpp"
#include <iostream>
#include <string>

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
  if (!loaded)
    return;

  for (auto residue : file->get_resvec()) {
    for (auto atom : residue.get_atomvec()) {
      auto coords = atom.position();
      auto element = atom.get_symbol();
      int color = 0x808080;
      if (element == " C")
        color = 0x00ff00;
      if (element == " N")
        color = 0x0000ff;
      if (element == " O")
        color = 0xff0000;
      if (element == " H")
        color = 0xffffff;
      if (element == " S")
        color = 0xffff00;
      render_window->CreateSingleSphere(coords[0], coords[1], coords[2], 0.4,
                                        color);
    }
  }
}

void MainWindow::readSlot() {
  file = new PDB(QFileDialog::getOpenFileName().toStdString());
  loaded = true;
  fillTable();
  table->update();
  renderPDB();
}
