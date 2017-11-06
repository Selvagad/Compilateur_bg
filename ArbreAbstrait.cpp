#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"
#include <typeinfo>
#include <iomanip>
#include <random>
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
    if (instruction != nullptr) m_instructions.push_back(instruction);
}

void NoeudSeqInst::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    for (unsigned int i = 0; i < m_instructions.size(); i++) {
        m_instructions[i]->traduitEnCPP(cout, indentation);
        cout << endl;
    }
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

void NoeudAffectation::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    m_variable->traduitEnCPP(cout, indentation);
    cout << " = ";
    m_expression->traduitEnCPP(cout, 0);
    cout << ";";
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

void NoeudOperateurBinaire::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    m_operandeGauche->traduitEnCPP(cout, 0);
    cout << " " << m_operateur.getChaine() << " ";
    m_operandeDroit->traduitEnCPP(cout, 0);
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

void NoeudInstSi::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    cout << setw(4 * indentation) << "" << "if ("; // Ecrit "if (" avec un décalage de 4*indentation espaces
    m_condition->traduitEnCPP(cout, 0); // Traduit la condition en C++ sans décalage
    cout << ") {" << endl; // Ecrit ") {" et passe à la ligne
    m_sequence->traduitEnCPP(cout, indentation + 1); // Traduit en C++ la séquence avec indentation augmentée
    cout << setw(4 * indentation) << "" << "}" << endl; // Ecrit "}" avec l'indentation initiale et passe à la ligne
}



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Tant Que/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud* sequence) : m_condition(condition), m_sequence(sequence) {
}

int NoeudInstTantQue::executer() {
    while (m_condition->executer()) m_sequence->executer();
    return 0;
}

void NoeudInstTantQue::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    cout << setw(4 * indentation) << "" << "while (";
    m_condition->traduitEnCPP(cout, 0);
    cout << ") {" << endl;
    m_sequence->traduitEnCPP(cout, indentation + 1);
    cout << setw(4 * indentation) << "" << "}";
}



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////REPETER/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

NoeudInstRepeter::NoeudInstRepeter(Noeud* sequence, Noeud* condition) : m_sequence(sequence), m_condition(condition) {
}

int NoeudInstRepeter::executer() {
    do {
        m_sequence->executer();
    }    while (m_condition->executer());
    return 0;
}

void NoeudInstRepeter::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    cout << setw(4 * indentation) << "" << "while (";
    m_condition->traduitEnCPP(cout, 0);
    cout << ") {" << endl;
    cout << setw(4 * indentation + 1) << "" << "do {" << endl;
    m_sequence->traduitEnCPP(cout, indentation + 1);
    cout << setw(4 * indentation + 1) << "" << "}" << endl;
    cout << setw(4 * indentation) << "" << "}" << endl;

}



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////SI RICHE /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

NoeudInstSiRiche::NoeudInstSiRiche(vector<Noeud *> expressions, vector<Noeud *> sequences) : m_conditions(expressions), m_sequences(sequences) {
} //appel automatique du constructeur vector

void NoeudInstSiRiche::ajouterE(Noeud* expression) {
    if (expression != nullptr) m_conditions.push_back(expression);
}

void NoeudInstSiRiche::ajouterS(Noeud* sequence) {
    if (sequence != nullptr) m_sequences.push_back(sequence);
}

int NoeudInstSiRiche::executer() {
    bool sin = false;

    for (int i = 0; i < m_conditions.size(); i++) { //on parcourt toutes les conditions rentrées dans le si et on execute leurs sequences respectives
        if (m_conditions.at(i)->executer()) {
            m_sequences.at(i)->executer();
            sin = true; // on declare un booleen qui servira a indiquer si on est toujours dans les sinon si ou si il est faux ça veut dire qu'on est dans le sinon final
        }
    }
    if (sin != true) {
        m_sequences.at(m_sequences.size() - 1)->executer();     //on passe au sinon final
    }
    return 0;
}

void NoeudInstSiRiche::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    cout << setw(4 * indentation) << ""<< "if ("; 
    m_conditions.at(0)->traduitEnCPP(cout, 0); 
    cout << ") {" << endl; 
    m_sequences.at(0)->traduitEnCPP(cout, indentation + 1); 
    cout << setw(4 * indentation) <<"" << "}" << endl; 
    for (int i = 1; i < m_conditions.size(); i++) {
        cout << setw(4 * indentation) << "" << "else if(";
        m_conditions.at(i)->traduitEnCPP(cout, 0); 
        cout << ") {" << endl; 
        m_sequences.at(i)->traduitEnCPP(cout, indentation + 1); 
        cout << setw(4 * indentation) << "" << "}" << endl;
    }
    if (m_conditions.size() > 0) {
        cout << setw(4 * indentation) << "" << "else {" << endl;
        m_sequences.at(m_sequences.size() - 1)->traduitEnCPP(cout, indentation + 1);
        cout << setw(4 * indentation) << "" << "}" << endl;
    }


}


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////ECRIRE ///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

NoeudInstEcrire::NoeudInstEcrire(vector<Noeud*> parametres) : m_parametres(parametres) {
}//appel automatique du constructeur vector

void NoeudInstEcrire::ajoute(Noeud* parametre) {
    if (parametre != nullptr) m_parametres.push_back(parametre);
}

int NoeudInstEcrire::executer() {

    for (int i = 0; i < m_parametres.size(); i++) {     //on boucle pour tous les parametres du ecrire

        if ((typeid (*(m_parametres.at(i))) == typeid (SymboleValue) && *((SymboleValue*) m_parametres.at(i)) == "<CHAINE>")) { //on verifie si l'objet est un symbolevalue et une chaine
            cout << ((SymboleValue*) m_parametres.at(i))->getChaine();      //et on affiche ce parametre sur la sortie cout
        } else cout << m_parametres.at(i)->executer();

        cout << endl;
    }
    return 0;
}

void NoeudInstEcrire::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    cout << setw(4 * indentation) << "" << "cout <<";


    for (int i = 0; i < m_parametres.size(); i++) {
        m_parametres.at(i)->traduitEnCPP(cout, 0);
        cout << "<<";

    }
    cout << "endl;" << endl;

}



///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////LIRE /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

NoeudInstLire::NoeudInstLire(Noeud* param, vector<Noeud*> parametres) : m_param(param), m_parametres(parametres) {
}//appel automatique du constructeur vector

int NoeudInstLire::executer() {
    int var;
    cin >> var;
    ((SymboleValue*) m_param)->setValeur(var); //on conserve le 1er parametre

    for (int i = 0; i < m_parametres.size(); i++) {  //pour tout les parametres du vecteur, on lit la valeur et on la set a la variable
        int val;
        cin >> val;
        ((SymboleValue*) m_parametres.at(i))->setValeur(val);

    }
    return 0;
}

void NoeudInstLire::traduitEnCPP(ostream & cout, unsigned int indentation) const {


    cout << setw(4 * indentation) << "" << "cin >> ";
    m_param->traduitEnCPP(cout, 0);

    for (unsigned int i = 0; i < m_parametres.size(); i++) {

        cout << " >> ";
        m_parametres.at(i)->traduitEnCPP(cout, 0);


    }
    cout << ";" << endl;
}


///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////POUR /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

NoeudInstPour::NoeudInstPour(Noeud* affectationDeb, Noeud* conditionArret, Noeud* affectationFin, Noeud* sequence) : NoeudInstTantQue(conditionArret, sequence), m_affectationDeb(affectationDeb), m_affectationFin(affectationFin) {
    if (affectationFin != nullptr) {
        m_sequence->ajoute(affectationFin); // on ajoute l'incrémentation à la séquence.
    }
}//

int NoeudInstPour::executer() {
    if (m_affectationDeb != nullptr) { // si l'affectation du debut n'est pas nulle
        m_affectationDeb->executer();
    }
    NoeudInstTantQue::executer();  //on appelle la methode executer de tant que

    return 0;
}

void NoeudInstPour::traduitEnCPP(ostream & cout, unsigned int indentation) const {

    cout << setw(4 * indentation) << "" << "for(";
    if (m_affectationDeb) {
        m_affectationDeb->traduitEnCPP(cout, 0);

        m_condition->traduitEnCPP(cout, indentation);
        cout << ";";
    } else {
        m_condition->traduitEnCPP(cout, indentation);
        cout << "){";
    }

    if (m_affectationDeb) {     //s'il y a une afffectationDeb c'est un for avec 3 parametres
        m_affectationFin->traduitEnCPP(cout, 0);
        cout << "){" << endl;
    }


    m_sequence->traduitEnCPP(cout, indentation + 1);
    cout << endl;
    cout << setw(4 * indentation) << "" << "}";


}
///////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////SELON /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

NoeudInstSelon::NoeudInstSelon(Noeud* var, Noeud* nb, Noeud* sequence, Noeud* sequenceDefaut, vector<Noeud *> sequences) : m_var(var), m_nb(nb), m_seq(sequence), m_sequences(sequences), m_seqDef(sequenceDefaut) {
} //appel automatique du constructeur vector

void NoeudInstSelon::ajouterS(Noeud* sequence) {
    if (sequence != nullptr) m_sequences.push_back(sequence);    //on a fait une methode ajouter mais on a pas pensé a l'utiliser
}

int NoeudInstSelon::executer() {
    bool sin = true;
    int valeur = ((SymboleValue*) m_var)->getVal();     

    if (valeur == m_nb->executer()) {       //si le cas correspond , on l'execute
        m_seq->executer();
        sin = false;        //et on ne fait pas le defaut
    }

    if (!m_sequences.empty()) {     //pour tout cas , on fait la même verification pour tout les cas
        int i = 0;
        while (i < m_sequences.size() && sin == true) {
            if (valeur == m_sequences.at(i)->executer()) {
                m_sequences.at(i + 1)->executer();
                sin = false;
            }
            i += 2;
        }
    }

    if (m_seqDef != nullptr && sin) {  //si aucun des cas ne correspond on fait le defaut
        m_seqDef->executer();
    }
}

void NoeudInstSelon::traduitEnCPP(ostream & cout, unsigned int indentation) const {
    cout << setw(4 * indentation) << "" << "switch(";
    m_var->traduitEnCPP(cout, 0);
    cout << "){" << endl;

    cout << setw(4 * indentation + 1) << "" << "case ";
    m_nb->traduitEnCPP(cout, 0);
    cout << " : " << endl;
    m_seq->traduitEnCPP(cout, indentation + 2);
    cout << setw(4 * indentation + 1) << "" << "break;";


    for (int i = 0; i < m_sequences.size(); i += 2) { //on incremente de deux car on ajoute la variable "cas" dans le vecteur donc on veut la sauter
        cout << "\n" << setw(4 * indentation + 1) << "" << "case ";
        m_sequences.at(i)->traduitEnCPP(cout, 0);
        cout << " : " << endl;
        m_sequences.at(i + 1)->traduitEnCPP(cout, indentation + 2);
        cout << setw(4 * indentation + 1) << "" << "break;";
    }
    if (m_seqDef) {
        cout << "\n" << setw(4 * indentation + 1) << "" << "defaut : " << endl;
        m_seqDef->traduitEnCPP(cout, indentation + 2);
    }
    cout << setw(4 * indentation) << "" << "}";
}


