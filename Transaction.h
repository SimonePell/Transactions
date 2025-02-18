#include <fstream>
#include <string>
using namespace std;

class Account;

class Transaction {
public:
    virtual ~Transaction() {}
    virtual void apply(Account& account) const = 0;
    virtual void save(ofstream& file, double currentBalance) const = 0;
    virtual string getType() const = 0;
    virtual double getAmount() const = 0;
};