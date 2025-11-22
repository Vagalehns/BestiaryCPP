//
// Created by arman on 11/21/2025.
//

#ifndef BESTIARYCPP_REGION_H
#define BESTIARYCPP_REGION_H

#include "genericDataBase.h"
#include "tableV2.h"
#include "../TUI_functions.h"


struct Region : DefaultStruct {
    FirstLevelRegion first_level_region;
    std::string name;
    KeyID ID;

    void display() {
        std::cout << "\n Name: " << name ;
        std::cout << "\n First level region: " << FirstLevelRegionStrings[first_level_region] << std::endl;
    };

    Region(){};
    Region(FirstLevelRegion first_level_region, std::string name) {
        this->first_level_region = first_level_region;
        this->name = name;
    }
};

inline std::vector<Region> getDefaultBroadRegions() {
    return {
        // --- ASIA (5) ---
        {ASIA, "East Asia"},
        {ASIA, "South Asia"},
        {ASIA, "Southeast Asia"},
        {ASIA, "Central Asia & Siberia"},
        {ASIA, "Middle East & Arabian Peninsula"},

        // --- AFRICA (6) ---
        {AFRICA, "North Africa (Maghreb & Sahara)"},
        {AFRICA, "West Africa"},
        {AFRICA, "East Africa & Horn of Africa"},
        {AFRICA, "Central Africa & Congo Basin"},
        {AFRICA, "Southern Africa"},
        {AFRICA, "Madagascar & Western Indian Ocean Islands"},

        // --- NORTH_AMERICA (4) ---
        {NORTH_AMERICA, "Canada & Greenland"},
        {NORTH_AMERICA, "Continental USA & Northern Mexico"},
        {NORTH_AMERICA, "Central America"},
        {NORTH_AMERICA, "Caribbean Islands"},

        // --- SOUTH_AMERICA (3) ---
        {SOUTH_AMERICA, "Amazon Basin & Northern South America"},
        {SOUTH_AMERICA, "Andes Mountains Region"},
        {SOUTH_AMERICA, "Southern Cone"},

        // --- ANTARCTICA (2) ---
        {ANTARCTICA, "Antarctic Peninsula"},
        {ANTARCTICA, "Antarctic Mainland"},

        // --- EUROPE (4) ---
        {EUROPE, "Western Europe"},
        {EUROPE, "Eastern Europe"},
        {EUROPE, "Northern Europe"},
        {EUROPE, "Southern Europe"},

        // --- AUSTRALIA_OCEANIA (3) ---
        {AUSTRALIA_OCEANIA, "Australia"},
        {AUSTRALIA_OCEANIA, "New Zealand"},
        {AUSTRALIA_OCEANIA, "Pacific Islands"},

        // --- OCEANIC REGIONS (5) ---
        {PACIFIC_OCEAN, "North Pacific Ocean"},
        {PACIFIC_OCEAN, "South Pacific Ocean"},
        {ATLANTIC_OCEAN, "North Atlantic Ocean"},
        {ATLANTIC_OCEAN, "South Atlantic Ocean"},
        {INDIAN_OCEAN, "Indian Ocean"},

        // --- SOUTHERN & ARCTIC OCEANS (2) ---
        {SOUTHERN_OCEAN, "Southern Ocean"},
        {ARCTIC_OCEAN, "Arctic Ocean"},

        // --- GENERAL MARINE/GLOBAL (2) ---
        {OCEANS, "Global Intertidal Zones"},
        {OCEANS, "Deep Sea"}
    };
};

class RegionDB : public DB<Region, MAX_REGIONS> {
public:

    RegionDB ( bool add_defaults=true) {
        if (add_defaults) {
            for (auto region: getDefaultBroadRegions()) {
                appendAutoID(region);
            }
        }
    }

        bool inputForm() override {
            Region new_region;
            bool confirm;

            do {
                clearConsole();

                new_region.name=getStringFromUser("Write region name you want to add", true);

                new_region.first_level_region= (FirstLevelRegion) getOptionFromUser(FirstLevelRegionStrings, "Pick first level region!");

                clearConsole();
                std::cout<<"Do you want to add?";
                new_region.display();
                confirm = getConfirmationFromUser(">");



                if (!confirm) {
                    std::cout<<"Do you want to cancel?";
                    if (getConfirmationFromUser(">")) {
                        return false;
                    }
                }

            } while (!confirm);

            appendAutoID(new_region);
            return true;
        };


        void display(bool hide_filtered) override {

            TableV2 table({{"ID", 'i'}, {"Name", 's'}, {"First Level Region", 's'} });

            for (int i = 0; i < counter; i++) {

                if (data[i].filtered_out && hide_filtered) continue;

                table.addItem(data[i].ID);
                table.addItem(data[i].name);
                table.addItem(FirstLevelRegionStrings[data[i].first_level_region]);
            }

            table.show();

        };

        std::string convertToCSV() override {return "";};
        Region getFromCSVline(std::string CSV_line) override {return Region();};
};

#endif //BESTIARYCPP_REGION_H