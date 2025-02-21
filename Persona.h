#ifndef PERSONA_H 
#define PERSONA_H

#include <string>

class Persona {
private:
    std::string nome;
    std::string cognome;
    std::string codicefiscale;

public:
    Persona(const std::string &nome, const std::string &cognome, const std::string &codicefiscale);
    std::string getNome() const;
    std::string getCognome() const;
    std::string getCodicefiscale() const;
};

#endif 
