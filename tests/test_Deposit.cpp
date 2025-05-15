#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "Deposit.h"
#include "Account.h"
#include "Persona.h"

// file usati nei test
const std::string TEST_FILE = "test_account.txt";
const std::string TEST_TRANSACTIONS = "test_transactions.txt";

// classe di test per i depositi
class DepositTest : public ::testing::Test {
protected:
    // viene eseguito prima di ogni test
    void SetUp() override {
        // crea un account fittizio
        Persona persona("Mario", "Rossi", "MRORSS80A01");
        account = std::make_unique<Account>("IT1234567", persona, TEST_FILE);

        // svuota il file delle transazioni
        std::ofstream clearFile(TEST_TRANSACTIONS, std::ios::trunc);
        clearFile.close();

        // crea la cartella se non esiste
        if (!std::filesystem::exists("TRANSACTION")) {
            std::filesystem::create_directory("TRANSACTION");
        }
    }

    // viene eseguito dopo ogni test
    void TearDown() override {
        std::remove(TEST_FILE.c_str());
        std::remove(TEST_TRANSACTIONS.c_str());
    }

    std::unique_ptr<Account> account;
};

// controlla se i valori del deposito sono corretti appena creato
TEST_F(DepositTest, DepositCreation) {
    Deposit deposit(150.0, "Stipendio", "IT1234567");
    EXPECT_EQ(deposit.getAmount(), 150.0);
    EXPECT_EQ(deposit.getDescription(), "Stipendio");
    EXPECT_EQ(deposit.getIban(), "IT1234567");
    EXPECT_EQ(deposit.getType(), "Deposit");
}

// verifica che il saldo aumenti quando si fa un deposito
TEST_F(DepositTest, ApplyDepositThroughAccount) {
    account->addTransaction(std::make_unique<Deposit>(200.0, "Bonifico", "IT1234567"), TEST_TRANSACTIONS);
    EXPECT_EQ(account->getSaldo(), 200.0);
}

// controlla che il deposito venga scritto correttamente nel file
TEST_F(DepositTest, SaveToLogTransaction) {
    account->addTransaction(std::make_unique<Deposit>(300.0, "Regalo", "IT1234567"), TEST_TRANSACTIONS);

    std::ifstream file(TEST_TRANSACTIONS);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool found = false;
    // cerca una riga con l'iban e la descrizione
    while (std::getline(file, line)) {
        if (line.find("IT1234567") != std::string::npos &&
            line.find("Regalo") != std::string::npos) {
            found = true;
            break;
        }
    }
    file.close();
    EXPECT_TRUE(found);
}

// controlla che si possa cambiare la descrizione di un deposito
TEST_F(DepositTest, ModifyTransactionDescription) {
    account->addTransaction(std::make_unique<Deposit>(100.0, "Vecchia descrizione", "IT1234567"), TEST_TRANSACTIONS);

    bool modified = account->modifyTransaction(0, "Descrizione aggiornata", TEST_TRANSACTIONS);
    EXPECT_TRUE(modified);

    std::ifstream file(TEST_TRANSACTIONS);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool found = false;
    // cerca nel file la nuova descrizione
    while (std::getline(file, line)) {
        if (line.find("Descrizione aggiornata") != std::string::npos) {
            found = true;
            break;
        }
    }
    file.close();
    EXPECT_TRUE(found);
}
