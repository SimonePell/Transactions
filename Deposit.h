#include "Transaction.h"
using namespace std;

class Deposit : public Transaction {
private:
    double amount;
public:
    Deposit(double amount);
    void apply(Account& account) const override;
    void save(ofstream& file, double currentBalance) const override;
    string getType() const override { return "Deposit"; }
    double getAmount() const override { return amount; }
};
