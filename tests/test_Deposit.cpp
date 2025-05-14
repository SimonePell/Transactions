#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "Deposit.h"
#include "Account.h"
#include "Persona.h"

const std::string TEST_FILE = "test_account.txt";
const std::string TEST_TRANSACTIONS = "test_transactions.txt";

class DepositTest : public ::testing::Test {
protected:
    void SetUp() override {
        Persona persona("Mario", "Rossi", "MRORSS80A01");
        account = std::make_unique<Account>("IT1234567", persona, TEST_FILE);

        std::ofstream clearFile(TEST_TRANSACTIONS, std::ios::trunc); // Pulisce il file
        clearFile.close();

        if (!std::filesystem::exists("TRANSACTION")) {
            std::filesystem::create_directory("TRANSACTION");
        }
    }

    void TearDown() override {
        std::remove(TEST_FILE.c_str());
        std::remove(TEST_TRANSACTIONS.c_str());
    }

    std::unique_ptr<Account> account;
};

// Verifica i valori di una nuova transazione Deposit
TEST_F(DepositTest, DepositCreation) {
    Deposit deposit(150.0, "Stipendio", "IT1234567");
    EXPECT_EQ(deposit.getAmount(), 150.0);
    EXPECT_EQ(deposit.getDescription(), "Stipendio");
    EXPECT_EQ(deposit.getIban(), "IT1234567");
    EXPECT_EQ(deposit.getType(), "Deposit");
}

// Verifica che l'applicazione del deposito aggiorni correttamente il saldo
TEST_F(DepositTest, ApplyDepositThroughAccount) {
    account->addTransaction(std::make_unique<Deposit>(200.0, "Bonifico", "IT1234567"), TEST_TRANSACTIONS);
    EXPECT_EQ(account->getSaldo(), 200.0);
}

// Verifica che il deposito venga salvato nel file di log
TEST_F(DepositTest, SaveToLogTransaction) {
    account->addTransaction(std::make_unique<Deposit>(300.0, "Regalo", "IT1234567"), TEST_TRANSACTIONS);

    std::ifstream file(TEST_TRANSACTIONS);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool found = false;
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

// Verifica che si possa aggiornare la descrizione di un deposito
TEST_F(DepositTest, ModifyTransactionDescription) {
    account->addTransaction(std::make_unique<Deposit>(100.0, "Vecchia descrizione", "IT1234567"), TEST_TRANSACTIONS);

    bool modified = account->modifyTransaction(0, "Descrizione aggiornata", TEST_TRANSACTIONS);
    EXPECT_TRUE(modified);

    std::ifstream file(TEST_TRANSACTIONS);
    ASSERT_TRUE(file.is_open());

    std::string line;
    bool found = false;
    while (std::getline(file, line)) {
        if (line.find("Descrizione aggiornata") != std::string::npos) {
            found = true;
            break;
        }
    }
    file.close();
    EXPECT_TRUE(found);
}
