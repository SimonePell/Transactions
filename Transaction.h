#ifndef TRANSACTION_H  
#define TRANSACTION_H

#include <fstream>
#include <string>
#include <ctime>
#include <memory>

class Account;

class Transaction {
protected:
    double amount;
    std::string description;
    std::time_t time;
    std::time_t lastModified;
    std::string iban;

public:
    //costruttore nuova transazione
    Transaction(double amt, std::string desc, std::string accountIban)
        : amount(amt), description(std::move(desc)), iban(std::move(accountIban)), time(std::time(nullptr)), lastModified(std::time(nullptr)) {}

    //costruttore per una transazione già presente in un file 
    Transaction(double amt, std::string desc, std::time_t timeStamp, std::time_t lastMod, std::string accountIban)
        : amount(amt), description(std::move(desc)), time(timeStamp), lastModified(lastMod), iban(std::move(accountIban)) {}

    virtual ~Transaction() {}
    
    virtual void apply(Account& account, const std::string& filePath) const = 0;
    virtual void saveToAccountFile(const std::string& file, double currentBalance) const = 0;

    virtual double getAmount() const = 0;
    virtual std::string getType() const = 0;
    std::string getDescription() const { return description; }
    std::time_t getTime() const { return time; }
    std::time_t getLastModified() const { return lastModified; }
    std::string getIban() const {return iban;}

    virtual void saveToLogTransaction(const std::string& filePath, const std::string& iban) const = 0;
    virtual void updateLogTransaction(const std::string& filePath, const Transaction& updatedTransaction) = 0;

    virtual bool modifyDescription(const std::string& newDescription, const std::string& filePath) = 0;

    virtual std::unique_ptr<Transaction> clone() const = 0;
    virtual std::string formatTime(std::time_t rawTime) const = 0;
};

#endif 
