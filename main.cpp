#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <filesystem>
#include "Account.h"
#include "Persona.h"
#include "Deposit.h"
#include "Withdrawal.h"


namespace fs = std::filesystem;

Account loadAccountFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string iban, nome, cognome, codicefiscale;

    double saldo;
    if (file.is_open()) {
        std::getline(file, iban);
        std::getline(file, nome);
        std::getline(file, cognome);
        std::getline(file, codicefiscale);
        file >> saldo;
        file.ignore();

        Persona persona(nome, cognome, codicefiscale);
        Account account(iban, persona, filePath);
        account.updateSaldo(saldo);

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string type;
            double amount;

            std::getline(ss, type, ',');
            ss >> amount;
        }

        file.close();
        return account;
    } else {
        throw std::runtime_error("Errore durante la lettura del file");
    }
}

void saveAccountToFile(const Account& account) {
    std::string directory = "TRANSACTION/Accounts/";

    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    std::ofstream file(account.getFileRiferimento(), std::ios::trunc); // Sovrascrive tutto
    if (file.is_open()) {
        file << account.getIban() << "\n"
             << account.getNome() << "\n"
             << account.getCognome() << "\n"
             << account.getCodicefiscale() << "\n"
             << account.getSaldo() << "\n";

        std::ifstream fileInput(account.getFileRiferimento());
        if (fileInput.is_open()) {
            std::string line;

            // Salta le prime 5 righe (dati dell'account)
            for (int i = 0; i < 5 && std::getline(fileInput, line); ++i) {}

            // Scrivi tutte le transazioni nel file di output
            while (std::getline(fileInput, line)) {
                file << line << "\n";
            }

            fileInput.close();
        }
        file.close();
    } else {
        throw std::runtime_error("c'è stato un errore nell'apertura del file");
    }
}




void accountMenu(Account &account) {
    int choice;
    double amount;

    while (true) {
        std::cout << "\n--- Menu Account ---\n";
        std::cout << "1. Effettua un deposito\n";
        std::cout << "2. Effettua un prelievo\n";
        std::cout << "3. Visualizza saldo\n"; 
        std::cout << "4. Esci dall'account\n";
        std::cout << "Scegli: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Inserisci l'importo da depositare: ";
                std::cin >> amount;
                account.addTransaction(new Deposit(amount));
                std::cout << "Deposito effettuato con successo.\n";
                break;

            case 2:
                std::cout << "Inserisci l'importo da prelevare: ";
                std::cin >> amount;
                if (account.getSaldo() >= amount) {
                    account.addTransaction(new Withdrawal(amount));
                    std::cout << "Prelievo effettuato con successo.\n";
                } else {
                    std::cout << "Soldi insufficienti.\n";
                }
                break;

            case 3:
                std::cout << "Saldo attuale: " << account.getSaldo() << "\n";
                break;

            case 4:
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
                    std::string directory = "TRANSACTION/Accounts/";
                    bool found = false;
                    if (!fs::exists(directory)) {fs::create_directories(directory);}
                    std::string filePath = directory + iban + "_transactions.csv";
                    if (fs::exists(filePath)) {
                        Account account = loadAccountFromFile(filePath);
                        accountMenu(account);
                        found = true;
                    }
                    if (!found) {std::cout << "Account non trovato.\n";}
                }
                break;
            case 2: 
            {
                std::cout << "Crea un nuovo account.\n";
                std::cout << "Inserisci IBAN: ";
                std::cin >> iban;
                std::cout << "Inserisci Nome: ";
                std::cin >> nome;
                std::cout << "Inserisci Cognome: ";
                std::cin >> cognome;
                std::cout << "Inserisci Codice Fiscale: ";
                std::cin >> codicefiscale;
                Persona persona(nome, cognome, codicefiscale);
                Account account(iban, persona, "TRANSACTION/Accounts/" + iban + "_transactions.csv");
                saveAccountToFile(account);
                std::cout << "Account creato con successo.\n";
                accountMenu(account);
                break;

            }
            case 3:
                std::cout << "Chiusura dell'applicazione.\n";
                return 0;
            default:
                std::cout << "Scelta non valida.\n";
        }
    }
}
