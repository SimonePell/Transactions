#include "Persona.h"
#include "Transaction.h"
#include <string>
#include <vector>
#include <memory>
using namespace std;

class Account{
private:
    string iban;
    Persona intestatario;
    string fileRiferimento;
    vector<unique_ptr<Transaction>> transazioni;
    double saldo;
public:
    Account(const string& iban, const Persona& intestatario, const string& fileRiferimento);
    void addTransaction(Transaction* transaction);
    void updateSaldo(double amount);
    void saveToFile()const;
    void createFile()const;
    double getSaldo()const;
};