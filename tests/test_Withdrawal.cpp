#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "Withdrawal.h"
#include "Account.h"
#include "Persona.h"
#include <fstream>
#include <memory>

const std::string TEST_FILE = "test_account.txt";
const std::string TEST_TRANSACTIONS = "test_transactions.txt";

class WithdrawalTest : public ::testing::Test {
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

TEST_F(WithdrawalTest, WithdrawalCreation) {
    Withdrawal withdrawal(50.0, "Shopping", "IT1234567");
    EXPECT_EQ(withdrawal.getAmount(), 50.0);
    EXPECT_EQ(withdrawal.getDescription(), "Shopping");
    EXPECT_EQ(withdrawal.getIban(), "IT1234567");
    EXPECT_EQ(withdrawal.getType(), "Withdrawal");
}

TEST_F(WithdrawalTest, ApplyWithdrawal) {
    account->updateSaldo(200.0);
    Withdrawal withdrawal(50.0, "Cibo", "IT1234567");
    withdrawal.apply(*account);
    EXPECT_EQ(account->getSaldo(), 150.0);
}

TEST_F(WithdrawalTest, SaveToAccountFile) {
    account->updateSaldo(300.0);
    Withdrawal withdrawal(75.0, "Cibo", "IT1234567");
    withdrawal.apply(*account);
    std::ifstream file(TEST_FILE);
    ASSERT_TRUE(file.is_open());
    double storedBalance;
    file >> storedBalance;
    file.close();
    EXPECT_EQ(storedBalance, 225.0);
}

TEST_F(WithdrawalTest, SaveToLogTransaction) {
    account->updateSaldo(500.0);
    Withdrawal withdrawal(200.0, "Affitto", "IT1234567");
    withdrawal.apply(*account, TEST_TRANSACTIONS);
    
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

TEST_F(WithdrawalTest, ModifyTransactionDescription) {
    Withdrawal withdrawal(300.0, "Deposito iniziale", "IT1234567");
    withdrawal.apply(*account, TEST_TRANSACTIONS);
    withdrawal.modifyDescription("Descrizione aggiornata", TEST_TRANSACTIONS);

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


