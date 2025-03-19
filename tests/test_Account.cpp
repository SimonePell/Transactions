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

    // Ensure transaction file is completely empty
    std::ofstream testFile(TEST_TRANSACTIONS, std::ios::trunc);
    testFile.close();

    // Verify file is empty
    std::ifstream verifyFile(TEST_TRANSACTIONS);
    EXPECT_TRUE(verifyFile.peek() == std::ifstream::traits_type::eof()) << "ERROR: Transactions file not empty!";
    verifyFile.close();
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
    std::unique_ptr<Transaction> deposit = std::make_unique<Deposit>(100.0, "transazione", "IT1234567");
    account->addTransaction(std::move(deposit));
    EXPECT_EQ(account->getSaldo(), 100.0);
}
TEST_F(AccountTest, AddTransactionWithdrawal) {
    account->updateSaldo(200.0);
    std::unique_ptr<Transaction> withdrawal = std::make_unique<Withdrawal>(50.0, "transazione", "IT1234567");
    account->addTransaction(std::move(withdrawal));
    EXPECT_EQ(account->getSaldo(), 150.0);
}

TEST_F(AccountTest, HasNoTransactionsInitially) {
    EXPECT_FALSE(account->hasTransactions(TEST_TRANSACTIONS));
}


TEST_F(AccountTest, DeleteTransactionByIndex) {
    // Step 1: Add a transaction
    std::unique_ptr<Transaction> deposit = std::make_unique<Deposit>(100.0, "Test deposit", "IT1234567");
    account->addTransaction(std::move(deposit), TEST_TRANSACTIONS);
    account->saveToAccountFile();

    // Step 3: Delete the transaction
    account->deleteTransactionByIndex(0, TEST_TRANSACTIONS);

    // Step 4: Verify account balance is updated
    std::cout << "DEBUG: Balance after deletion: " << account->getSaldo() << std::endl;
    EXPECT_EQ(account->getSaldo(), 0) << "ERROR: Balance not updated after deletion!";

    // Step 5: Verify transaction file is empty after deletion
    std::ifstream afterFile(TEST_TRANSACTIONS);
    bool fileIsEmpty = afterFile.peek() == std::ifstream::traits_type::eof();
    afterFile.close();
    
    ASSERT_TRUE(fileIsEmpty) << "ERROR: Transactions file is not empty after deletion!";
}


TEST_F(AccountTest, SearchTransaction) {
    std::ofstream file(TEST_TRANSACTIONS);
    file << "IT1234567,Deposit,100,2024-03-18 10:00:00,Test deposit,2024-03-18 10:01:00\n";
    file.close();
    account->searchTransaction("deposit"); // Should print a result
}

TEST_F(AccountTest, ModifyTransactionByIndex) {
    std::ofstream file(TEST_TRANSACTIONS);
    file << "IT1234567,Deposit,100,2024-03-18 10:00:00,Old description,2024-03-18 10:01:00\n";
    file.close();
    account->modifyTransactionByIndex(0);
    // Normally we'd mock cin, but this requires interactive input.
}