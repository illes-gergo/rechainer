#pragma once

#include "pdb_handler.hpp"
#include "qboxlayout.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include <QtWidgets/QtWidgets>

class MainWindow : public QWidget {
  Q_OBJECT;
  QPushButton *load_file, *show_render;
  QVBoxLayout *layout;
  QTableWidget *table;
  PDBFile *file;

public:
  MainWindow(QWidget *parent = nullptr);

public slots:
  void readSlot();

private:
  bool loaded;
  void fillTable();
};
