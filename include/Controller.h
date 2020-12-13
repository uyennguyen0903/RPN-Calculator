#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <vector>

#include "AtomManager.h"
#include "ExpressionLiteral.h"
#include "LiteralManager.h"
#include "NumericLiteral.h"
#include "Operand.h"
#include "Operator.h"
#include "Pile.h"
#include "Utils.h"
#include "Program.h"

class Controller {
 private:
  LiteralManager& literal_manager_;
  AtomManager& atom_manager_ = AtomManager::GetInstance();
  Pile& pile_;
  Operator* operator_;

 public:
  Controller(LiteralManager& literal_manager, Pile& pile,
             Operator* op = nullptr)
      : literal_manager_(literal_manager), pile_(pile), operator_(op){};

  ~Controller() { delete operator_; }

  void SetOperator(Operator* op);

  QString Commande(const QString& expression);

  int ParseProgram(const QStringList& list, int position);
};

#endif  // CONTROLLER_H_