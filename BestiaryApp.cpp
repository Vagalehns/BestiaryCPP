//
// Created by armands on 14.11.25.
//

#include "BestiaryApp.h"

#include "file_functions.h"
#include "info.h"
#include "styling_functions.h"


#define DEF_ANIMALDB_FILENAME "animal_db.csv"
#define DEF_ENCLOSUREDB_FILENAME "enclosures_db.csv"
#define DEF_KEEPERDB_FILENAME "keepers_db.csv"
#define DEF_SPECIESDB_FILENAME "species_db.csv"
#define DEF_REGIONDB_FILENAME "regions_db.csv"

inline bool echo(const std::string &str ){
    std::cout << str << std::endl;
    return true;
};

template<class DB>
auto makeAdd(DB& db, std::string func_name) {
    return [&, func_name]() -> MenuReturn {
        if (db.addByForm())
            return {STAY_SHOW_MSG, func_name + " added"};
        else
            return {STAY_SHOW_ERROR, "Failed to add " + func_name};
    };
}

template<class DB>
auto makeEdit(DB& db, std::string func_name) {
    return [&, func_name]() -> MenuReturn {
        if (db.editByForm())
            return {STAY_SHOW_MSG, func_name + " edited"};
        else
            return {STAY_SHOW_ERROR, "Failed to edit " + func_name};
    };
}

template<class DB>
auto makeView(DB& db) {
    return [&]() -> MenuReturn {
        db.display();
        return {STAY, ""};
    };
}


bool BestiaryApp::saveEverything(std::filesystem::path dest) {
    int saved=0;
    saved += this->animalDB.saveToCSVFile(DEF_ANIMALDB_FILENAME, dest.string());
    saved += this->enclosureDB.saveToCSVFile(DEF_ENCLOSUREDB_FILENAME, dest.string());
    saved += this->keeperDB.saveToCSVFile(DEF_KEEPERDB_FILENAME, dest.string());
    saved += this->speciesDB.saveToCSVFile(DEF_SPECIESDB_FILENAME, dest.string());
    saved += this->regionDB.saveToCSVFile(DEF_REGIONDB_FILENAME, dest.string());

    return saved==5;
};

bool BestiaryApp::loadEverything(std::filesystem::path dest, bool clear_data) {

    if (
        checkIfFileExists(dest / DEF_ANIMALDB_FILENAME)&&
        checkIfFileExists(dest / DEF_ENCLOSUREDB_FILENAME)&&
        checkIfFileExists(dest / DEF_KEEPERDB_FILENAME)&&
        checkIfFileExists(dest / DEF_SPECIESDB_FILENAME)&&
        checkIfFileExists(dest / DEF_REGIONDB_FILENAME)
        ) {

        int loaded=0;

        loaded+=this->animalDB.readFromCSVFile(DEF_ANIMALDB_FILENAME, dest.string(), clear_data);
        loaded+=this->enclosureDB.readFromCSVFile(DEF_ENCLOSUREDB_FILENAME, dest.string(), clear_data);
        loaded+=this->keeperDB.readFromCSVFile(DEF_KEEPERDB_FILENAME, dest.string(), clear_data);
        loaded+=this->speciesDB.readFromCSVFile(DEF_SPECIESDB_FILENAME, dest.string(), clear_data);
        loaded+=this->regionDB.readFromCSVFile(DEF_REGIONDB_FILENAME, dest.string(), clear_data);

        return loaded==5;

        }else {
            return false;
        };
};

BestiaryApp::BestiaryApp(AppState state) :

    speciesDB(
        std::bind_front(&RegionDB::getByID, &regionDB),
        std::bind_front(&RegionDB::pickByUser, &regionDB)
    ),

    animalDB(
        std::bind_front(&SpeciesDB::getByID, &speciesDB),
        std::bind_front(&SpeciesDB::pickByUser, &speciesDB),

        std::bind_front(&KeeperDB::getByID, &keeperDB),
        std::bind_front(&KeeperDB::pickByUser, &keeperDB),

        std::bind_front(&EnclosureDB::getByID, &enclosureDB),
        std::bind_front(&EnclosureDB::pickByUser, &enclosureDB)
    ),

    menu_start("Main menu", "Exit"),
    menu_add_data("Add data", "Back"),
    menu_view_data("View data", "Back"),
    menu_load_data("Load data", "Back"),
    menu_save_data("Save data", "Back"),
    menu_edit_data("Edit data", "Back"){


    State = state;

    menu_save_data.addItem({"Save to zip", [this]()->MenuReturn {

            auto dest=getPathFromUser("Where to save?", true);
            ensureFolderExists(dest);

            auto filename=getStringFromUser("Input file name", true, "\\/:*?\"<>|\0");

            filename+=".zip";


            std::filesystem::path temp_path="./temp_save_folder";

            createFolder(temp_path);
            saveEverything(temp_path);
            zipFolder(temp_path, (dest / filename));
            removeFile(temp_path);


            return {STAY_SHOW_MSG, "Zip successfully saved"};
    }});

    menu_save_data.addItem({"Save to folder", [this]()->MenuReturn {

            auto dest=getPathFromUser("Where to save?", true);

            ensureFolderExists(dest);

            if (saveEverything(dest)) {
                return {STAY_SHOW_MSG, "Files saved"};
            }else {
                return {STAY_SHOW_ERROR, "Something went wrong"};
            }
    }});

    menu_save_data.addItem({"Save specific table", [this]()->MenuReturn {

        auto table_choice=getOptionFromUser(
            std::array<const char*, 5> {{"Animals", "Enclosures", "Keepers", "Species", "Regions"}},
            "Pick the table?"
            );;

        auto dest=getPathFromUser("Where to save?");
        auto filename=getStringFromUser("Input file name", true, "\\/:*?\"<>|\0");

        bool saved;
        switch (table_choice) {
            case 0:
                saved=this->animalDB.saveToCSVFile(filename, dest.string());
                break;
            case 1:
                saved=this->enclosureDB.saveToCSVFile(filename, dest.string());
                break;
            case 2:
                saved=this->keeperDB.saveToCSVFile(filename, dest.string());
                break;
            case 3:
                saved=this->speciesDB.saveToCSVFile(filename, dest.string());
                break;
            case 4:
                saved=this->speciesDB.saveToCSVFile(filename, dest.string());
                break;
            default:
                saved=false;
        }

        if (saved) {
            return {STAY_SHOW_MSG, "Table saved"};
        }else {
            return {STAY_SHOW_ERROR, "Failed to save to CSV file"};
        }

    }});

    menu_load_data.addItem({"Load from folder", [this]()->MenuReturn {

            clearConsole();
            std::cout << "If you don't clear data there's extremely small chance of conflicting IDs\nDo you want to clear data?";

            bool clear_data = getConfirmationFromUser(">");
            auto dest=getPathFromUser("Input folder path");

            if (loadEverything(dest, clear_data)) {
                return {STAY_SHOW_MSG, "Table loaded"};
            }else {
                return {STAY_SHOW_ERROR, "Failed to load properly"};
            }



    }});

    menu_load_data.addItem({"Load from zip", [this]()->MenuReturn {

        clearConsole();
        std::cout << "If you don't clear data there's extremely small chance of conflicting IDs\nDo you want to clear data?";

        bool clear_data = getConfirmationFromUser(">");
        auto dest=getPathFromUser("Input folder path");
        auto filename=getStringFromUser("Input file name", true, "\\/:*?\"<>|\0");

        if (!checkIfFileExists(dest/ filename)) {
            return {STAY_SHOW_ERROR, "File doesn't exist"};
        };

        std::filesystem::path temp_path="./temp_save_folder";

        createFolder(temp_path);

        unzipFile(dest/filename, temp_path);

        loadEverything(temp_path/"temp_save_folder", clear_data);
        removeFile(temp_path);

        return {STAY_SHOW_MSG, "Zip successfully saved"};

    }});

    menu_load_data.addItem({"Load specific table", [this]()->MenuReturn {

        auto dest=getPathFromUser("Input folder path");
        auto filename=getStringFromUser("Input file name", true, "\\/:*?\"<>|\0");

        if (!checkIfFileExists(dest/ filename)) {
            return {STAY_SHOW_ERROR, "File doesn't exist"};
        };

        auto table_choice=getOptionFromUser(
        std::array<const char*, 5> {{"Animals", "Enclosures", "Keepers", "Species", "Regions"}},
        "Pick the table?"
        );;

        bool loaded;
        switch (table_choice) {
            case 0:
                loaded=this->animalDB.readFromCSVFile(filename, dest.string());
                break;
            case 1:
                loaded=this->enclosureDB.readFromCSVFile(filename, dest.string());
                break;
            case 2:
                loaded=this->keeperDB.readFromCSVFile(filename, dest.string());
                break;
            case 3:
                loaded=this->speciesDB.readFromCSVFile(filename, dest.string());
                break;
            case 4:
                loaded=this->speciesDB.readFromCSVFile(filename, dest.string());
                break;
            default:
                loaded=false;
        }

        if (loaded) {
            return {STAY_SHOW_MSG, "Table successfully loaded"};
        }else {
            return {STAY_SHOW_ERROR, "Failed to load properly"};
        }

    }});




    menu_add_data.addItem({"Add region",    makeAdd(regionDB, "Region")});
    menu_add_data.addItem({"Add species",   makeAdd(speciesDB, "Species")});
    menu_add_data.addItem({"Add animal",    makeAdd(animalDB, "Animal")});
    menu_add_data.addItem({"Add keeper",    makeAdd(keeperDB, "Keeper")});
    menu_add_data.addItem({"Add enclosure", makeAdd(enclosureDB, "Enclosure")});

    menu_edit_data.addItem({"Add region",    makeEdit(regionDB, "Region")});
    menu_edit_data.addItem({"Add species",   makeEdit(speciesDB, "Species")});
    menu_edit_data.addItem({"Add animal",    makeEdit(animalDB, "Animal")});
    menu_edit_data.addItem({"Add keeper",    makeEdit(keeperDB, "Keeper")});
    menu_edit_data.addItem({"Add enclosure", makeEdit(enclosureDB, "Enclosure")});

    menu_view_data.addItem({"View region",    makeView(regionDB)});
    menu_view_data.addItem({"View species",   makeView(speciesDB)});
    menu_view_data.addItem({"View animal",    makeView(animalDB)});
    menu_view_data.addItem({"View keeper",    makeView(keeperDB)});
    menu_view_data.addItem({"View enclosure", makeView(enclosureDB)});

    menu_view_data.addItem({"View feeding", []()->MenuReturn{return MenuReturn(STAY);}});

    menu_start.addItem({"Add data", [this]()->MenuReturn{return this->menu_add_data.open();}});
    menu_start.addItem({"Edit data", [this]()->MenuReturn{return this->menu_edit_data.open();}});
    menu_start.addItem({"View data", [this]()->MenuReturn{return this->menu_view_data.open();}});
    menu_start.addItem({"Load data", [this]()->MenuReturn{return this->menu_load_data.open();}});
    menu_start.addItem({"Save data", [this]()->MenuReturn{return this->menu_save_data.open();}});
}

bool BestiaryApp::Run() {

    if (State.help) {
        std::cout << HELP_MESSEGE;
        return true;
    }

    if (!State.nofuzz) {
        Splashscreen();
    }

    menu_start.open();

    std::cout << "App Closed";

    return true;
}

void BestiaryApp::Splashscreen() {

        std::cout << "\n" << BRIGHTGREENSTY << BOLDSTY << SPLASHSCREEN << ENDSTY << "\n";
        std::cout << "                " << APP_USECASE << "\n\n";
        std::cout << "Version: " << APP_VERSION << "\n";
        std::cout << "Made by: " << APP_AUTHOR << "\n";
        std::cout << "Copyright " << APP_UPDATE_YEAR << "\n\n";
        std::cout << "===================================";
        std::cout << "\nPress Enter to continue or e to exit...";


        int user_input;

        do {
            user_input = std::cin.get();
        } while (user_input != '\n' && user_input != 'e');
        if (user_input == 'e') {
            std::exit(0);
        }

};