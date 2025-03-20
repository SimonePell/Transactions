#include <gtest/gtest.h>
#include "Deposit.h"
#include "Account.h"
#include "Persona.h"
#include <fstream>
#include <filesystem>
#include <memory>

const std::string TEST_FILE = "test_account.txt";
const std::string TEST_TRANSACTIONS = "test_transactions.txt";

class DepositTest : public ::testing::Test {
protected:
    void SetUp() override {
        Persona persona("Mario", "Rossi", "MRORSS80A01");
        account = std::make_unique<Account>("IT1234567", persona, TEST_FILE);
        if (!std::filesystem::exists("TRANSACTION")) {
            std::filesystem::create_directory("TRANSACTION");
        }
    
        std::ofstream testFile(TEST_TRANSACTIONS, std::ios::app);
        if (!testFile) {
            throw std::runtime_error("Errore: impossibile creare il file di test per le transazioni.");
        }
        testFile.close();
    }

    void TearDown() override {
        std::remove(TEST_FILE.c_str());
        std::remove(TEST_TRANSACTIONS.c_str());
    }

    std::unique_ptr<Account> account;
};

TEST_F(DepositTest, DepositCreation) {
    Deposit deposit(100.0, "Stipendio", "IT1234567");
    EXPECT_EQ(deposit.getAmount(), 100.0);
    EXPECT_EQ(deposit.getDescription(), "Stipendio");
    EXPECT_EQ(deposit.getIban(), "IT1234567");
    EXPECT_EQ(deposit.getType(), "Deposit");
}

TEST_F(DepositTest, ApplyDeposit) {
    Deposit deposit(200.0, "Bonus", "IT1234567");
    deposit.apply(*account);
    EXPECT_EQ(account->getSaldo(), 200.0);
}

TEST_F(DepositTest, SaveToAccountFile) {
    Deposit deposit(150.0, "regalo", "IT1234567");
    deposit.apply(*account);
    std::ifstream file(TEST_FILE);
    ASSERT_TRUE(file.is_open());
    double storedBalance;
    file >> storedBalance;
    file.close();
    EXPECT_EQ(storedBalance, 150.0);
}

TEST_F(DepositTest, SaveToLogTransaction) {
    account->updateSaldo(500.0);
    Deposit deposit(200.0, "Affitto", "IT1234567");
    deposit.apply(*account, TEST_TRANSACTIONS);
    
    std::ifstream file(TEST_TRANSACTIONS);
    ASSERT_TRUE(file.is_open());
    std::string line;
    bool found = false;
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

TEST_F(DepositTest, ModifyTransactionDescription) {
    Deposit deposit(300.0, "Deposito iniziale", "IT1234567");
    deposit.apply(*account, TEST_TRANSACTIONS);
    deposit.modifyDescription("Descrizione aggiornata", TEST_TRANSACTIONS);

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

