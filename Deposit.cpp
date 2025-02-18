#include "Deposit.h"
#include "Account.h"
using namespace std;

Deposit::Deposit(double amt) : amount(amt) {}

void Deposit::apply(Account& account) const {
    account.updateSaldo(amount);
}

void Deposit::save(ofstream& file, double currentBalance) const {
    file << getType() << "," << getAmount() << "," << currentBalance << endl;
}
