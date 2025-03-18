#include "./mainwindow.hpp"
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
#include <string>
#include <unordered_map>

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

  // Layout
  layout->addWidget(load_file);
  layout->addWidget(show_render);
  layout->addWidget(table);
  setLayout(layout);
  resize(800, 600);

  // Events
  connect(load_file, &QPushButton::clicked, this, &MainWindow::readSlot);
}

void MainWindow::fillTable() {
  if (!loaded)
    return;
  table->clearContents();
  table->setRowCount(file->resCount);
  table->setColumnCount(4);
  table->setHorizontalHeaderItem(0, new QTableWidgetItem("Resiude sequence number"));
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
    table->setItem(i, 0, new QTableWidgetItem(std::to_string(currRes.resSeq).c_str()));
    table->setItem(i, 1, new QTableWidgetItem(std::string(currRes.resname, 3).c_str()));
    table->setItem(i, 2, new QTableWidgetItem((std::to_string(currRes.atomID.size()).c_str())));
    table->setItem(i, 3, new QTableWidgetItem(QString(currRes.chainID)));
    i++;
  }
}

void MainWindow::readSlot() {
  file = new PDBFile(QFileDialog::getOpenFileName().toStdString());
  loaded = true;
  fillTable();
  table->update();
}
