#include "./mainwindow.hpp"
#include "qabstractitemview.h"
#include "qboxlayout.h"
#include "qfiledialog.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  loaded = false;
  this->setWindowTitle("ReChainer");

  // Widget creation
  load_file = new QPushButton("Load PDB");
  layout = new QVBoxLayout();
  table = new QTableWidget();
  table->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // Layout
  layout->addWidget(load_file);
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
  table->setRowCount(file->get_rescount());
  table->setColumnCount(4);
  table->setHorizontalHeaderItem(0,new QTableWidgetItem("Resiude sequence number"));
  table->setHorizontalHeaderItem(1,new QTableWidgetItem("Residue name"));
  table->setHorizontalHeaderItem(2,new QTableWidgetItem("Number of atoms"));
  table->setHorizontalHeaderItem(3,new QTableWidgetItem("Chain identifier"));
  table->resizeColumnsToContents();

  for (int i = 0; i < file->get_rescount(); i++) {
    table->setItem(i, 0,
                   new QTableWidgetItem(
                       std::to_string(file->get_res(i).get_resseq()).c_str()));
    table->setItem(
        i, 1, new QTableWidgetItem(file->get_res(i).get_resname().c_str()));
    table->setItem(
        i, 2, new QTableWidgetItem(std::to_string(file->get_res(i).get_reccount()).c_str()));
    table->setItem(
        i, 3, new QTableWidgetItem(file->get_res(i).get_reschain().c_str()));
  }
}

void MainWindow::readSlot() {
  file = new PDB(QFileDialog::getOpenFileName().toStdString());
  loaded = true;
  fillTable();
  table->update();
}
