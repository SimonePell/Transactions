#include "Transaction.h"
using namespace std;

class Withdrawal : public Transaction {
private:
    double amount;
public:
    Withdrawal(double amount);
    void apply(Account& account) const override;
    void save(ofstream& file, double currentBalance) const override;
    std::string getType() const override { return "Withdrawal"; }
    double getAmount() const override { return amount; }
};