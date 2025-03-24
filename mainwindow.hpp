#pragma once

#include "pdb_handler.hpp"
#include "qboxlayout.h"
#include "qpushbutton.h"
#include "qtablewidget.h"
#include "renderer.hpp"
#include "editwindow.hpp"
#include "initWindow.hpp"
#include <QtWidgets/QtWidgets>
#include <qevent.h>

class MainWindow : public QWidget {
  Q_OBJECT;
  QPushButton *load_file, *show_render, *showEdit, *showInit;
  QVBoxLayout *layout;
  QTableWidget *table;
  PDBFile *file = nullptr;
  RenderWindow *rwin;
  EditorWindow *edwin;
  InitWindow *inwin;
  void closeEvent(QCloseEvent *event);

public:
  MainWindow(QWidget *parent = nullptr);

public slots:
  void readSlot();

private:
  bool loaded;
  void fillTable();
};
