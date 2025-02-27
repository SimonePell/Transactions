#include <gtest/gtest.h>
#include "Persona.h"

//inizializzazione
TEST(PersonaTest, Initialization) {
    Persona persona("John", "Doe", "JHNDOE90A01F205N");
    EXPECT_EQ(persona.getNome(), "John");
    EXPECT_EQ(persona.getCognome(), "Doe");
    EXPECT_EQ(persona.getCodicefiscale(), "JHNDOE90A01F205N");
}

//getters
TEST(PersonaTest, Getters) {
    Persona persona("Alice", "Smith", "ALCSMT85B02G306P");
    EXPECT_EQ(persona.getNome(), "Alice");
    EXPECT_EQ(persona.getCognome(), "Smith");
    EXPECT_EQ(persona.getCodicefiscale(), "ALCSMT85B02G306P");
}

