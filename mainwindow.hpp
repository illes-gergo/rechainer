#pragma once

#include "pdb_handler.hpp"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include "renderer.hpp"
#include <QtWidgets/QtWidgets>

class MainWindow : public QWidget {
  Q_OBJECT;
  QPushButton *load_file, *show_render;
  QVBoxLayout *layout;
  QTableWidget *table;
  PDB *file;
  RenderWindow *render_window;

public:
  MainWindow(QWidget *parent = nullptr);

public slots:
  void readSlot();
  void renderSlot();

private:
  bool loaded;
  void fillTable();
  void renderPDB();
};
