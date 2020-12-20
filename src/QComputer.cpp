#include "QComputer.h"

#include <QChar>
#include <QDebug>

QComputer::QComputer(QWidget* parent) : QWidget(parent) {
  pile = new Pile;
  controleur = new Controller(LiteralManager::GetInstance(), *pile, nullptr);
  // Créer la barre de message pour l'utilisateur.
  message = new QLineEdit;
  message->setReadOnly(true);
  message->setFixedHeight(40);
  message->setAlignment(Qt::AlignCenter);
  message->setStyleSheet("font:20px; color: red");

  // Créer et customiser la panel de pile.
  vue_pile = new QTableWidget(pile->GetNbDisplay(), 1);
  // maintenant nbAfficher = 5, il faut modifier quand on a la class Pile.
  vue_pile->horizontalHeader()->setVisible(false);
  vue_pile->horizontalHeader()->setStretchLastSection(true);

  QStringList labelList;
  for (unsigned int i = pile->GetNbDisplay(); i > 0; --i) {
    QString str = QString::number(i);
    labelList << str;
  }

  vue_pile->setVerticalHeaderLabels(labelList);
  //  vue_pile->setDisabled(true);  // Mettre la pile non modiable.

  for (unsigned int i = 0; i < pile->GetNbDisplay(); i++) {
    vue_pile->setItem(i, 0, new QTableWidgetItem(""));
  }

  // Créer la barre de commande.
  commande = new QLineEdit;
  commande->setFixedHeight(40);
  commande->setAlignment(Qt::AlignRight);
  commande->setStyleSheet("font:20px");

  // Mettre un titre à la fenêtre.
  setWindowTitle("Comp'UT");

  // Créer les buttons pour les claviers cliquables.
  // Expressions litérales.
  QPushButton* un = new QPushButton("1");
  connect(un, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* deux = new QPushButton("2");
  connect(deux, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* trois = new QPushButton("3");
  connect(trois, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* quatre = new QPushButton("4");
  connect(quatre, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* cinq = new QPushButton("5");
  connect(cinq, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* six = new QPushButton("6");
  connect(six, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* sept = new QPushButton("7");
  connect(sept, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* huit = new QPushButton("8");
  connect(huit, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* neuf = new QPushButton("9");
  connect(neuf, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* zero = new QPushButton("0");
  connect(zero, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* point = new QPushButton(".");
  connect(point, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* paren_ouvrante = new QPushButton("[");
  connect(paren_ouvrante, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* paren_ferrmante = new QPushButton("]");
  connect(paren_ferrmante, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* space = new QPushButton("SPACE");
  connect(space, SIGNAL(clicked()), this, SLOT(onClick()));

  // Opérandes.
  QPushButton* plus = new QPushButton("+");
  connect(plus, SIGNAL(clicked()), this, SLOT(onClick()));
  connect(plus, SIGNAL(clicked()), this, SLOT(getNextCommande()));
  QPushButton* moins = new QPushButton("-");
  connect(moins, SIGNAL(clicked()), this, SLOT(onClick()));
  connect(moins, SIGNAL(clicked()), this, SLOT(getNextCommande()));
  QPushButton* fois = new QPushButton("*");
  connect(fois, SIGNAL(clicked()), this, SLOT(onClick()));
  connect(fois, SIGNAL(clicked()), this, SLOT(getNextCommande()));
  QPushButton* division = new QPushButton("/");
  connect(division, SIGNAL(clicked()), this, SLOT(onClick()));
  connect(division, SIGNAL(clicked()), this, SLOT(getNextCommande()));

  // Les touches Entrée & Eval.
  QPushButton* entree = new QPushButton("ENTER");
  connect(entree, SIGNAL(clicked()), this, SLOT(getNextCommande()));
  QPushButton* eval = new QPushButton("EVAL");
  connect(eval, SIGNAL(clicked()), this, SLOT(onClick()));

  // Opérateurs numériques
  QPushButton* div = new QPushButton("DIV");
  connect(div, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* mod = new QPushButton("MOD");
  connect(mod, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* neg = new QPushButton("NEG");
  connect(neg, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* sq_root = new QPushButton("SQRT");
  connect(sq_root, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* pow = new QPushButton("POW");
  connect(pow, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* exp = new QPushButton("EXP");
  connect(exp, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* ln = new QPushButton("LN");
  connect(ln, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* num = new QPushButton("NUM");
  connect(num, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* den = new QPushButton("DEN");
  connect(den, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* sin = new QPushButton("SIN");
  connect(sin, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* cos = new QPushButton("COS");
  connect(cos, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* tan = new QPushButton("TAN");
  connect(tan, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* arcsin = new QPushButton("ARCSIN");
  connect(arcsin, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* arccos = new QPushButton("ARCCOS");
  connect(arccos, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* arctan = new QPushButton("ARCTAN");
  connect(arctan, SIGNAL(clicked()), this, SLOT(onClick()));

  // Opérateurs logiques.
  QPushButton* equal = new QPushButton("=");
  connect(equal, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* diff = new QPushButton("!=");
  connect(diff, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* sup = new QPushButton(">");
  connect(sup, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* inf = new QPushButton("<");
  connect(inf, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* et = new QPushButton("AND");
  connect(et, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* ou = new QPushButton("OR");
  connect(ou, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* non = new QPushButton("NOT");
  connect(non, SIGNAL(clicked()), this, SLOT(onClick()));

  // Opérateurs de manipulation.
  QPushButton* dup = new QPushButton("DUP");
  connect(dup, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* drop = new QPushButton("DROP");
  connect(drop, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* echanger = new QPushButton("SWAP");
  connect(echanger, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* effacer = new QPushButton("CLEAR");
  connect(effacer, SIGNAL(clicked()), this, SLOT(onClick()));

  // Opérateurs conditionnels et de boucle.
  QPushButton* ift = new QPushButton("IFT");
  connect(ift, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* ifte = new QPushButton("IFTE");
  connect(ifte, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* backspace = new QPushButton("DEL");
  connect(backspace, SIGNAL(clicked()), this, SLOT(onClick()));

  QHBoxLayout* line0 = new QHBoxLayout;
  line0->addWidget(space);
  line0->addWidget(zero);
  line0->addWidget(point);
  line0->addWidget(entree);

  QHBoxLayout* line1 = new QHBoxLayout;
  line1->addWidget(un);
  line1->addWidget(deux);
  line1->addWidget(trois);
  line1->addWidget(plus);

  QHBoxLayout* line2 = new QHBoxLayout;
  line2->addWidget(quatre);
  line2->addWidget(cinq);
  line2->addWidget(six);
  line2->addWidget(moins);

  QHBoxLayout* line3 = new QHBoxLayout;
  line3->addWidget(sept);
  line3->addWidget(huit);
  line3->addWidget(neuf);
  line3->addWidget(fois);

  QHBoxLayout* line4 = new QHBoxLayout;
  line4->addWidget(eval);
  line4->addWidget(paren_ouvrante);
  line4->addWidget(paren_ferrmante);
  line4->addWidget(division);

  QHBoxLayout* line55 = new QHBoxLayout;
  line55->addWidget(equal);
  line55->addWidget(diff);
  line55->addWidget(sup);
  line55->addWidget(inf);

  QHBoxLayout* line66 = new QHBoxLayout;
  line66->addWidget(drop);
  line66->addWidget(echanger);
  line66->addWidget(effacer);
  line66->addWidget(backspace);

  QHBoxLayout* line5 = new QHBoxLayout;
  line5->addWidget(neg);
  line5->addWidget(den);
  line5->addWidget(num);

  QHBoxLayout* line6 = new QHBoxLayout;
  line6->addWidget(sq_root);
  line6->addWidget(div);
  line6->addWidget(mod);

  QHBoxLayout* line7 = new QHBoxLayout;
  line7->addWidget(pow);
  line7->addWidget(exp);
  line7->addWidget(ln);

  QHBoxLayout* line8 = new QHBoxLayout;
  line8->addWidget(sin);
  line8->addWidget(cos);
  line8->addWidget(tan);

  QHBoxLayout* line9 = new QHBoxLayout;
  line9->addWidget(arcsin);
  line9->addWidget(arccos);
  line9->addWidget(arctan);

  QHBoxLayout* line10 = new QHBoxLayout;
  line10->addWidget(et);
  line10->addWidget(ou);
  line10->addWidget(non);

  QHBoxLayout* line11 = new QHBoxLayout;
  line11->addWidget(ift);
  line11->addWidget(ifte);
  line11->addWidget(dup);

  QHBoxLayout* L1 = new QHBoxLayout;
  L1->addLayout(line11);
  L1->addLayout(line66);

  QHBoxLayout* L2 = new QHBoxLayout;
  L2->addLayout(line10);
  L2->addLayout(line55);

  QHBoxLayout* L3 = new QHBoxLayout;
  L3->addLayout(line5);
  L3->addLayout(line4);

  QHBoxLayout* L4 = new QHBoxLayout;
  L4->addLayout(line6);
  L4->addLayout(line3);

  QHBoxLayout* L5 = new QHBoxLayout;
  L5->addLayout(line7);
  L5->addLayout(line2);

  QHBoxLayout* L6 = new QHBoxLayout;
  L6->addLayout(line8);
  L6->addLayout(line1);

  QHBoxLayout* L7 = new QHBoxLayout;
  L7->addLayout(line9);
  L7->addLayout(line0);

  QVBoxLayout* clavier = new QVBoxLayout;
  clavier->addLayout(L1);
  clavier->addLayout(L2);
  clavier->addLayout(L3);
  clavier->addLayout(L4);
  clavier->addLayout(L5);
  clavier->addLayout(L6);
  clavier->addLayout(L7);

  wClavier = new QGroupBox;
  wClavier->setLayout(clavier);

  // boutons de gestion des vues
  QPushButton* vueGVar = new QPushButton("Gestion des variables");
  connect(vueGVar, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* vueGProg = new QPushButton("Gestion des programmes");
  connect(vueGProg, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* parametre = new QPushButton("Paramètre");
  connect(parametre, SIGNAL(clicked()), this, SLOT(onClick()));

  QHBoxLayout* menuVue = new QHBoxLayout;
  menuVue->addWidget(vueGVar);
  menuVue->addWidget(vueGProg);
  menuVue->addWidget(parametre);

  // boutons de gestion des vues
  QPushButton* affClavierP = new QPushButton("Clavier principal");
  connect(affClavierP, SIGNAL(clicked()), this, SLOT(onClick()));
  QPushButton* affClavierVar =
      new QPushButton("Clavier des variables et programmes");
  connect(affClavierVar, SIGNAL(clicked()), this, SLOT(onClick()));

  QHBoxLayout* menuAffClavier = new QHBoxLayout;
  menuAffClavier->addWidget(affClavierP);
  menuAffClavier->addWidget(affClavierVar);

  // Aligner tous les objets précédents.
  couche = new QVBoxLayout;
  couche->addLayout(menuVue);
  couche->addLayout(menuAffClavier);

  couche->addWidget(message);
  couche->addWidget(vue_pile);
  couche->addWidget(commande);
  couche->addLayout(clavier);
  couche->addWidget(wClavier);
  setLayout(couche);

  connect(pile, SIGNAL(ModifyStatus()), this, SLOT(refresh()));
  connect(commande, SIGNAL(returnPressed()), this, SLOT(getNextCommande()));
}

void QComputer::refresh() {
  message->setText(pile->GetMessage());
  for (size_t i = 0; i < pile->GetNbDisplay(); i++) {
    vue_pile->item(i, 0)->setText("");
  }

  size_t nb = 0;
  QFont fnt;
  fnt.setPointSize(12);
  for (Pile::Iterator it = pile->begin();
       it != pile->end() && nb < pile->GetNbDisplay(); ++it) {
    vue_pile->item(pile->GetNbDisplay() - nb - 1, 0)
        ->setTextAlignment(Qt::AlignRight);
    vue_pile->item(pile->GetNbDisplay() - nb - 1, 0)->setText((*it).Print());
    vue_pile->item(pile->GetNbDisplay() - nb - 1, 0)->setFont(fnt);
    nb++;
  }
}

void QComputer::getNextCommande() {
  QString c = commande->text();  // on récupère la ligne de commande
  pile->SetMessage("");          // effacer l'ancien message.
  controleur->PileBackUp();
  try {
    controleur->CommandeProcess(c);
    commande->clear();
  } catch (ComputerException& error) {
    controleur->PileRestore();
    pile->SetMessage(error.GetInfo());
    refresh();
  }
}

void QComputer::onClick() {
  QPushButton* button = (QPushButton*)sender();

  if (button->text() == "SPACE") {
    QString c = commande->text();  // on récupère la ligne de commande
    c += " ";
    commande->setText(c);
    return;
  }

  if (button->text() == "DEL") {
    QString c = commande->text();  // on récupère la ligne de commande
    c.remove(c.size() - 1, 1);
    commande->setText(c);
    return;
  }

  if (button->text() == "Gestion des variables") {
    // affichage de la vue de gestion des variables
    return;
  }

  if (button->text() == "Gestion des programmes") {
    // affichage de la vue de gestion des programmes
    return;
  }

  if (button->text() == "Paramètre") {
    // affichage de la vue des paramtres
    return;
  }

  if (button->text() == "Clavier principal") {
    // modification de la visibilité du claviers des variables et programmes
    pile->SetMessage("Affichage/Masquage du clavier principal");
    if (wClavier->isVisible() == true) {
      wClavier->hide();
      this->setFixedSize(680, 445);
    } else {
      wClavier->show();
      this->setFixedSize(680, 775);
    }
    return;
  }

  if (button->text() == "Clavier des variables et programmes") {
    // modification de la visibilité du claviers des variables et programmes
    pile->SetMessage("Affichage/Masquage du clavier variables et programmes");
    return;
  }

  QString c = commande->text();  // on récupère la ligne de commande
  QString newc = button->text();
  if (newc.size() != 1 or checkInput(newc[0]) or
      (c.size() > 0 and checkInput(c[c.size() - 1])))
    c += " ";
  c += newc;
  commande->setText(c);
}

bool QComputer::checkInput(QChar input) {
  // vérifier si input n'est pas un valeur numérique ni une virgule ni un espace
  return !input.isDigit() and input != '.' and input != ' ';
}
