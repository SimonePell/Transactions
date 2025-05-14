#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include "Account.h"
#include "Persona.h"
#include "Deposit.h"
#include "Withdrawal.h"

const std::string ACCOUNTS_PATH = "TRANSACTION/Accounts/";

namespace fs = std::filesystem;

void accountMenu(Account &account) {
    int choice, id;
    double amount;
    std::string desc, newDesc;
    Transaction* t;

    while (true) {
        std::cout << "\n--- Menu Account ---\n";
        std::cout << "1. Effettua un deposito\n";
        std::cout << "2. Effettua un prelievo\n";
        std::cout << "3. Visualizza saldo\n"; 
        std::cout << "4. Cerca Transazione\n";
        std::cout << "5. Elimina Transazione\n";
        std::cout << "6. Visualizza Transazioni\n";
        std::cout << "7. Modifica descrizione\n";
        std::cout << "8. Esci dall'account\n";
        std::cout << "Scegli: ";
        std::cin >> choice;

        switch (choice) {
            case 1: 
                std::cout << "Inserisci l'importo da depositare: ";
                std::cin >> amount;
                if(amount <= 0){
                    std::cout << "Errore: importo non valido.\n";
                    break;
                }
                std::cout << "Inserisci la descrizione del deposito: ";
                std::cin.ignore(); 
                std::getline(std::cin, desc);
                account.addTransaction(std::make_unique<Deposit>(amount, desc, account.getIban()));
                std::cout << "Deposito effettuato con successo.\n";
                break;

            case 2:
                std::cout << "Inserisci l'importo da prelevare: ";
                std::cin >> amount;
                if(amount <= 0){
                    std::cout << "Errore: importo non valido.\n";
                    break;
                }
                std::cout <<"Inserire la descrizione del prelievo: ";
                std::cin.ignore();
                std::getline(std::cin, desc);
                account.addTransaction(std::make_unique<Withdrawal>(amount, desc, account.getIban()));
                std::cout << "Prelievo effettuato con successo.\n";
                break;
            
            case 3:
                std::cout << "Il saldo dell'account è: " << account.getSaldo() << "\n";
                break;

            case 4:
                std::cout << "Inserisci una parola/descrizione o una data: ";
                std::cin.ignore();
                std::getline(std::cin, desc);
                {
                    auto results = account.searchTransaction(desc);
                    if (results.empty()) {
                        std::cout << "Nessuna transazione trovata con il criterio: " << desc << "\n";
                    } else {
                        for (size_t i = 0; i < results.size(); ++i) {
                            const auto* t = results[i];
                            std::cout << i << ". " << t->getIban() << ", " << t->getType() << ", "
                                      << t->getAmount() << ", " << t->formatTime(t->getTime()) << ", "
                                      << t->getDescription() << ", " << t->formatTime(t->getLastModified()) << "\n";
                        }
                    }
                }
                break;

            case 5:
                if(!account.hasTransactions()){
                    std::cout << "Errore: Nessuna transazione trovata per questo account.\n";
                    break;
                }   
                std::cout << "Digitare 1 per eliminare per indice, 2 per ricerca: ";
                std::cin >> choice;
                if(choice == 1){
                    std::cout << "Indice da eliminare: ";
                    std::cin >> id;
                    if (account.deleteTransactionByIndex(id)) {
                        std::cout << "Transazione eliminata con successo.\n";
                    } else {
                        std::cout << "Errore: indice non valido.\n";
                    }
                } else if(choice == 2){
                    std::cout << "Query: ";
                    std::cin.ignore();
                    std::getline(std::cin, desc);
                    auto results = account.searchTransaction(desc);
                    if (results.empty()) {
                        std::cout << "Nessuna transazione trovata.\n";
                        break;
                    }
                    for (size_t i = 0; i < results.size(); ++i) {
                        const auto* t = results[i];
                        std::cout << i << ". " << t->getDescription() << "\n";
                    }
                    std::cout << "Indice da eliminare: ";
                    std::cin >> id;
                    if (account.deleteTransactionBySearch(desc, id)) {
                        std::cout << "Transazione eliminata.\n";
                    } else {
                        std::cout << "Errore nella cancellazione.\n";
                    }
                } else {
                    std::cout << "Scelta non valida.\n";
                }
                break;

            case 6:
                if (!account.hasTransactions()) {
                    std::cout << "Nessuna transazione trovata.\n";
                } else {
                    for (size_t i = 0; i < account.getTransazioni().size(); ++i) {
                        const auto& t = account[i];
                        std::cout << i << ". " << t->getIban() << ", " << t->getType() << ", "
                                  << t->getAmount() << ", " << t->formatTime(t->getTime()) << ", "
                                  << t->getDescription() << ", " << t->formatTime(t->getLastModified()) << "\n";
                    }
                }
                break;
            
            case 7:
                std::cout << "Digitare 1 per modificare per indice, 2 per ricerca: ";
                std::cin >> choice;
                if(choice == 1){
                    std::cout << "Indice da modificare: ";
                    std::cin >> id;
                    std::cout << "Nuova descrizione: ";
                    std::cin.ignore();
                    std::getline(std::cin, newDesc);
                    if (account.modifyTransactionByIndex(id, newDesc)) {
                        std::cout << "Modifica effettuata.\n";
                    } else {
                        std::cout << "Errore nella modifica.\n";
                    }
                } else if(choice == 2){
                    std::cout << "Query: ";
                    std::cin.ignore();
                    std::getline(std::cin, desc);
                    auto results = account.searchTransaction(desc);
                    if (results.empty()) {
                        std::cout << "Nessuna transazione trovata.\n";
                        break;
                    }
                    for (size_t i = 0; i < results.size(); ++i) {
                        const auto* t = results[i];
                        std::cout << i << ". " << t->getDescription() << "\n";
                    }
                    std::cout << "Indice da modificare: ";
                    std::cin >> id;
                    std::cout << "Nuova descrizione: ";
                    std::cin.ignore();
                    std::getline(std::cin, newDesc);
                    if (account.modifyTransactionBySearch(desc, id, newDesc)) {
                        std::cout << "Modifica effettuata.\n";
                    } else {
                        std::cout << "Errore nella modifica.\n";
                    }
                } else {
                    std::cout << "Scelta non valida.\n";
                }
                break;
            case 8:
                return;

            default:
                std::cout << "Scelta non valida.\n";
        }
    }
}

int main() {
    std::string iban, nome, cognome, codicefiscale;
    int choice;

    while (true) {
        std::cout << "\n--- Menu Principale ---\n";
        std::cout << "1. Seleziona un account esistente\n";
        std::cout << "2. Crea un nuovo account\n";
        std::cout << "3. Esci\n";
        std::cout << "Scegli: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Inserisci l'IBAN dell'account: ";
                std::cin >> iban;
                {
                    std::string filePath = ACCOUNTS_PATH + iban + "_transactions.csv";
                    if (fs::exists(filePath)) {
                        Account account = Account::loadFromFile(filePath);
                        accountMenu(account);
                    } else {
                        std::cout << "Account non trovato.\n";
                    }
                }
                break;
            case 2: 
                std::cout << "Crea un nuovo account.\n";
                std::cout << "Inserisci IBAN: ";
                std::cin >> iban;
                std::cout << "Inserisci Nome: ";
                std::cin >> nome;
                std::cout << "Inserisci Cognome: ";
                std::cin >> cognome;
                std::cout << "Inserisci Codice Fiscale: ";
                std::cin >> codicefiscale;
                {
                    Persona persona(nome, cognome, codicefiscale);
                    Account account(iban, persona, ACCOUNTS_PATH + iban + "_transactions.csv");
                    account.saveToAccountFile();
                    std::cout << "Account creato con successo.\n";
                    accountMenu(account);
                }
                break;
            case 3:
                std::cout << "Chiusura dell'applicazione.\n";
                return 0;
            default:
                std::cout << "Scelta non valida.\n";
        }
    }
}
