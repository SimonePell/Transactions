#include <string>
#include <stdio.h>
using namespace std;


class Persona{
private:
    string nome;
    string cognome;
    string codicefiscale;
public:
    Persona(const string &nome, const string &cognome, const string &codicefiscale){};
    string getNome()const;
    string getCognome()const;
    string getCodicefiscale()const;
};