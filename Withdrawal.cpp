#include "Withdrawal.h"
#include "Account.h"
using namespace std;

Withdrawal::Withdrawal(double amt) : amount(amt) {}

void Withdrawal::apply(Account& account) const {
    account.updateSaldo(-amount);
}

void Withdrawal::save(std::ofstream& file, double currentBalance) const {
    file << getType() << "," << getAmount() << "," << currentBalance << endl;
}
