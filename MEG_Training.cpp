#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <ctime>
#include <limits>
#include <sstream>
#include <map>
#include <algorithm>
#include <functional>

class Entity {
public:
    std::string name;
    std::string description;
    int dangerLevel;

    Entity(std::string n, std::string desc, int danger) 
        : name(n), description(desc), dangerLevel(danger) {}
};

class Item {
public:
    std::string name;
    std::string description;
    std::map<std::string, int> effects;

    Item(std::string n, std::string desc, std::map<std::string, int> eff) 
        : name(n), description(desc), effects(eff) {}
};

class Room {
public:
    std::string description;
    bool hasExit;
    std::vector<std::string> features;
    Entity* entity;
    Item* item;
    
    Room(std::string desc, bool exit, std::vector<std::string> feat, Entity* ent, Item* it) 
        : description(desc), hasExit(exit), features(feat), entity(ent), item(it) {}
};

class Player {
public:
    int sanity;
    int hydration;
    int energy;
    std::vector<Item*> inventory;
    
    Player() : sanity(100), hydration(100), energy(100) {}
};

class Event {
public:
    std::string description;
    std::vector<std::string> choices;
    std::function<void(Player&)> outcome;

    Event(std::string desc, std::vector<std::string> ch, std::function<void(Player&)> out)
        : description(desc), choices(ch), outcome(out) {}
};

class CrazyTraveler {
public:
    std::string name;
    std::string dialogue;
    std::vector<std::string> responses;
    std::function<void(Player&, int)> outcome;

    CrazyTraveler(std::string n, std::string dial, std::vector<std::string> resp, std::function<void(Player&, int)> out)
        : name(n), dialogue(dial), responses(resp), outcome(out) {}
};

class Merchant {
public:
    std::string name;
    std::vector<Item> inventory;

    Merchant(std::string n, std::vector<Item> inv) : name(n), inventory(inv) {}
};

std::random_device rd;
std::mt19937 gen(rd());
std::vector<std::string> roomTypes = {
    "stanza", "corridoio", "sala", "angolo", "incrocio"
};

std::vector<std::string> wallColors = {
    "color crema", "giallo pallido", "beige", "grigio chiaro"
};

std::vector<std::string> floorTypes = {
    "moquette gialla", "linoleum consumato", "piastrelle sporche", "pavimento in cemento"
};

std::vector<std::string> lightTypes = {
    "luci fluorescenti ronzanti", "lampadine tremolanti", "luci al neon fioche", "illuminazione intermittente"
};

std::vector<std::string> roomFeatures = {
    "una scrivania vuota", "una sedia rovesciata", "un poster sbiadito", "un telefono scollegato",
    "un orologio fermo", "una pianta appassita", "un quadro storto", "una finestra oscurata",
    "un ventilatore rotto", "un calendario vecchio", "una macchia sul muro", "un graffito incomprensibile"
};

std::vector<Entity> entities = {
    Entity("Hound", "Una creatura simile a un cane con la pelle rugosa e senza pelo.", 3),
    Entity("Skin-Stealer", "Un umanoide con la pelle che sembra una maschera mal indossata.", 4),
    Entity("Smiler", "Una faccia sorridente che appare nell'oscurità, con denti innaturalmente bianchi.", 5),
    Entity("Clump", "Una massa informe di carne e oggetti che si muove lentamente.", 2)
};

std::vector<Item> items = {
    Item("Bottiglia d'acqua di mandorle", "Una bottiglia contenente il famoso liquido delle Backrooms.", {{"hydration", 30}, {"sanity", -5}}),
    Item("Barretta energetica", "Una barretta energetica un po' ammuffita.", {{"energy", 20}}),
    Item("Kit di pronto soccorso", "Un piccolo kit di pronto soccorso.", {{"sanity", 15}, {"energy", 10}}),
    Item("Torcia", "Una torcia con batterie quasi scariche.", {{"sanity", 5}}),
    Item("Amuleto misterioso", "Un amuleto che sembra vibrare leggermente.", {{"sanity", 10}})
};
std::vector<Event> events = {
    Event(
        "Trovi una porta misteriosa con strani simboli. Cosa fai?",
        {"Apri la porta", "Ignora la porta"},
        [](Player& p) {
            if (rand() % 2 == 0) {
                std::cout << "La porta si apre su un vuoto nero. Perdi 10 punti sanità.\n";
                p.sanity -= 10;
            } else {
                std::cout << "Trovi una stanza sicura. Recuperi 15 punti energia.\n";
                p.energy = std::min(100, p.energy + 15);
            }
        }
    ),
    Event(
        "Senti un pianto in lontananza. Come reagisci?",
        {"Segui il suono", "Ignora e continua"},
        [](Player& p) {
            if (rand() % 2 == 0) {
                std::cout << "Trovi un altro viaggiatore ferito. Ti aiuta a orientarti. Guadagni 10 punti sanità.\n";
                p.sanity = std::min(100, p.sanity + 10);
            } else {
                std::cout << "Il pianto si trasforma in una risata maniacale. Perdi 15 punti sanità.\n";
                p.sanity -= 15;
            }
        }
    ),
    Event(
        "Le pareti iniziano a pulsare e a cambiare colore. Cosa fai?",
        {"Chiudi gli occhi e aspetta che passi", "Tocca le pareti", "Corri via"},
        [](Player& p) {
            std::uniform_int_distribution<> dis(1, 3);
            int result = dis(gen);
            switch(result) {
                case 1:
                    std::cout << "Riapri gli occhi e tutto sembra normale. Perdi 5 punti sanità.\n";
                    p.sanity -= 5;
                    break;
                case 2:
                    std::cout << "Le pareti sono calde e viscide. Perdi 20 punti sanità.\n";
                    p.sanity -= 20;
                    break;
                case 3:
                    std::cout << "Corri finché non sei esausto. Perdi 15 punti energia.\n";
                    p.energy -= 15;
                    break;
            }
        }
    ),
    Event(
        "Vedi il tuo riflesso in una pozza d'acqua, ma non sei tu. Il riflesso ti sorride.",
        {"Sorridi di rimando", "Disturba la superficie dell'acqua", "Scappa"},
        [](Player& p) {
            std::uniform_int_distribution<> dis(1, 3);
            int result = dis(gen);
            switch(result) {
                case 1:
                    std::cout << "Il riflesso sembra soddisfatto. Guadagni 10 punti sanità.\n";
                    p.sanity = std::min(100, p.sanity + 10);
                    break;
                case 2:
                    std::cout << "Senti un urlo assordante. Perdi 25 punti sanità.\n";
                    p.sanity -= 25;
                    break;
                case 3:
                    std::cout << "Corri via, ma senti gli occhi del riflesso su di te. Perdi 15 punti sanità.\n";
                    p.sanity -= 15;
                    break;
            }
        }
    )
};
std::vector<CrazyTraveler> crazyTravelers = {
    CrazyTraveler(
        "Il Profeta del Nulla",
        "Le pareti... le pareti parlano! Non le senti? Cosa dobbiamo fare?",
        {"Ascoltare le pareti", "Ignorarlo e allontanarsi", "Cercare di calmarlo"},
        [](Player& p, int choice) {
            switch(choice) {
                case 0:
                    std::cout << "Ti avvicini alle pareti. Un brivido ti percorre la schiena. Perdi 20 punti sanità.\n";
                    p.sanity -= 20;
                    break;
                case 1:
                    std::cout << "Ti allontani, ma il senso di colpa ti perseguita. Perdi 10 punti sanità.\n";
                    p.sanity -= 10;
                    break;
                case 2:
                    std::cout << "Riesci a calmare il viaggiatore. Vi fate compagnia per un po'. Guadagni 15 punti sanità.\n";
                    p.sanity = std::min(100, p.sanity + 15);
                    break;
            }
        }
    ),
    CrazyTraveler(
        "Il Collezionista di Volti",
        "Ho una collezione meravigliosa! Vuoi vedere? Vuoi... partecipare?",
        {"Accetta di vedere la collezione", "Rifiuta educatamente", "Scappa immediatamente"},
        [](Player& p, int choice) {
            switch(choice) {
                case 0:
                    std::cout << "La 'collezione' è terrificante. Perdi 25 punti sanità ma trovi una bottiglia d'acqua di mandorle.\n";
                    p.sanity -= 25;
                    p.inventory.push_back(&items[0]); // Bottiglia d'acqua di mandorle
                    break;
                case 1:
                    std::cout << "Il collezionista sembra deluso ma ti lascia andare. Perdi 5 punti sanità.\n";
                    p.sanity -= 5;
                    break;
                case 2:
                    std::cout << "Corri via, ma l'adrenalina ti prosciuga. Perdi 20 punti energia.\n";
                    p.energy -= 20;
                    break;
            }
        }
    ),
    CrazyTraveler(
        "L'Eco del Passato",
        "Ho visto il futuro nel passato! Vuoi che te lo mostri?",
        {"Accetta l'offerta", "Rifiuta gentilmente", "Chiedi maggiori informazioni"},
        [](Player& p, int choice) {
            switch(choice) {
                case 0:
                    std::cout << "Le visioni sono travolgenti. Perdi 30 punti sanità ma guadagni una strana conoscenza.\n";
                    p.sanity -= 30;
                    p.inventory.push_back(&items[4]); // Amuleto misterioso
                    break;
                case 1:
                    std::cout << "L'Eco sembra offeso. Ti maledice. Perdi 10 punti energia.\n";
                    p.energy -= 10;
                    break;
                case 2:
                    std::cout << "L'Eco parla per ore. Sei confuso ma intrigato. Perdi 15 punti energia ma guadagni 5 punti sanità.\n";
                    p.energy -= 15;
                    p.sanity = std::min(100, p.sanity + 5);
                    break;
            }
        }
    )
};

std::vector<Merchant> merchants = {
    Merchant("Il Barattatore", {
        Item("Mappa parziale", "Una mappa strappata che mostra parte del livello.", {{"sanity", 20}}),
        Item("Bussola instabile", "Una bussola che sembra funzionare... a volte.", {{"energy", 15}}),
        Item("Pillole calmanti", "Pillole che promettono di calmare la mente.", {{"sanity", 30}, {"energy", -10}})
    }),
    Merchant("L'Alchimista delle Backrooms", {
        Item("Elisir di lucidità", "Un liquido brillante che promette di schiarire la mente.", {{"sanity", 40}, {"hydration", -20}}),
        Item("Polvere energetica", "Una polvere che brilla leggermente.", {{"energy", 35}, {"sanity", -10}}),
        Item("Unguento protettivo", "Un unguento che si dice protegga dalle entità.", {{"sanity", 25}, {"energy", 15}})
    })
};
std::string generateRoomDescription() {
    std::uniform_int_distribution<> dis(0, 3);
    std::stringstream ss;
    
    ss << "Un" << (roomTypes[dis(gen)] == "stanza" ? "a " : " ") << roomTypes[dis(gen)] << " con ";
    ss << floorTypes[dis(gen)] << " e pareti " << wallColors[dis(gen)] << ". ";
    ss << lightTypes[dis(gen)] << " illuminano l'ambiente. ";
    
    return ss.str();
}

std::vector<std::string> generateRoomFeatures() {
    std::uniform_int_distribution<> dis(0, roomFeatures.size() - 1);
    std::uniform_int_distribution<> numFeatures(1, 3);
    
    std::vector<std::string> features;
    int featureCount = numFeatures(gen);
    
    for (int i = 0; i < featureCount; ++i) {
        features.push_back(roomFeatures[dis(gen)]);
    }
    
    return features;
}

Room generateRoom() {
    std::uniform_int_distribution<> exitDis(1, 100);
    std::uniform_int_distribution<> entityDis(1, 100);
    std::uniform_int_distribution<> itemDis(1, 100);
    
    bool hasExit = (exitDis(gen) <= 5);  // 5% di probabilità di trovare l'uscita
    
    std::string description = generateRoomDescription();
    std::vector<std::string> features = generateRoomFeatures();
    
    Entity* entity = nullptr;
    if (entityDis(gen) <= 20) {  // 20% di probabilità di incontrare un'entità
        entity = &entities[std::uniform_int_distribution<>(0, entities.size() - 1)(gen)];
    }
    
    Item* item = nullptr;
    if (itemDis(gen) <= 30) {  // 30% di probabilità di trovare un oggetto
        item = &items[std::uniform_int_distribution<>(0, items.size() - 1)(gen)];
    }
    
    return Room(description, hasExit, features, entity, item);
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void waitForEnter() {
    std::cout << "\nPremi Invio per continuare...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void showMainMenu() {
    clearScreen();
    std::cout << "===================================\n";
    std::cout << "  Benvenuto al Training MEG Avanzato delle Backrooms\n";
    std::cout << "===================================\n";
    std::cout << "1. Inizia il training\n";
    std::cout << "2. Spiegazione\n";
    std::cout << "3. Esci\n";
    std::cout << "Scelta: ";
}

void showExplanation() {
    clearScreen();
    std::cout << "=== Spiegazione del Training MEG Avanzato ===\n\n";
    std::cout << "Benvenuto, esploratore delle Backrooms.\n\n";
    std::cout << "Questo è un simulatore di training avanzato creato dal Major Explorer Group (MEG)\n";
    std::cout << "per preparare gli esploratori esperti alle sfide più complesse delle Backrooms.\n\n";
    std::cout << "In questa versione avanzata del training:\n";
    std::cout << "- Ogni stanza è generata in modo unico con descrizioni dettagliate\n";
    std::cout << "- Dovrai gestire la tua sanità mentale, idratazione ed energia\n";
    std::cout << "- Incontrerai vari oggetti e caratteristiche ambientali\n";
    std::cout << "- Potresti imbatterti in entità pericolose\n";
    std::cout << "- Eventi casuali e incontri con altri viaggiatori metteranno alla prova le tue decisioni\n";
    std::cout << "- Potresti incontrare mercanti che offrono oggetti utili\n";
    std::cout << "- Fai attenzione all'acqua di mandorle, la bevanda caratteristica delle Backrooms\n";
    std::cout << "- L'obiettivo rimane trovare l'uscita, ma il percorso sarà più impegnativo\n\n";
    std::cout << "Ricorda: anche se questo training è più realistico, le vere Backrooms\n";
    std::cout << "sono ancora più imprevedibili e pericolose. Buona fortuna, esploratore.\n";
    waitForEnter();
}
void printStatus(Player& player) {
    std::cout << "Sanità mentale: " << player.sanity << "% | ";
    std::cout << "Idratazione: " << player.hydration << "% | ";
    std::cout << "Energia: " << player.energy << "%\n";
    std::cout << "Inventario: ";
    if (player.inventory.empty()) {
        std::cout << "vuoto";
    } else {
        for (const auto& item : player.inventory) {
            std::cout << item->name << ", ";
        }
    }
    std::cout << "\n";
}

void useItem(Player& player, Item* item) {
    for (const auto& effect : item->effects) {
        if (effect.first == "sanity") player.sanity = std::min(100, player.sanity + effect.second);
        if (effect.first == "hydration") player.hydration = std::min(100, player.hydration + effect.second);
        if (effect.first == "energy") player.energy = std::min(100, player.energy + effect.second);
    }
    std::cout << "Hai usato " << item->name << ". ";
    std::cout << "Ti senti " << (item->effects.begin()->second > 0 ? "meglio" : "peggio") << "!\n";
    player.inventory.erase(std::remove(player.inventory.begin(), player.inventory.end(), item), player.inventory.end());
}

void handleEvent(Player& player) {
    Event& currentEvent = events[std::uniform_int_distribution<>(0, events.size() - 1)(gen)];
    std::cout << "\nEVENTO SPECIALE: " << currentEvent.description << "\n";
    for (size_t i = 0; i < currentEvent.choices.size(); ++i) {
        std::cout << i + 1 << ". " << currentEvent.choices[i] << "\n";
    }
    int choice;
    std::cout << "Scelta: ";
    std::cin >> choice;
    currentEvent.outcome(player);
}

void handleCrazyTraveler(Player& player) {
    CrazyTraveler& traveler = crazyTravelers[std::uniform_int_distribution<>(0, crazyTravelers.size() - 1)(gen)];
    std::cout << "\nINCONTRO: Hai incontrato " << traveler.name << "!\n";
    std::cout << traveler.name << ": \"" << traveler.dialogue << "\"\n";
    for (size_t i = 0; i < traveler.responses.size(); ++i) {
        std::cout << i + 1 << ". " << traveler.responses[i] << "\n";
    }
    int choice;
    std::cout << "Scelta: ";
    std::cin >> choice;
    traveler.outcome(player, choice - 1);
}

void handleMerchant(Player& player) {
    Merchant& merchant = merchants[std::uniform_int_distribution<>(0, merchants.size() - 1)(gen)];
    std::cout << "\nMERCANTE: Hai incontrato " << merchant.name << "!\n";
    std::cout << "Offre i seguenti oggetti:\n";
    for (size_t i = 0; i < merchant.inventory.size(); ++i) {
        std::cout << i + 1 << ". " << merchant.inventory[i].name << " - " << merchant.inventory[i].description << "\n";
    }
    std::cout << "0. Non comprare nulla\n";
    int choice;
    std::cout << "Cosa vuoi comprare? ";
    std::cin >> choice;
    if (choice > 0 && choice <= merchant.inventory.size()) {
        player.inventory.push_back(&merchant.inventory[choice - 1]);
        std::cout << "Hai ottenuto: " << merchant.inventory[choice - 1].name << "\n";
    } else {
        std::cout << "Hai deciso di non comprare nulla.\n";
    }
}

void playGame() {
    Player player;
    int moves = 0;
    bool hasExit = false;

    clearScreen();
    std::cout << "Inizia il training avanzato del Livello 0 delle Backrooms.\n";
    std::cout << "Ricorda: mantieni la calma, gestisci le tue risorse e trova l'uscita.\n\n";

    while (player.sanity > 0 && player.hydration > 0 && player.energy > 0 && !hasExit) {
        Room currentRoom = generateRoom();
        
        std::cout << currentRoom.description << "\n";
        std::cout << "Noti: ";
        for (const auto& feature : currentRoom.features) {
            std::cout << feature << ", ";
        }
        std::cout << "\n\n";
        
        if (currentRoom.entity) {
            std::cout << "ATTENZIONE! " << currentRoom.entity->description << "\n";
        }
        
        if (currentRoom.item) {
            std::cout << "Hai trovato: " << currentRoom.item->name << " - " << currentRoom.item->description << "\n";
        }
        
        printStatus(player);
        
        // Possibilità di evento casuale, incontro con viaggiatore impazzito o mercante
        std::uniform_int_distribution<> eventDis(1, 100);
        int eventRoll = eventDis(gen);
        if (eventRoll <= 15) {  // 15% di probabilità di evento speciale
            handleEvent(player);
        } else if (eventRoll <= 30) {  // 15% di probabilità di incontrare un viaggiatore impazzito
            handleCrazyTraveler(player);
        } else if (eventRoll <= 40) {  // 10% di probabilità di incontrare un mercante
            handleMerchant(player);
        }
        
        std::cout << "Cosa vuoi fare? (avanza/resta/osserva/usa [nome oggetto]): ";
        std::string action;
        std::getline(std::cin >> std::ws, action);
        
        if (action == "avanza") {
            moves++;
            player.energy -= 5;
            player.hydration -= 3;
        } else if (action == "resta") {
            player.energy += 10;
            player.hydration -= 1;
            player.sanity -= 5;
        } else if (action == "osserva") {
            player.sanity -= 2;
        } else if (action.substr(0, 3) == "usa") {
            std::string itemName = action.substr(4);
            auto it = std::find_if(player.inventory.begin(), player.inventory.end(),
                                   [&itemName](const Item* item) { return item->name == itemName; });
            if (it != player.inventory.end()) {
                useItem(player, *it);
            } else {
                std::cout << "Non hai questo oggetto nell'inventario.\n";
            }
        }
        
        if (currentRoom.hasExit) {
            hasExit = true;
            std::cout << "Congratulazioni! Hai trovato l'uscita dopo " << moves << " mosse!\n";
        }
        
        std::cout << "\n";
    }

    if (player.sanity <= 0) {
        std::cout << "Hai perso la sanità mentale. Il training è terminato.\n";
    } else if (player.hydration <= 0) {
        std::cout << "Sei disidratato. Il training è terminato.\n";
    } else if (player.energy <= 0) {
        std::cout << "Hai esaurito le energie. Il training è terminato.\n";
    }

    std::cout << "Grazie per aver partecipato al training avanzato delle Backrooms.\n";
}

int main() {
    srand(time(0));
    int choice;
    do {
        showMainMenu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                showExplanation();
                break;
            case 3:
                std::cout << "Grazie per aver utilizzato il simulatore di training MEG. Arrivederci!\n";
                break;
            default:
                std::cout << "Scelta non valida. Riprova.\n";
        }
    } while (choice != 3);
    return 0;
}
