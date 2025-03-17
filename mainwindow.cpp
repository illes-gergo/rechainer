#include "./mainwindow.hpp"
#include "qabstractitemview.h"
#include "qboxlayout.h"
#include "qfiledialog.h"
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

  // Layout
  layout->addWidget(load_file);
  layout->addWidget(show_render);
  layout->addWidget(table);
  setLayout(layout);
  resize(800, 600);
  render_window = new RenderWindow();

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

  for (int i = 0; i < file->resCount; i++) {
    table->setItem(
        i, 0, new QTableWidgetItem(QString(std::to_string(file->residues.at(i)->resSeq).c_str())));
    table->setItem(i, 1,
                   new QTableWidgetItem(std::string(file->residues.at(i)->resname,3).c_str()));
    table->setItem(i, 2,
                   new QTableWidgetItem(
                       (std::to_string(file->residues.at(i)->atomIndex.size()) + '\0').c_str()));
    table->setItem(i, 3, new QTableWidgetItem(QString(file->residues.at(i)->chainID)));
  }
}

void MainWindow::readSlot() {
  file = new PDBFile(QFileDialog::getOpenFileName().toStdString());
  loaded = true;
  fillTable();
  table->update();
}
