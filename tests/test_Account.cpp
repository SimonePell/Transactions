#include <gtest/gtest.h>
#include "Account.h"
#include "Persona.h"
#include "Deposit.h"
#include "Withdrawal.h"

//inizializzazione
TEST(AccountTest, AccountInitialization) {
    Persona persona("Mario", "Rossi", "mrorss");
    Account account("123", persona, "123_transactions.csv");
    
    EXPECT_EQ(account.getIban(), "123");
    EXPECT_EQ(account.getNome(), "Mario");
    EXPECT_EQ(account.getCognome(), "Rossi");
    EXPECT_EQ(account.getCodicefiscale(), "mrorss");
    EXPECT_EQ(account.getSaldo(), 0);
}   

// depositi
TEST(AccountTest, DepositTransaction) {
    Persona persona("Mario", "Rossi", "mrorss");
    Account account("123", persona, "123_transactions.csv");
    
    Deposit deposit(100.0);
    account.addTransaction(&deposit);
    
    EXPECT_EQ(account.getSaldo(), 100.0);
}

// prelievi
TEST(AccountTest, WithdrawalTransaction) {
    Persona persona("Mario", "Rossi", "mrorss");
    Account account("123", persona, "123_transactions.csv");
    
    Deposit deposit(200.0);
    account.addTransaction(&deposit);
    Withdrawal withdrawal(50.0);
    account.addTransaction(&withdrawal);
    
    EXPECT_EQ(account.getSaldo(), 150.0);
}

// prelievi con saldo insufficiente
TEST(AccountTest, WithdrawalInsufficientFunds) {
    Persona persona("Mario", "Rossi", "mrorss");
    Account account("123", persona, "123_transactions.csv");
    
    Withdrawal withdrawal(50.0);
    account.addTransaction(&withdrawal);
    
    EXPECT_LT(account.getSaldo(), 0);  // il saldo dovrebbe andare in negativo dato che non ci sono controlli
}
