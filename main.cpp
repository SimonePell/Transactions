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
const std::string TRANSACTIONS_PATH = "TRANSACTION/transazioni.txt";

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
        std::cout << "4. Modifica Transazione\n";
        std::cout << "5. Elimina Transazione\n";
        std::cout << "6. Visualizza Transazioni\n";
        std::cout << "7. Esci dall'account\n";
        std::cout << "Scegli: ";
        std::cin >> choice;

        switch (choice) {
            case 1: 
                std::cout << "Inserisci l'importo da depositare: ";
                std::cin >> amount;
                if(amount < 0){
                    std::cout << "Errore: importo non valido.\n";
                    std::cout << "Deposito non effettuato.\n";
                    break;
                }
                if(amount == 0){
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
                if (account.getSaldo() >= amount) {
                    std::cout << "Inserisci la descrizione del prelievo: ";
                    std::cin.ignore();  
                    std::getline(std::cin, desc);
                    account.addTransaction(std::make_unique<Withdrawal>(amount, desc, account.getIban())); // Use make_unique
                    std::cout << "Prelievo effettuato con successo.\n";
                } else {
                    std::cout << "Soldi insufficienti.\n";
                }
                break;

            case 3:
                std::cout << "Saldo attuale: " << account.getSaldo() << "\n";
                break;

            /*case 4:
                std::cout << "Transazioni eliminabili: \n";
                account.printTransactions();
                std::cout << "Inserisci l'id della transazione da eliminare: ";
                std::cin >> id;
                account.deleteTransaction(id);
                break;*/

            case 5:
                if(account.hasTransactions() == false){
                    std::cout << "Errore: Nessuna transazione trovata per questo account.\n";
                    break;
                }   
                account.printTransactions();

                int index;
                std::cout << "Inserisci l'indice della transazione da eliminare: ";
                
                while (true) {
                    std::cin >> index;
                
                    // Check if input is a valid integer and greater than or equal to 0
                    if (std::cin.fail() || index < 0) {
                        std::cin.clear(); // Clear error state
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                        std::cout << "Errore: Inserisci un numero intero valido (>= 0): ";
                    } else {
                        break; // Valid input, exit loop
                    }
                }
                
                try {
                    account.deleteTransactionByIndex(index);
                } catch (const std::exception& e) {
                    std::cerr << "Errore: " << e.what() << std::endl;
                }
                break;

            case 6:
                account.printTransactions();
                break;

            case 7:
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
