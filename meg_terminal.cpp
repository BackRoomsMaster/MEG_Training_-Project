#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>

class MEGTerminal {
private:
    std::map<std::string, std::vector<std::string>> database;
    std::string currentLevel;

public:
    MEGTerminal() : currentLevel("Level 0") {
        initializeDatabase();
    }

    void run() {
        std::string input;
        std::cout << "Welcome to the MEG Backrooms Database Terminal\n";
        std::cout << "Type 'help' for available commands.\n\n";
        
        while (true) {
            std::cout << "MEG@" << currentLevel << "> ";
            std::getline(std::cin, input);

            if (input == "exit") {
                break;
            } else if (input == "levels") {
                showLoadingAnimation("Retrieving level data");
                listLevels();
            } else if (input.substr(0, 6) == "access") {
                if (input.length() > 7) {
                    accessLevel(input.substr(7));
                } else {
                    std::cout << "Usage: access <level number>\n";
                }
            } else if (input == "info") {
                showLoadingAnimation("Loading level information");
                showLevelInfo();
            } else if (input == "help") {
                showHelp();
            } else {
                std::cout << "Command not recognized. Type 'help' for available commands.\n";
            }
        }

        std::cout << "Logging out of MEG Database. Stay safe in the Backrooms.\n";
    }

private:
    void initializeDatabase() {
        database["Level 0"] = {
            "Also known as: The Lobby, Tutorial Level, The Yellow Rooms",
            "Endless expanse of randomly segmented empty rooms",
            "Monotone yellow wallpaper, beige carpet, fluorescent lighting",
            "Buzzing fluorescent lights, occasional humming",
            "Smells of moist carpet, dusty air, and cleaning chemicals",
            "Low entity count, relatively safe"
        };

        database["Level 1"] = {
            "Also known as: Habitable Zone, Warehouse",
            "Expansive warehouse-like environment",
            "Concrete floors, exposed pipes, metallic walls",
            "Dim lighting, some areas in complete darkness",
            "Constant humming of machinery",
            "Supplies can sometimes be found here",
            "Beware of hostile entities, especially hounds"
        };

        database["Level 2"] = {
            "Also known as: Pipe Dreams, Maintenance Tunnels",
            "Maze of concrete hallways and maintenance rooms",
            "Pipes of various sizes line the walls and ceilings",
            "Dim fluorescent lighting, some areas dark",
            "Constant sound of running water and steam hissing",
            "Risk of hypothermia due to low temperatures",
            "Entities more common, including child-like laughter"
        };

        database["Level 3"] = {
            "Also known as: Electrical Station",
            "Massive electrical facility with catwalks and machinery",
            "Concrete floors, metal walls, high ceilings",
            "Dim lighting from sparse light fixtures",
            "Constant electrical humming and occasional sparks",
            "Danger of electrocution and falling",
            "Home to 'Smilers' entity, extreme caution advised"
        };

        database["Level 4"] = {
            "Also known as: Abandoned Office",
            "Labyrinth of office cubicles and corridors",
            "Dull colors, old office equipment, dropped ceilings",
            "Flickering fluorescent lights",
            "Smell of stale air and old paper",
            "Relatively safe, good for scavenging supplies",
            "Beware of hostile entities disguised as mannequins"
        };

        database["Level 5"] = {
            "Also known as: The Hotel",
            "Endless hotel corridors with numbered rooms",
            "Red carpets, peeling wallpaper, old paintings",
            "Dim lighting from wall sconces",
            "Musty smell, occasional whiff of cleaning products",
            "Rooms may contain supplies or dangers",
            "Entity known as 'The Hotel Manager' may appear"
        };

        database["Level 6"] = {
            "Also known as: Lights Out",
            "Pitch black maze with metal walls",
            "Completely dark, requires light source to navigate",
            "Silent except for explorer's movements",
            "Metal smell and cold temperatures",
            "Highly dangerous due to darkness and entities",
            "Home to 'The Beast', an extremely dangerous entity"
        };

        database["Level 7"] = {
            "Also known as: Thalassophobia",
            "Endless ocean with small islands and structures",
            "Dark water, wooden structures, constant overcast sky",
            "Sound of waves and distant thunder",
            "Smell of salt water and rotting wood",
            "Danger of drowning and aquatic entities",
            "Boats can be found for transportation"
        };

        database["Level 8"] = {
            "Also known as: Caverns",
            "Massive cave system with bioluminescent flora",
            "Rock walls, underground rivers, glowing plants",
            "Dim bioluminescent lighting",
            "Damp, earthy smell",
            "Home to unique subterranean entities",
            "Rich in almond water, a valuable resource"
        };

        database["Level 9"] = {
            "Also known as: The Suburbs",
            "Endless suburban neighborhoods, always twilight",
            "Cookie-cutter houses, empty streets, manicured lawns",
            "Dim twilight lighting, some streetlights",
            "Smell of fresh-cut grass and barbecue",
            "Appears safe but highly dangerous",
            "Inhabited by entities mimicking suburban life"
        };
    }

    void showLoadingAnimation(const std::string& message) {
        const char animation[] = {'|', '/', '-', '\\'};
        const int animationLength = sizeof(animation) / sizeof(animation[0]);
        const int totalFrames = 20;

        for (int i = 0; i < totalFrames; ++i) {
            std::cout << "\r" << message << " " << animation[i % animationLength] << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "\r" << std::string(message.length() + 2, ' ') << "\r";
    }

    void listLevels() {
        std::cout << "Known Backrooms Levels:\n";
        for (const auto& level : database) {
            std::cout << "- " << level.first << "\n";
        }
    }

    void accessLevel(const std::string& levelNumber) {
        std::string level = "Level " + levelNumber;
        if (database.find(level) != database.end()) {
            showLoadingAnimation("Accessing " + level + " database");
            currentLevel = level;
            std::cout << "Successfully accessed database for " << level << "\n";
        } else {
            std::cout << "Error: Level not found in database.\n";
        }
    }

    void showLevelInfo() {
        std::cout << "Information for " << currentLevel << ":\n";
        for (const auto& info : database[currentLevel]) {
            std::cout << "- " << info << "\n";
        }
    }

    void showHelp() {
        std::cout << "Available commands:\n";
        std::cout << "  levels - List all known Backrooms levels\n";
        std::cout << "  access <level number> - Access information for a specific level\n";
        std::cout << "  info - Show information about the current level\n";
        std::cout << "  help - Show this help message\n";
        std::cout << "  exit - Log out of the MEG Database\n";
    }
};

int main() {
    MEGTerminal terminal;
    terminal.run();
    return 0;
}
