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
        std::cout << "7. modifica descrizione\n";
        std::cout << "8. Esci dall'account\n";
        std::cout << "Scegli: ";
        std::cin >> choice;

        switch (choice) {
            case 1: 
                std::cout << "Inserisci l'importo da depositare: ";
                std::cin >> amount;
                if(amount <= 0){
                    std::cout << "Errore: importo non valido.\n";
                    std::cout << "Deposito non effettuato.\n";
                    break;
                }
                std::cout << "Inserisci la descrizione del deposito: ";
                std::cin.ignore(); 
                std::getline(std::cin, desc);
                account.addTransaction(std::make_unique<Deposit>(amount, desc, account.getIban())); // Use make_unique
                std::cout << "Deposito effettuato con successo.\n";
                break;

            case 2:
                std::cout << "Inserisci l'importo da prelevare: ";
                std::cin >> amount;
                if(amount <= 0){
                    std::cout << "Errore: importo non valido.\n";
                    std::cout << "Prelievo non effettuato.\n";
                    break;
                }
                std::cout <<"Inserire la descrizone del prelievo: ";
                std::cin.ignore();
                std::getline(std::cin, desc);
                account.addTransaction(std::make_unique<Withdrawal>(amount, desc, account.getIban()));
                std::cout << "Prelievo effettuato con successo.\n";
                break;
            
            case 3:
                std::cout << "Il saldo dell'account è: " << account.getSaldo();
                break;

            case 4:
                std::cout << "Inserisci una parola/descrizione o una data: ";
                std::cin.ignore();
                std::getline(std::cin, desc);
                account.searchTransaction(desc);
                break;

            case 5:
                if(account.hasTransactions() == false){
                    std::cout << "Errore: Nessuna transazione trovata per questo account.\n";
                    break;
                }   
                std::cout << "Digitare 1 per eliminare una transazione per indice, 2 per eliminare una transazione per descrizione o data: ";
                std::cin >> choice;
                if(choice == 1){
                    std::cout << "Inserisci l'indice della transazione da eliminare: ";
                    std::cin >> id;
                    account.deleteTransactionByIndex(id);
                }else if(choice == 2){
                    std::cout << "Inserisci la descrizione o data della transazione da eliminare: ";
                    std::cin.ignore();
                    std::getline(std::cin, desc);
                    account.deleteTransactionBySearch(desc);
                }else{
                    std::cout << "Scelta non valida.\n";
                }
                break;

            case 6:
                account.printTransactions();
                break;
            
            case 7:
                std::cout << "Digitare 1 per modificare una transazione per indice, 2 per modificare una transazione per descrizione o data: ";
                std::cin >> choice;
                if(choice == 1){
                    std::cout << "Inserisci l'indice della transazione da modificare: ";
                    std::cin >> id;
                    account.modifyTransactionByIndex(id);
                }else if(choice == 2){
                    std::cout << "Inserisci la descrizione o data della transazione da modificare: ";
                    std::cin.ignore();
                    std::getline(std::cin, desc);
                    account.modifyTransactionBySearch(desc);
                }else{
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
                    Account account(iban, persona, "TRANSACTION/Accounts/" + iban + "_transactions.csv");
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
