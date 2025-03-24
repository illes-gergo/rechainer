#pragma once

#include "./initWindow.hpp"
#include <QWidget>
#include <qboxlayout.h>
#include <qpushbutton.h>
#include <qtablewidget.h>
#include <vector>

class EditorWindow : public QWidget {
public:
  EditorWindow(QWidget *parent = nullptr);

  QTableWidget *ruleList;
  QPushButton *addRule, *removeRule, *editRule;
  std::vector<Rule> *ruleVector;

private:
  QVBoxLayout *layout;
  QHBoxLayout *buttonLayout;
  void initComps();
};
