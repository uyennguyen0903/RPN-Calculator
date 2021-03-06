#include "Controller.h"

void Controller::EVAL() {
  if (pile_.GetPileSize() == 0) {
    throw(ComputerException("Opérateur invalid."));
  }

  Literal& exp_or_prog = pile_.Top();
  Literal::LiteralType type = exp_or_prog.GetLiteralType();

  QString str = exp_or_prog.Print();

  if (type == Literal::LiteralType::kExpression) {
    Literal* atom_value = atom_manager_.GetAtom(str).CopyAtomValue();
    if (atom_value != nullptr) {
      pile_.Pop();
      if (atom_value->GetLiteralType() == Literal::LiteralType::kProgram) {
        str = atom_value->Print();
        CommandeProcess(str.mid(1, str.length() - 2));
      } else {
        pile_.Push(*atom_value);
      }
    } else {
      throw(ComputerException("Expression n'est associée à aucune valeur."));
    }
    return;
  }

  if (type == Literal::LiteralType::kProgram) {
    // Effacer "]" & "[".
    pile_.Pop();
    CommandeProcess(str.mid(1, str.length() - 2));
    return;
  }

  throw(ComputerException(
      "EVAL opération est seulement pour expression/programme litérale."));
}

void Controller::DUP() {
  if (pile_.GetPileSize() == 0) {
    throw(ComputerException("Opérateur invalid."));
  }

  Literal* last_literal = &pile_.Top();

  Literal::LiteralType type = last_literal->GetLiteralType();

  if (type == Literal::LiteralType::kInteger) {
    pile_.Push(literal_manager_.AddLiteral(
        new Integer(dynamic_cast<Integer*>(last_literal))));
  }

  if (type == Literal::LiteralType::kReal) {
    pile_.Push(literal_manager_.AddLiteral(
        new Real(dynamic_cast<Real*>(last_literal))));
  }

  if (type == Literal::LiteralType::kFraction) {
    pile_.Push(literal_manager_.AddLiteral(
        new Fraction(dynamic_cast<Fraction*>(last_literal))));
  }

  if (type == Literal::LiteralType::kExpression) {
    pile_.Push(literal_manager_.AddLiteral(
        new ExpressionLiteral(dynamic_cast<ExpressionLiteral*>(last_literal))));
  }

  if (type == Literal::LiteralType::kProgram) {
    pile_.Push(literal_manager_.AddLiteral(
        new Program(dynamic_cast<Program*>(last_literal))));
  }
}

void Controller::DROP() {
  if (pile_.GetPileSize() == 0) {
    throw(ComputerException("Opérateur invalid."));
  }

  pile_.Pop();
}

void Controller::SWAP() {
  if (pile_.GetPileSize() < 2) {
    throw(ComputerException("Opérateur invalid."));
  }

  Literal& literal1 = pile_.Top();
  pile_.Pop();

  Literal& literal2 = pile_.Top();
  pile_.Pop();

  pile_.Push(literal1);
  pile_.Push(literal2);
}

void Controller::CLEAR() {
  while (pile_.GetPileSize() != 0) {
    pile_.Pop();
  }
}

void Controller::FORGET() {
  if (pile_.GetPileSize() == 0) {
    throw(ComputerException("Opérateur invalid."));
  }

  Literal* last_literal = &pile_.Top();

  Literal::LiteralType type = last_literal->GetLiteralType();

  if (type == Literal::LiteralType::kExpression) {
    ExpressionLiteral* exp = dynamic_cast<ExpressionLiteral*>(last_literal);
    atom_manager_.SetValueAtom(exp->Print(), nullptr);
    //suppression du programme dans la vue programme s'il existe;
    ProgramLayout::GetInstance().deleteElement(exp->Print());
    //suppression de la variable dans la vue variable s'il existe;
    VariableLayout::GetInstance().deleteElement(exp->Print());

    pile_.Pop();
  } else {
    throw(ComputerException(
        "Cet opérateur est seulement utilisé par des litéral expression."));
  }
}

void Controller::IFT() {
  if (pile_.GetPileSize() < 2) {
    throw(ComputerException("Pas assez arguments."));
  }

  Literal* arg2 = &pile_.Top();
  pile_.Pop();
  Literal* arg1 = &pile_.Top();
  pile_.Pop();

  if (!LogicTest(arg1)) return;

  pile_.Push(*arg2);

  Literal::LiteralType type2 = arg2->GetLiteralType();
  if (type2 == Literal::LiteralType::kProgram ||
      type2 == Literal::LiteralType::kExpression)
    EVAL();
}

void Controller::IFTE() {
  if (pile_.GetPileSize() < 3) {
    throw(ComputerException("Pas assez arguments."));
  }

  Literal* arg3 = &pile_.Top();
  pile_.Pop();
  Literal* arg2 = &pile_.Top();
  pile_.Pop();
  Literal* arg1 = &pile_.Top();
  pile_.Pop();

  Literal::LiteralType type;

  if (LogicTest(arg1)) {
    pile_.Push(*arg2);
    type = arg2->GetLiteralType();
  } else {
    pile_.Push(*arg3);
    type = arg3->GetLiteralType();
  }

  if (type == Literal::LiteralType::kProgram ||
      type == Literal::LiteralType::kExpression)
    EVAL();
}
