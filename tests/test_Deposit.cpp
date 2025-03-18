#include <gtest/gtest.h>
#include "Deposit.h"
#include "Account.h"
#include "Persona.h"

//inizializzazion
/*
TEST(DepositTest, DepositInitialization) {
    Deposit deposit(150.0);
    EXPECT_EQ(deposit.getAmount(), 150.0);
    EXPECT_EQ(deposit.getType(), "Deposit");
}

//deposito
TEST(DepositTest, ApplyDeposit) {
    Persona persona("Marco", "Ferrari", "MRCFRR75E05H501Y");
    Account account("IT40D1234567890123456789013", persona, "test_deposit.txt");
    
    Deposit deposit(200.0);
    deposit.apply(account);
    
    EXPECT_EQ(account.getSaldo(), 200.0);
}
    */