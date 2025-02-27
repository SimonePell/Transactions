#include <gtest/gtest.h>
#include "Withdrawal.h"
#include "Deposit.h"
#include "Account.h"
#include "Persona.h"

//inizializzazione
TEST(WithdrawalTest, WithdrawalInitialization) {
    Withdrawal withdrawal(100.0);
    EXPECT_EQ(withdrawal.getAmount(), 100.0);
    EXPECT_EQ(withdrawal.getType(), "Withdrawal");
}

//prelievo con saldo sufficiente
TEST(WithdrawalTest, ApplyWithdrawalSufficientFunds) {
    Persona persona("Laura", "Bianchi", "LRABNC80H07F205X");
    Account account("IT50E1234567890123456789014", persona, "test_withdrawal.txt");
    
    Deposit deposit(300.0);
    deposit.apply(account);
    Withdrawal withdrawal(150.0);
    withdrawal.apply(account);
    
    EXPECT_EQ(account.getSaldo(), 150.0);
}

//prelievo con saldo insufficiente
TEST(WithdrawalTest, ApplyWithdrawalInsufficientFunds) {
    Persona persona("Giorgio", "Neri", "GRGNER85D08G506Y");
    Account account("IT60F1234567890123456789015", persona, "test_withdrawal.txt");
    
    Withdrawal withdrawal(100.0);
    withdrawal.apply(account);
    
    EXPECT_LT(account.getSaldo(), 0); //dovrebbe essere negativo dato che non ci sono controlli
}
