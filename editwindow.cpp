#include "./editwindow.hpp"
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qtablewidget.h>
#include <qwidget.h>
#include <vector>

void EditorWindow::initComps() {
  ruleList = new QTableWidget();
  addRule = new QPushButton("Add rule");
  removeRule = new QPushButton("Remove rule");
  editRule = new QPushButton("Edit rule");
  layout = new QVBoxLayout();
  buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(addRule);
  buttonLayout->addWidget(removeRule);
  buttonLayout->addWidget(editRule);
  layout->addWidget(ruleList);
  layout->addLayout(buttonLayout);
  this->setLayout(layout);
  ruleVector = new std::vector<Rule>();
  this->setWindowTitle("ReChaineR - Fragmentaion editor");
  this->resize(500,300);
}

EditorWindow::EditorWindow(QWidget *parent) : QWidget(parent) { initComps(); }
