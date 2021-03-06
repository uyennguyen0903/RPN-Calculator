#include "BinaryOperator.h"
#include "ClavierVar.h"

void BinaryOperator::Execute() {
  if (IsEnoughArguments(2)) {
    Literal& arg2 = pile_.Top();
    pile_.Pop();
    Literal& arg1 = pile_.Top();
    pile_.Pop();
    Literal* res = nullptr;
    QString error_str = "";
    try {
      res = SimplifyResult(Compute(arg1, arg2));
    } catch (ComputerException& error) {
      error_str = error.GetInfo();
    }
    UpdatePile(arg1, arg2, res, error_str);
    if (error_str != "") throw ComputerException(error_str);
  } else {
    throw(ComputerException("Pas assez argument."));
  }
}

void BinaryOperator::UpdatePile(Literal& arg1, Literal& arg2,
                                Literal* const res, const QString& error_str) {
  if (error_str == "") {
    pile_.Push(literal_manager_.AddLiteral(res));
  } else {
    pile_.Push(arg1);
    pile_.Push(arg2);
  }
}

// ****************************************************************************
// ****************************************************************************

// STO.

void STO::UpdatePile(Literal& arg1, Literal& arg2, Literal* const res,
                     const QString& error_str) {
  if (error_str != "") {
    pile_.Push(arg1);
    pile_.Push(arg2);
  }
}

Literal* STO::Compute(Literal& arg1, Literal& arg2) {
  if (arg2.GetLiteralType() == Literal::LiteralType::kExpression) {
    Literal::LiteralType type1 = arg1.GetLiteralType();

    if (type1 == Literal::LiteralType::kInteger) {
      return Compute(dynamic_cast<Integer&>(arg1),
                     dynamic_cast<ExpressionLiteral&>(arg2));
    }

    if (type1 == Literal::LiteralType::kFraction) {
      return Compute(dynamic_cast<Fraction&>(arg1),
                     dynamic_cast<ExpressionLiteral&>(arg2));
    }

    if (type1 == Literal::LiteralType::kReal) {
      return Compute(dynamic_cast<Real&>(arg1),
                     dynamic_cast<ExpressionLiteral&>(arg2));
    }

    if (type1 == Literal::LiteralType::kProgram) {
      return Compute(dynamic_cast<Program&>(arg1),
                     dynamic_cast<ExpressionLiteral&>(arg2));
    }
  }

  throw(ComputerException("Impossible d'effectuer l'opération " + Print() +
                          " entre " + arg1.Print() + " et " + arg2.Print()));
}

Literal* STO::Compute(Integer& arg1, ExpressionLiteral& arg2) {
  arg2.GetAtom().SetValue(new Integer(&arg1));
  ClavierVar::GetInstance().addButton(arg2.GetAtom().GetId());
  //ajout du variable dans la vue variable;
  VariableLayout::GetInstance().addElement(arg2.GetAtom().GetId(), arg1.Print());
  //suppression du programme dans la vue programme s'il existe;
  ProgramLayout::GetInstance().deleteElement(arg2.GetAtom().GetId());
  return nullptr;
}

Literal* STO::Compute(Real& arg1, ExpressionLiteral& arg2) {
  arg2.GetAtom().SetValue(new Real(&arg1));
  //ajout du bouton correspondant au clavier
  ClavierVar::GetInstance().addButton(arg2.GetAtom().GetId());
  //ajout du variable dans la vue variable;
  VariableLayout::GetInstance().addElement(arg2.GetAtom().GetId(), arg1.Print());
  //suppression du programme dans la vue programme s'il existe;
  ProgramLayout::GetInstance().deleteElement(arg2.GetAtom().GetId());
  return nullptr;
}

Literal* STO::Compute(Fraction& arg1, ExpressionLiteral& arg2) {
  arg2.GetAtom().SetValue(new Fraction(&arg1));
  //ajout du bouton correspondant au clavier
  ClavierVar::GetInstance().addButton(arg2.GetAtom().GetId());
  //ajout du variable dans la vue variable;
  VariableLayout::GetInstance().addElement(arg2.GetAtom().GetId(), arg1.Print());
  //suppression du programme dans la vue programme s'il existe;
  ProgramLayout::GetInstance().deleteElement(arg2.GetAtom().GetId());
  return nullptr;
}

Literal* STO::Compute(Program& arg1, ExpressionLiteral& arg2) {
  arg2.GetAtom().SetValue(new Program(&arg1));
  //ajout du bouton correspondant au clavier
  ClavierVar::GetInstance().addButton(arg2.GetAtom().GetId());
  //ajout du programme dans la vue programme;
  ProgramLayout::GetInstance().addElement(arg2.GetAtom().GetId(), arg1.Print());
  //suppression dde la variable dans la vue programme s'il existe;
  VariableLayout::GetInstance().deleteElement(arg2.GetAtom().GetId());
  return nullptr;
}

// ****************************************************************************
// ****************************************************************************

// DIV, MOD

Literal* DivMod::Compute(Literal& arg1, Literal& arg2) {
  Literal::LiteralType type1 = arg1.GetLiteralType();
  Literal::LiteralType type2 = arg2.GetLiteralType();

  if (type1 == Literal::LiteralType::kExpression) {
    ExpressionLiteral& exp1 = dynamic_cast<ExpressionLiteral&>(arg1);
    Literal* value1 = exp1.GetAtom().CopyAtomValue();
    if (value1 != nullptr) {
      return Compute(*value1, arg2);
    } else {
      throw(ComputerException("Atom/Expression n'est pas value associée."));
    }
  }

  if (type2 == Literal::LiteralType::kExpression) {
    ExpressionLiteral& exp2 = dynamic_cast<ExpressionLiteral&>(arg2);
    Literal* value2 = exp2.GetAtom().CopyAtomValue();
    if (value2 != nullptr) {
      return Compute(arg1, *value2);
    } else {
      throw(ComputerException("Atom/Expression n'est pas value associée."));
    }
  }

  if (arg1.GetLiteralType() == Literal::LiteralType::kInteger &&
      arg2.GetLiteralType() == Literal::LiteralType::kInteger) {
    return Compute(dynamic_cast<Integer&>(arg1), dynamic_cast<Integer&>(arg2));
  }

  throw(ComputerException(
      "Cet opérateur est seulement utilisé pour des litérales entières."));
}

Literal* DivMod::Compute(Integer& arg1, Integer& arg2) {
  if (arg2.GetInt() == 0) {
    throw(ComputerException("Division par zéro"));
  }
  if (div_) {
    return new Integer(arg1.GetInt() / arg2.GetInt());
  } else {
    return new Integer(arg1.GetInt() % arg2.GetInt());
  }
}

// ****************************************************************************
// ****************************************************************************

// AND, OR.

Literal* AndOr::Compute(Literal& arg1, Literal& arg2) {
  if (and_ == true) {
    if (LogicTest(&arg1) && LogicTest(&arg2)) {
      return new Integer(long(1));
    }
    return new Integer(long(0));
  } else {
    if (LogicTest(&arg1) || LogicTest(&arg2)) {
      return new Integer(long(1));
    }
    return new Integer(long(0));
  }
}

// ****************************************************************************
// ****************************************************************************

// POW

Literal* Pow::Compute(Literal& arg1, Literal& arg2) {
  Literal::LiteralType type1 = arg1.GetLiteralType();
  Literal::LiteralType type2 = arg2.GetLiteralType();

  if (type1 == Literal::LiteralType::kProgram ||
      type2 == Literal::LiteralType::kProgram) {
    throw(ComputerException(
        "Cet opérateur n'est pas utilisé pour des litérales programmes."));
  }

  if (type1 == Literal::LiteralType::kExpression) {
    ExpressionLiteral& exp1 = dynamic_cast<ExpressionLiteral&>(arg1);
    Literal* value1 = exp1.GetAtom().CopyAtomValue();
    if (value1 != nullptr) {
      return Compute(*value1, arg2);
    } else {
      throw(ComputerException("Atom/Expression n'est pas value associée."));
    }
  }

  if (type2 == Literal::LiteralType::kExpression) {
    ExpressionLiteral& exp2 = dynamic_cast<ExpressionLiteral&>(arg2);
    Literal* value2 = exp2.GetAtom().CopyAtomValue();
    if (value2 != nullptr) {
      return Compute(arg1, *value2);
    } else {
      throw(ComputerException("Atom/Expression n'est pas value associée."));
    }
  }

  float n1, n2;

  if (type1 != Literal::LiteralType::kReal) {
    n1 = ConvertToReal(arg1)->GetReal();
  } else {
    n1 = (dynamic_cast<Real&>(arg1)).GetReal();
  }

  if (type2 != Literal::LiteralType::kReal) {
    n2 = ConvertToReal(arg2)->GetReal();
  } else {
    n2 = (dynamic_cast<Real&>(arg2)).GetReal();
  }

  if (trunc(n2) != n2 && n1 < 0) {
    throw(ComputerException("Opération invalide."));
  }

  float res = pow(n1, n2);
  if (res == NAN) {
    throw(ComputerException("Operation invalide."));
  }

  return new Real(pow(n1, n2));
}
