#include "./initWindow.hpp"
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qwidget.h>

void InitWindow::initComps() {
  sendRule = new QPushButton("Set rule");
  selectionType = new QComboBox();
  booleanOperation = new QComboBox();
  specText = new QLineEdit();

  selectionType->addItems(QStringList(
      QList<QString>({"Distance", "Residue number", "Atom ID", "Residue name",
                      "Atom name", "Atom type", "Chain ID"})));
  booleanOperation->addItems(QStringList(
      QList<QString>({"Add", "Remove"})));

  layout = new QVBoxLayout();
  layout->addWidget(selectionType);
  layout->addWidget(booleanOperation);
  layout->addWidget(specText);
  layout->addWidget(sendRule);
  this->setWindowTitle("ReChaineR - Rule editor");
  this->resize(500,150);
}

InitWindow::InitWindow(QWidget *parent) : QWidget(parent) {
  initComps();
  this->setLayout(layout);
}
