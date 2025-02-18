#include <iostream>
#include <string>
#include <map>
#include "Account.h"
#include "Persona.h"
#include "Deposit.h"
#include "Withdrawal.h"

int main() {
    std::map<std::string, Account> accounts;
    std::string iban, nome, cognome, codiceFiscale;
    double amount;
    int choice, transactionChoice;

    while (true) {
        std::cout << "1. Seleziona un account esistente\n2. Crea un nuovo account\n3. Esci\nScegli: ";
        std::cin >> choice;

        if (choice == 3) break;

        switch (choice) {
            case 1: {
                std::cout << "Inserisci l'IBAN dell'account: ";
                std::cin >> iban;
                if (accounts.find(iban) == accounts.end()) {
                    std::cout << "Account non trovato.\n";
                } else {
                    std::cout << "Account selezionato con successo.\n";
                }
                break;
            }
            case 2: {
                std::cout << "Crea un nuovo account.\n";
                std::cout << "Inserisci IBAN: ";
                std::cin >> iban;
                std::cout << "Inserisci Nome: ";
                std::cin >> nome;
                std::cout << "Inserisci Cognome: ";
                std::cin >> cognome;
                std::cout << "Inserisci Codice Fiscale: ";
                std::cin >> codiceFiscale;
                Persona persona(nome, cognome, codiceFiscale);
                accounts[iban] = Account(iban, persona, iban + "_transactions.csv");
                std::cout << "Account creato con successo.\n";
                break;
            }
        }

        if (accounts.find(iban) != accounts.end()) {
            std::cout << "Scegli un tipo di transazione:\n1. Deposito\n2. Prelievo\nInserisci scelta: ";
            std::cin >> transactionChoice;
            std::cout << "Inserisci l'importo: ";
            std::cin >> amount;

            switch (transactionChoice) {
                case 1:
                    accounts[iban].addTransaction(new Deposit(amount));
                    break;
                case 2:
                    accounts[iban].addTransaction(new Withdrawal(amount));
                    break;
            }

            std::cout << "Transazione completata. Saldo attuale: " << accounts[iban].getSaldo() << "\n";
        }
    }

    return 0;
}
