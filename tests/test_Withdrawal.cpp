#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "Withdrawal.h"
#include "Account.h"
#include "Persona.h"

// file usati per i test
const std::string TEST_FILE = "test_account.txt";
const std::string TEST_TRANSACTIONS = "test_transactions.txt";

// classe di test per il prelievo
class WithdrawalTest : public ::testing::Test {
protected:
    // eseguito prima di ogni test
    void SetUp() override {
        // crea un account fittizio
        Persona persona("Mario", "Rossi", "MRORSS80A01");
        account = std::make_unique<Account>("IT1234567", persona, TEST_FILE);

        // svuota il file delle transazioni
        std::ofstream clearFile(TEST_TRANSACTIONS, std::ios::trunc);
        clearFile.close();

        // crea la cartella delle transazioni se non esiste
        if (!std::filesystem::exists("TRANSACTION")) {
            std::filesystem::create_directory("TRANSACTION");
        }
    }

    // eseguito dopo ogni test
    void TearDown() override {
        std::remove(TEST_FILE.c_str());
        std::remove(TEST_TRANSACTIONS.c_str());
    }

    std::unique_ptr<Account> account;
};

// controlla se i valori del prelievo sono corretti dopo la creazione
TEST_F(WithdrawalTest, WithdrawalCreation) {
    Withdrawal withdrawal(50.0, "Shopping", "IT1234567");
    EXPECT_EQ(withdrawal.getAmount(), 50.0);
    EXPECT_EQ(withdrawal.getDescription(), "Shopping");
    EXPECT_EQ(withdrawal.getIban(), "IT1234567");
    EXPECT_EQ(withdrawal.getType(), "Withdrawal");
}

// verifica che il saldo diminuisca correttamente dopo un prelievo
TEST_F(WithdrawalTest, ApplyWithdrawalThroughAccount) {
    account->updateSaldo(200.0);
    account->addTransaction(std::make_unique<Withdrawal>(50.0, "Cibo", "IT1234567"), TEST_TRANSACTIONS);
    EXPECT_EQ(account->getSaldo(), 150.0);
}

// controlla che il prelievo venga scritto nel file delle transazioni
TEST_F(WithdrawalTest, SaveToLogTransaction) {
    account->updateSaldo(500.0);
    account->addTransaction(std::make_unique<Withdrawal>(200.0, "Affitto", "IT1234567"), TEST_TRANSACTIONS);

    std::ifstream file(TEST_TRANSACTIONS);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool found = false;
    // cerca nel file la riga con l'iban e la descrizione
    while (std::getline(file, line)) {
        if (line.find("IT1234567") != std::string::npos &&
            line.find("Affitto") != std::string::npos) {
            found = true;
            break;
        }
    }
    file.close();
    EXPECT_TRUE(found);
}

// controlla che la descrizione del prelievo si possa aggiornare
TEST_F(WithdrawalTest, ModifyTransactionDescription) {
    account->addTransaction(std::make_unique<Withdrawal>(300.0, "Old desc", "IT1234567"), TEST_TRANSACTIONS);

    bool modified = account->modifyTransaction(0, "Nuova descrizione", TEST_TRANSACTIONS);
    EXPECT_TRUE(modified);

    std::ifstream file(TEST_TRANSACTIONS);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool found = false;
    // controlla se la nuova descrizione è presente nel file
    while (std::getline(file, line)) {
        if (line.find("Nuova descrizione") != std::string::npos) {
            found = true;
            break;
        }
    }
    file.close();
    EXPECT_TRUE(found);
}
