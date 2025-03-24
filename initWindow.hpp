#pragma once
#include <QWidget>
#include <qboxlayout.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qwidget.h>

enum selecType {
  DISTANCE,
  RESNUM,
  ATOMNUM,
  RESNAME,
  ATOMNAME,
  ATOMTYPE,
  CHAINID
};
enum ruleOperation { ADD, REMOVE };

typedef struct {
  int type;
  int operation;
  std::string spec;
} Rule;

class InitWindow : public QWidget {
public:
  InitWindow(QWidget *parent = nullptr);
  QComboBox *selectionType, *booleanOperation;
  QLineEdit *specText;
  QPushButton *sendRule;

private:
  void initComps();
  QVBoxLayout *layout;
};
