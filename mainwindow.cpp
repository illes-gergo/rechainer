#include "./mainwindow.hpp"
#include "editwindow.hpp"
#include "initWindow.hpp"
#include "pdb_handler.hpp"
#include "qabstractitemview.h"
#include "qboxlayout.h"
#include "qfiledialog.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include "renderer.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <qevent.h>
#include <qnamespace.h>
#include <string>
#include <unordered_map>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  loaded = false;
  this->setWindowTitle("ReChaineR");

  // Widget creation
  load_file = new QPushButton("Load PDB");
  show_render = new QPushButton("Show Renderer");
  showEdit = new QPushButton("Show rule editor");

  layout = new QVBoxLayout();
  table = new QTableWidget();
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // Layout
  layout->addWidget(load_file);
  layout->addWidget(show_render);
  layout->addWidget(showEdit);
  layout->addWidget(table);
  setLayout(layout);
  resize(800, 600);

  rwin = new RenderWindow(file);
  edwin = new EditorWindow();
  inwin = new InitWindow();

  // Events
  connect(load_file, &QPushButton::clicked, this, &MainWindow::readSlot);
  connect(show_render, &QPushButton::clicked, rwin, &RenderWindow::renderSlot);
  connect(showEdit, &QPushButton::clicked, edwin, &QWidget::show);
  connect(edwin->editRule, &QPushButton::clicked, inwin, &InitWindow::show);
  connect(edwin->addRule, &QPushButton::clicked, inwin, &InitWindow::show);
}

void MainWindow::closeEvent(QCloseEvent *event){
  rwin->close();
  edwin->close();
  inwin->close();
  std::cout << "C U" << std::endl;
}

void MainWindow::fillTable() {
  if (!loaded)
    return;
  table->clearContents();
  table->setRowCount(file->resCount);
  table->setColumnCount(4);
  table->setHorizontalHeaderItem(
      0, new QTableWidgetItem("Resiude sequence number"));
  table->setHorizontalHeaderItem(1, new QTableWidgetItem("Residue name"));
  table->setHorizontalHeaderItem(2, new QTableWidgetItem("Number of atoms"));
  table->setHorizontalHeaderItem(3, new QTableWidgetItem("Chain identifier"));
  table->resizeColumnsToContents();

  std::vector<int> keys = std::vector<int>(file->residues.size());
  int i = 0;
  for (std::pair<int, RESIDUE> pair : file->residues) {
    keys.at(i) = pair.first;
    i++;
  }

  std::stable_sort(keys.begin(), keys.end());
  RESIDUE currRes;
  i = 0;
  for (int key : keys) {
    currRes = file->residues.at(key);
    table->setItem(
        i, 0, new QTableWidgetItem(std::to_string(currRes.resSeq).c_str()));
    table->setItem(
        i, 1, new QTableWidgetItem(std::string(currRes.resname, 3).c_str()));
    table->setItem(
        i, 2,
        new QTableWidgetItem((std::to_string(currRes.atomID.size()).c_str())));
    table->setItem(i, 3, new QTableWidgetItem(QString(currRes.chainID)));
    i++;
  }
}

void MainWindow::readSlot() {
  file = new PDBFile(QFileDialog::getOpenFileName().toStdString());
  loaded = true;
  fillTable();
  table->update();
  rwin->file = this->file;
}
