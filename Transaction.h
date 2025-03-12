#ifndef TRANSACTION_H  
#define TRANSACTION_H

#include <fstream>
#include <string>
#include <ctime>

class Account;

class Transaction {
protected:
    double amount;
    std::string description;
    std::time_t time;
public:
    Transaction(double amt, std::string desc = "No Description"): amount(amt), description(std::move(desc)), time(std::time(nullptr)) {} 
    virtual ~Transaction() {}
    virtual void apply(Account& account) const = 0;
    virtual void save(const std::string& file, double currentBalance) const = 0;
    virtual std::string getType() const = 0;
    virtual double getAmount() const = 0;
    std::time_t getTime() const { return time; }
    std::string getDescription() const { return description; }
    virtual void logTransaction(const std::string& filePath, const std::string& iban) const = 0;
    virtual void setAmount(double newAmount) = 0;
    virtual void setDescription(const std::string& newDescription) = 0;

};

#endif 
