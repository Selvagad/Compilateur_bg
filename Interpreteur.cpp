#include "Interpreteur.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

Interpreteur::Interpreteur(ifstream & fichier) :
m_lecteur(fichier), m_table(), m_arbre(nullptr) {
}

void Interpreteur::traduitEnCPP(ostream & cout, unsigned int indentation) const {
 cout << setw(4*indentation) << "" << "int main() {" << endl; // Début d’un programme C++
 // Ecrire en C++ la déclaration des variables présentes dans le programme...
 // ... variables dont on retrouvera le nom en parcourant la table des symboles !
 // Par exemple, si le programme contient i,j,k, il faudra écrire : int i; int j; int k; ...
  for (int i = 0 ; i < m_table.getTaille() ; i++){ 
      if (m_table[i]=="<VARIABLE>"){
          cout << setw(4* (indentation+1)) << "" << "int " << m_table[i].getChaine()<<";"<<endl;
      }
}
 getArbre()->traduitEnCPP(cout,indentation+1); // lance l'opération traduitEnCPP sur la racine
 cout << setw(4*(indentation+1)) << "" << "return 0;" << endl ;
 cout << setw(4*indentation) << "}" << endl ; // Fin d’un programme C++
}

void Interpreteur::analyse() {
    m_arbre = programme(); // on lance l'analyse de la première règle
}

void Interpreteur::tester(const string & symboleAttendu) const throw (SyntaxeException) {
    // Teste si le symbole courant est égal au symboleAttendu... Si non, lève une exception
    static char messageWhat[256];
    if (m_lecteur.getSymbole() != symboleAttendu) {
        sprintf(messageWhat,
                "Ligne %d, Colonne %d - Erreur de syntaxe - Symbole attendu : %s - Symbole trouvé : %s",
                m_lecteur.getLigne(), m_lecteur.getColonne(),
                symboleAttendu.c_str(), m_lecteur.getSymbole().getChaine().c_str());
        throw SyntaxeException(messageWhat);
    }
}

void Interpreteur::testerEtAvancer(const string & symboleAttendu) throw (SyntaxeException) {
    // Teste si le symbole courant est égal au symboleAttendu... Si oui, avance, Sinon, lève une exception
    tester(symboleAttendu);
    m_lecteur.avancer();
}

void Interpreteur::erreur(const string & message) const throw (SyntaxeException) {
    // Lève une exception contenant le message et le symbole courant trouvé
    // Utilisé lorsqu'il y a plusieurs symboles attendus possibles...
    static char messageWhat[256];
    sprintf(messageWhat,
            "Ligne %d, Colonne %d - Erreur de syntaxe - %s - Symbole trouvé : %s",
            m_lecteur.getLigne(), m_lecteur.getColonne(), message.c_str(), m_lecteur.getSymbole().getChaine().c_str());
    throw SyntaxeException(messageWhat);
}

Noeud* Interpreteur::programme() {
    // <programme> ::= procedure principale() <seqInst> finproc FIN_FICHIER
    testerEtAvancer("procedure");
    testerEtAvancer("principale");
    testerEtAvancer("(");
    testerEtAvancer(")");
    Noeud* sequence = seqInst();
    testerEtAvancer("finproc");
    tester("<FINDEFICHIER>");
    return sequence;
}

Noeud* Interpreteur::seqInst() {
    // <seqInst> ::= <inst> { <inst> }
    NoeudSeqInst* sequence = new NoeudSeqInst();
    do {
        sequence->ajoute(inst());
    } while (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "si" || m_lecteur.getSymbole() == "tantque" || m_lecteur.getSymbole() == "repeter" || m_lecteur.getSymbole() == "siriche" || m_lecteur.getSymbole() == "ecrire"|| m_lecteur.getSymbole() == "lire");
    // Tant que le symbole courant est un début possible d'instruction...
    // Il faut compléter cette condition chaque fois qu'on rajoute une nouvelle instruction
    return sequence;
}

Noeud* Interpreteur::inst() {
    // <inst> ::= <affectation>  ; | <instSi>
    
    try{
    if (m_lecteur.getSymbole() == "<VARIABLE>") {
        Noeud *affect = affectation();
        testerEtAvancer(";");
        return affect;
    } else if (m_lecteur.getSymbole() == "si")
        return instSi();
    else if (m_lecteur.getSymbole() == "tantque")
        return instTantQue();
    else if (m_lecteur.getSymbole() == "repeter")
        return instRepeter();
    else if (m_lecteur.getSymbole() == "siriche")
        return instSiRiche();
    else if (m_lecteur.getSymbole() == "ecrire")
        return instEcrire();
    else if (m_lecteur.getSymbole() == "lire")
        return instLire();
        // Compléter les alternatives chaque fois qu'on rajoute une nouvelle instruction
    else erreur("Instruction incorrecte");
    }
    
    catch(SyntaxeException const& exc){
        cout << exc.what() << endl;
        
        while((m_lecteur.getSymbole()!="si" && m_lecteur.getSymbole()!="tantque" && m_lecteur.getSymbole()!="pour" && m_lecteur.getSymbole()!="ecrire" && m_lecteur.getSymbole()!="lire" && m_lecteur.getSymbole()!="finproc" && m_lecteur.getSymbole()!="selon") && m_lecteur.getSymbole()!="<FINDEFICHIER>"){
            m_lecteur.avancer();
        }
                 
    }
    
}

Noeud* Interpreteur::affectation() {
    // <affectation> ::= <variable> = <expression> 
    tester("<VARIABLE>");
    Noeud* var = m_table.chercheAjoute(m_lecteur.getSymbole()); // La variable est ajoutée à la table eton la mémorise
    m_lecteur.avancer();
    testerEtAvancer("=");
    Noeud* exp = expression(); // On mémorise l'expression trouvée
    return new NoeudAffectation(var, exp); // On renvoie un noeud affectation
}

Noeud* Interpreteur::expression() {
    // <expression> ::= <facteur> { <opBinaire> <facteur> }
    //  <opBinaire> ::= + | - | *  | / | < | > | <= | >= | == | != | et | ou
    Noeud* fact = facteur();
    while (m_lecteur.getSymbole() == "+" || m_lecteur.getSymbole() == "-" ||
            m_lecteur.getSymbole() == "*" || m_lecteur.getSymbole() == "/" ||
            m_lecteur.getSymbole() == "<" || m_lecteur.getSymbole() == "<=" ||
            m_lecteur.getSymbole() == ">" || m_lecteur.getSymbole() == ">=" ||
            m_lecteur.getSymbole() == "==" || m_lecteur.getSymbole() == "!=" ||
            m_lecteur.getSymbole() == "et" || m_lecteur.getSymbole() == "ou"
            ) {
        Symbole operateur = m_lecteur.getSymbole(); // On mémorise le symbole de l'opérateur
        m_lecteur.avancer();
        Noeud* factDroit = facteur(); // On mémorise l'opérande droit
        fact = new NoeudOperateurBinaire(operateur, fact, factDroit); // Et on construuit un noeud opérateur binaire
    }
    return fact; // On renvoie fact qui pointe sur la racine de l'expression
}

Noeud* Interpreteur::facteur() {
    // <facteur> ::= <entier> | <variable> | - <facteur> | non <facteur> | ( <expression> )
    Noeud* fact = nullptr;
    if (m_lecteur.getSymbole() == "<VARIABLE>" || m_lecteur.getSymbole() == "<ENTIER>") {
        fact = m_table.chercheAjoute(m_lecteur.getSymbole()); // on ajoute la variable ou l'entier à la table
        m_lecteur.avancer();
    } else if (m_lecteur.getSymbole() == "-") { // - <facteur>
        m_lecteur.avancer();
        // on représente le moins unaire (- facteur) par une soustraction binaire (0 - facteur)
        fact = new NoeudOperateurBinaire(Symbole("-"), m_table.chercheAjoute(Symbole("0")), facteur());
    } else if (m_lecteur.getSymbole() == "non") { // non <facteur>
        m_lecteur.avancer();
        // on représente le moins unaire (- facteur) par une soustractin binaire (0 - facteur)
        fact = new NoeudOperateurBinaire(Symbole("non"), facteur(), nullptr);
    } else if (m_lecteur.getSymbole() == "(") { // expression parenthésée
        m_lecteur.avancer();
        fact = expression();
        testerEtAvancer(")");
    } else
        erreur("Facteur incorrect");
    return fact;
}

//
Noeud* Interpreteur::instSi() {
    // <instSi> ::= si ( <expression> ) <seqInst> finsi
    try {
        testerEtAvancer("si");
    } catch (SyntaxeException &e) {
        cout << "erreur si";
    }
    testerEtAvancer("(");
    Noeud* condition = expression(); // On mémorise la condition
    testerEtAvancer(")");
    Noeud* sequence = seqInst(); // On mémorise la séquence d'instruction
    try {
        testerEtAvancer("finsi");
    } catch (SyntaxeException &e) {
        cout << "erreur finsi";
    }
    testerEtAvancer(";");
    

    return new NoeudInstSi(condition, sequence); // Et on renvoie un noeud Instruction Si
}

//<instTantQue>::=tantque(<expression>)<seqInst>fintantque
Noeud* Interpreteur::instTantQue() {
    try {
        testerEtAvancer("tantque");
    } catch (SyntaxeException &e) {
        cout << "erreur tant que";
    }
    testerEtAvancer("(");
    Noeud* condition = expression(); // On mémorise la condition
    testerEtAvancer(")");
    testerEtAvancer("{");
    Noeud* sequence = seqInst(); // On mémorise la séquence d'instruction
    testerEtAvancer("}");

    try {
        testerEtAvancer("fintantque");
    } catch (SyntaxeException &e) {
        cout << "erreur fintantque";
    }
testerEtAvancer(";");


    return new NoeudInstTantQue(condition,sequence);
}

//<instEcrire>::=ecrire(<expression>|<chaine>{,<expression> | <chaine>})
Noeud* Interpreteur::instEcrire() {
    try {
        testerEtAvancer("ecrire");
    } catch (SyntaxeException &e) {
        cout << "erreur ecrire";
    }
    vector<Noeud *> vect1;

    testerEtAvancer("(");
    Noeud* noeud1;
    if (m_lecteur.getSymbole() == "<CHAINE>") {
        noeud1 = m_table.chercheAjoute(m_lecteur.getSymbole()); // on ajoute la variable ou l'entier à la table
        m_lecteur.avancer();
    }else{ noeud1 = expression();}
    vect1.push_back(noeud1);
    while (m_lecteur.getSymbole() == ",") {
        testerEtAvancer(",");
       
        if (m_lecteur.getSymbole() == "<CHAINE>") {
            noeud1 = m_table.chercheAjoute(m_lecteur.getSymbole()); // on ajoute la variable ou l'entier à la table
            m_lecteur.avancer();
        }else{ noeud1 = expression();}
        vect1.push_back(noeud1);
    }
    testerEtAvancer(")");
    testerEtAvancer(";");
    return new NoeudInstEcrire(vect1);
}

//<instLire>::=lire(<variable>{,<variable>})
Noeud* Interpreteur::instLire() {
    try {
        testerEtAvancer("lire");
    } catch (SyntaxeException &e) {
        cout << "erreur lire";
    }
    Noeud* noeud1;
    vector<Noeud*> paramslire;
    testerEtAvancer("(");
    if (m_lecteur.getSymbole() == "<VARIABLE>") {
        noeud1 = m_table.chercheAjoute(m_lecteur.getSymbole());
    }
    while (m_lecteur.getSymbole() == ",") {
        testerEtAvancer(",");
        if (m_lecteur.getSymbole() == "<VARIABLE>") {
           noeud1 = m_table.chercheAjoute(m_lecteur.getSymbole());
           paramslire.push_back(noeud1);
           m_lecteur.avancer();
           
        }
    }
    testerEtAvancer(")");
    testerEtAvancer(";");
    return new NoeudInstLire(paramslire);
}

//<instPour>::=pour([<affectation>];<expression>;[<affectation>])<seqInst>finpour
Noeud* Interpreteur::instPour() {

    try {
        testerEtAvancer("pour");
    } catch (SyntaxeException &e) {
        cout << "erreur pour";
    }

    testerEtAvancer("(");
    Noeud* affect1 = affectation();
    testerEtAvancer(";");
    Noeud* contenu = expression();
    testerEtAvancer(";");
    Noeud* affect2 = affectation();
    testerEtAvancer(")");
    testerEtAvancer("{");
    Noeud* contenu2 = seqInst();
    testerEtAvancer("}");
    testerEtAvancer("finpour");
    testerEtAvancer(";");
    return new NoeudInstPour(affect1,contenu,affect2,contenu2);
}

//<instRepeter>::=repeter<seqInst>jusqua(<expression>)
Noeud* Interpreteur::instRepeter() {
    try {
        testerEtAvancer("repeter");
    } catch (SyntaxeException &e) {
        cout << "erreur repeter";
    }

    Noeud* sequence = seqInst();

    try {
        testerEtAvancer("jusqua");
    } catch (SyntaxeException &e) {
        cout << "erreur jusqua";
    }

    testerEtAvancer("(");
    Noeud* condition = expression();
    testerEtAvancer(")");
    testerEtAvancer(";");
    return new NoeudInstRepeter(sequence,condition);

}

//<instSiRiche> ::=si(<expression>) <seqInst> {sinonsi(<expression>) <seqInst> }[sinon <seqInst>]finsi
Noeud* Interpreteur::instSiRiche() {
    vector<Noeud*> conditions;
    vector<Noeud*> sequences;
    try {
        testerEtAvancer("siriche");
    } catch (SyntaxeException &e) {
        cout << "erreur siriche";
    }

    testerEtAvancer("(");
    Noeud* condition = expression();
    conditions.push_back(condition);
    testerEtAvancer(")");
    Noeud* sequence = seqInst();
    sequences.push_back(sequence);
    while(m_lecteur.getSymbole()=="sinonsi"){
        testerEtAvancer("sinonsi");
        testerEtAvancer("(");
        conditions.push_back(expression());
        testerEtAvancer(")");
        sequences.push_back(seqInst());
    }
    if (m_lecteur.getSymbole() == "sinon"){
        testerEtAvancer("sinon");
        sequences.push_back(seqInst());
    }
    try {
        testerEtAvancer("finsi");
    } catch (SyntaxeException &e) {
        cout << "erreur finsi";
    }
    testerEtAvancer(";");
    return new NoeudInstSiRiche(conditions,sequences);
}
