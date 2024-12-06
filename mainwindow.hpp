#pragma once

#include "pdb_handler.hpp"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include <QtWidgets/QtWidgets>

class MainWindow : public QWidget {
  Q_OBJECT;
  QPushButton *load_file;
  QLineEdit *filename;
  QVBoxLayout *layout;
  PDB *file;

  public:
    MainWindow(QWidget *parent = nullptr);

  public slots:
    void readSlot();  

};
