#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"
#include <typeinfo>
////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
  for (unsigned int i = 0; i < m_instructions.size(); i++)
    m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
  int valeur = m_expression->executer(); // On exécute (évalue) l'expression
  ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
  int og, od, valeur;
  if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
  if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
  // Et on combine les deux opérandes en fonctions de l'opérateur
  if (this->m_operateur == "+") valeur = (og + od);
  else if (this->m_operateur == "-") valeur = (og - od);
  else if (this->m_operateur == "*") valeur = (og * od);
  else if (this->m_operateur == "==") valeur = (og == od);
  else if (this->m_operateur == "!=") valeur = (og != od);
  else if (this->m_operateur == "<") valeur = (og < od);
  else if (this->m_operateur == ">") valeur = (og > od);
  else if (this->m_operateur == "<=") valeur = (og <= od);
  else if (this->m_operateur == ">=") valeur = (og >= od);
  else if (this->m_operateur == "et") valeur = (og && od);
  else if (this->m_operateur == "ou") valeur = (og || od);
  else if (this->m_operateur == "non") valeur = (!og);
  else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
  }
  return valeur; // On retourne la valeur calculée
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstSi::executer() {
  if (m_condition->executer()) m_sequence->executer();
  return 0; // La valeur renvoyée ne représente rien !
}

NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud* sequence) : m_condition(condition), m_sequence(sequence){}

int NoeudInstTantQue::executer(){
    while (m_condition->executer()) m_sequence->executer();
    return 0;
}



NoeudInstRepeter::NoeudInstRepeter(Noeud* sequence, Noeud* condition) :m_sequence(sequence), m_condition(condition){}

int NoeudInstRepeter::executer(){
    do {
        m_sequence->executer();
    }
    while (m_condition->executer());
    return 0;
}

NoeudInstSiRiche::NoeudInstSiRiche(){} //appel automatique du constructeur vector

void NoeudInstSiRiche::ajouterE(Noeud* expression){
    if (expression!=nullptr) m_expressions.push_back(expression);
}
void NoeudInstSiRiche::ajouterS(Noeud* sequence){
    if (sequence!=nullptr) m_sequences.push_back(sequence);
}

int NoeudInstSiRiche::executer(){
    bool sin= false;
    
    for (int i=0; i< m_expressions.size();i++){
            if (m_expressions.at(i)->executer()){
                m_sequences.at(i)->executer();
                sin = true;
            }
    }
    if (sin !=true) {
        m_sequences.at(m_sequences.size()-1);
    }
    return 0;
}

NoeudInstEcrire::NoeudInstEcrire(){}//appel automatique du constructeur vector

void NoeudInstEcrire::ajoute(Noeud* parametre){
     if (parametre!=nullptr) m_parametres.push_back(parametre);
}

int NoeudInstEcrire::executer(){
    
    for (int i=0;i<m_parametres.size();i++){
        
        if ( (typeid(*(m_parametres.at(i)))==typeid(SymboleValue) && *((SymboleValue*)m_parametres.at(i))== "<CHAINE>" )) {
            cout <<((SymboleValue)*m_parametres.at(i)).getChaine();
        }else cout << m_parametres.at(i)->executer();
        
        cout<<endl;
    }
    return 0;
}






NoeudInstLire::NoeudInstLire(){}//appel automatique du constructeur vector


int NoeudInstLire::executer(){
    
    for (int i=0;i<m_parametres.size();i++){
        int val;
        cin >> val;
        ((SymboleValue)*m_parametres.at(i)).setValeur(val);
        
    }
    return 0;
}
NoeudInstPour::NoeudInstPour(Noeud* affectationDeb, Noeud* conditionArret, Noeud* affectationFin, Noeud* sequence): NoeudInstTantQue(conditionArret, sequence), m_affectationDeb(affectationDeb) {
    if (affectationFin != nullptr) {
        m_sequence->ajoute(affectationFin) ; // on ajoute l'incrémentation à la séquence.
    }
}//

int NoeudInstPour::executer() {
    if (m_affectationDeb != nullptr) { // si la premiere affectation n'est pas nulle
        m_affectationDeb->executer(); 
    }
    NoeudInstTantQue::executer();

    return 0;
}
