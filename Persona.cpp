#include "Persona.h"

Persona::Persona(const std::string &nome, const std::string &cognome, const std::string &codicefiscale): nome(nome), cognome(cognome), codicefiscale(codicefiscale){};

std::string Persona::getNome()const{return nome;}

std::string Persona::getCognome()const{return cognome;}

std::string Persona::getCodicefiscale()const{return codicefiscale;}