#include "./mainwindow.hpp"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qpushbutton.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  this->setWindowTitle("ReChainer");
  load_file = new QPushButton("Load PDB"); 
  filename = new QLineEdit();
  layout = new QVBoxLayout();
  layout->addWidget(filename);
  layout->addWidget(load_file);
  setLayout(layout);
  connect(load_file, &QPushButton::clicked, this, &MainWindow::readSlot);
}

void MainWindow::readSlot(){
  file = new PDB(filename->text().toStdString());
}
