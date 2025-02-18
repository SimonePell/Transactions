#include "Persona.h"
using namespace std;

Persona::Persona(const string& nome, const string& cognome, const string& codicefiscale): nome(nome), cognome(cognome), codicefiscale(codicefiscale){}

string Persona::getNome() const {return nome;}

string Persona::getCognome() const {return cognome;}

string Persona::getCodicefiscale() const {return codicefiscale;}