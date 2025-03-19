#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

#include "Transaction.h"
#include "Account.h"
#include <string>
#include <ctime>
#include <memory>

class Withdrawal : public Transaction {
public:
    //nuova transazione
    explicit Withdrawal(double amount, std::string description, std::string iban);

    //transazione da caricare
    explicit Withdrawal(double amount, std::string description, std::time_t timeStamp, std::time_t lastMod, std::string iban);

    //funzioni utilizzate per una nuova transazione
    void apply(Account& account, const std::string& filePath = "TRANSACTION/transazioni.txt") const override;
    void saveToAccountFile(const std::string& filePath, double currentBalance) const override;
    void saveToLogTransaction(const std::string& filePath, const std::string& iban )const;

    //funzioni utilizzate per modificare una transazione
    bool modifyDescription(const std::string& newDescription, const std::string& filePath = "TRANSACTION/transazioni.txt") override;
    void updateLogTransaction(const std::string& filePath, const Transaction& updatedTransaction);

    std::string getType() const override { return "Withdrawal"; }
    double getAmount() const override { return amount; }

    std::unique_ptr<Transaction> clone() const override {return std::make_unique<Withdrawal>(*this);  /*Deep copy*/ }
    std::string formatTime(std::time_t rawTime) const override {
        char buffer[20];
        struct tm* timeinfo = localtime(&rawTime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return std::string(buffer);
    }
};

#endif 
