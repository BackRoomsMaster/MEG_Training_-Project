#include <iostream>
#include <string>
#include <limits>
#include <chrono>
#include <thread>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showLoadingAnimation(const std::string& message, int duration) {
    const char animation[] = {'|', '/', '-', '\\'};
    for (int i = 0; i < duration; ++i) {
        std::cout << "\r" << message << " " << animation[i % 4] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "\r" << message << " Completato!" << std::endl;
}

void executeCommand(const std::string& command) {
    system(command.c_str());
}

void pauseExecution() {
    std::cout << "\nPremi Invio per continuare...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

bool askToRun() {
    char response;
    std::cout << "Vuoi avviare il programma? (s/n): ";
    std::cin >> response;
    return (response == 's' || response == 'S');
}

void showMenu() {
    clearScreen();
    std::cout << "=== Menu di Installazione ===" << std::endl;
    std::cout << "1. Compila e opzionalmente avvia meg_terminal" << std::endl;
    std::cout << "2. Compila e opzionalmente avvia MEG_Training" << std::endl;
    std::cout << "3. Esci" << std::endl;
    std::cout << "Scelta: ";
}

int main() {
    int choice;
    
    do {
        showMenu();
        std::cin >> choice;
        
        switch(choice) {
            case 1:
                clearScreen();
                std::cout << "Compilazione di meg_terminal..." << std::endl;
                showLoadingAnimation("Compilazione in corso", 30);
                executeCommand("g++ -std=c++11 meg_terminal.cpp -o meg_terminal");
                if (askToRun()) {
                    std::cout << "Avvio di meg_terminal..." << std::endl;
                    showLoadingAnimation("Avvio in corso", 20);
                    executeCommand("./meg_terminal");
                }
                pauseExecution();
                break;
            case 2:
                clearScreen();
                std::cout << "Compilazione di MEG_Training..." << std::endl;
                showLoadingAnimation("Compilazione in corso", 30);
                executeCommand("g++ -std=c++11 MEG_Training.cpp -o MEG_Training");
                if (askToRun()) {
                    std::cout << "Avvio di MEG_Training..." << std::endl;
                    showLoadingAnimation("Avvio in corso", 20);
                    executeCommand("./MEG_Training");
                }
                pauseExecution();
                break;
            case 3:
                std::cout << "Uscita dal programma..." << std::endl;
                break;
            default:
                std::cout << "Scelta non valida. Riprova." << std::endl;
                pauseExecution();
        }
    } while (choice != 3);

    return 0;
}

