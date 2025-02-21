#ifndef TRANSACTION_H  // Include guard inizia qui
#define TRANSACTION_H

#include <fstream>
#include <string>

class Account;

class Transaction {
public:
    virtual ~Transaction() {}
    virtual void apply(Account& account) const = 0;
    virtual void save(std::ofstream& file, double currentBalance) const = 0;
    virtual std::string getType() const = 0;
    virtual double getAmount() const = 0;
};

#endif // TRANSACTION_H  // Include guard termina qui
