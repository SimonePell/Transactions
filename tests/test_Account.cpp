#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "Account.h"
#include "Persona.h"
#include "Deposit.h"
#include "Withdrawal.h"

const std::string TEST_FILE = "test_account.txt";
const std::string TEST_TRANSACTIONS = "test_transactions.txt";

class AccountTest : public ::testing::Test {
protected:
    void SetUp() override {
        Persona persona("Mario", "Rossi", "MRORSS80A01");
        account = std::make_unique<Account>("IT1234567", persona, TEST_FILE);

        if (!std::filesystem::exists("TRANSACTION")) {
            std::filesystem::create_directory("TRANSACTION");
        }

        std::ofstream testFile(TEST_TRANSACTIONS, std::ios::trunc);
        testFile.close();
    }

    void TearDown() override {
        std::remove(TEST_FILE.c_str());
        std::remove(TEST_TRANSACTIONS.c_str());
    }

    std::unique_ptr<Account> account;
};

TEST_F(AccountTest, AccountCreation) {
    EXPECT_EQ(account->getIban(), "IT1234567");
    EXPECT_EQ(account->getNome(), "Mario");
    EXPECT_EQ(account->getCognome(), "Rossi");
    EXPECT_EQ(account->getCodicefiscale(), "MRORSS80A01");
    EXPECT_EQ(account->getSaldo(), 0);
}

TEST_F(AccountTest, UpdateSaldo) {
    account->updateSaldo(100.5);
    EXPECT_EQ(account->getSaldo(), 100.5);
    account->updateSaldo(-50.5);
    EXPECT_EQ(account->getSaldo(), 50.0);
}

TEST_F(AccountTest, SaveAndLoadAccount) {
    account->updateSaldo(200.0);
    account->saveToAccountFile();

    Account loadedAccount = Account::loadFromFile(TEST_FILE);
    EXPECT_EQ(loadedAccount.getIban(), "IT1234567");
    EXPECT_EQ(loadedAccount.getSaldo(), 200.0);
}

TEST_F(AccountTest, AddTransactionDeposit) {
    auto deposit = std::make_unique<Deposit>(100.0, "transazione", "IT1234567");
    account->addTransaction(std::move(deposit), TEST_TRANSACTIONS);
    EXPECT_EQ(account->getSaldo(), 100.0);
}

TEST_F(AccountTest, AddTransactionWithdrawal) {
    account->updateSaldo(200.0);
    auto withdrawal = std::make_unique<Withdrawal>(50.0, "transazione", "IT1234567");
    account->addTransaction(std::move(withdrawal), TEST_TRANSACTIONS);
    EXPECT_EQ(account->getSaldo(), 150.0);
}

TEST_F(AccountTest, HasNoTransactionsInitially) {
    EXPECT_FALSE(!account->getTransazioni().empty());
}

TEST_F(AccountTest, DeleteTransactionByIndex) {
    auto deposit = std::make_unique<Deposit>(100.0, "Test deposit", "IT1234567");
    account->addTransaction(std::move(deposit), TEST_TRANSACTIONS);
    account->saveToAccountFile();

    bool deleted = account->deleteTransaction(0, TEST_TRANSACTIONS);
    EXPECT_TRUE(deleted);
    EXPECT_EQ(account->getSaldo(), 0);

    std::ifstream afterFile(TEST_TRANSACTIONS);
    bool fileIsEmpty = afterFile.peek() == std::ifstream::traits_type::eof();
    afterFile.close();
    EXPECT_TRUE(fileIsEmpty);
}

TEST_F(AccountTest, SearchTransaction) {
    account->saveToAccountFile();  // Crea test_account.txt

    // SCRIVI NEL FILE GIUSTO
    std::ofstream file("TRANSACTION/transazioni.txt");
    file << "IT1234567,Deposit,100,2024-03-18 10:00:00,Test deposit,2024-03-18 10:01:00\n";
    file.close();

    Account loaded = Account::loadFromFile(TEST_FILE);
    auto results = loaded.searchTransaction("deposit");
    EXPECT_FALSE(results.empty());
}


TEST_F(AccountTest, ModifyTransactionByIndex) {
    account->saveToAccountFile();  // Crea il file account

    std::ofstream file(TEST_TRANSACTIONS);
    file << "IT1234567,Deposit,100,2024-03-18 10:00:00,Old description,2024-03-18 10:01:00\n";
    file.close();

    Account loaded = Account::loadFromFile(TEST_FILE);
    bool modified = loaded.modifyTransaction(0, "New description", TEST_TRANSACTIONS);
    EXPECT_TRUE(modified);
}
