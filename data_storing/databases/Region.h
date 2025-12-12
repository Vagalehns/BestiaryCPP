//
// Created by arman on 11/21/2025.
//

#ifndef BESTIARYCPP_REGION_H
#define BESTIARYCPP_REGION_H

#include "../genericDatabase.h"
#include "../../TUI_functions.h"


struct Region : DefaultStruct {
    FirstLevelRegion first_level_region;
    std::string name;

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

        bool inputForm(Region &new_object) override {

                new_object.name=getStringFromUser("Write region name you want to add", true);

                new_object.first_level_region= (FirstLevelRegion) getOptionFromUser(FirstLevelRegionStrings, "Pick first level region!");

                return true;
        };


        std::vector< std::pair< std::string, char > > getBaseViewColums()  override {
                return {{"ID", 'i'}, {"Name", 's'}, {"First Level Region", 's'}};
            };


        std::vector<std::string> getAsStrings(Region &ref) override {
            std::vector<std::string> result;
            result.push_back(std::to_string(ref.ID));
            result.push_back(ref.name);
            result.push_back(FirstLevelRegionStrings[ref.first_level_region]);
            return result;
        }

        std::string convertToCSVLine(int index) override {
            std::stringstream ss;
            ss<<data[index].ID<<CSV_SEP<<data[index].name<<CSV_SEP<<data[index].first_level_region;
            return ss.str();
        };

        std::string getCSVHeader() override {
            std::stringstream ss;
            ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "First level region";
            return ss.str();
        };

        Region getFromCSVline(std::string CSV_line) override {
            std::string name, temp;
            std::stringstream ss(CSV_line);
            Region new_region;

            std::getline(ss, temp, CSV_SEP);
            new_region.ID = std::stoi(temp);

            std::getline(ss, temp, CSV_SEP);
            new_region.name = temp;

            std::getline(ss, temp, CSV_SEP);
            new_region.first_level_region = (FirstLevelRegion) std::stoi(temp);

            return new_region;
        };
};

#endif //BESTIARYCPP_REGION_H