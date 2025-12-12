//
// Created by arman on 11/24/2025.
//

#ifndef BESTIARYCPP_SPECIES_H
#define BESTIARYCPP_SPECIES_H


#include "../genericDatabase.h"
#include "../../TUI_functions.h"
#include "Region.h"

struct Species : DefaultStruct {

    std::string name;
    std::string latin_name;
    unsigned int icon;
    ExternalKey<Region> RegionID;

    void display() {
        std::cout << "\n Name: " << name ;
        std::cout << "\n Latin Name: " << latin_name ;
        std::cout << "\n Icon: " << SpeciesEmojiIcons[icon] ;
        std::cout << "\n Region ID: " << RegionID.ID << std::endl;
    };

    Species(){};

    Species(std::string name, std::string latin_name, unsigned int icon, ExternalKey<Region> regionID) {
        this->name = name;
        this->latin_name = latin_name;
        this->icon = icon;
        this->RegionID = regionID;
    }
};


class SpeciesDB : public DB<Species, MAX_SPECIES> {
    std::function<Region*(KeyID)> region_resolver;
    std::function<KeyID(bool &)> region_picker;
public:

    SpeciesDB (std::function<Region*(KeyID)> region_resolver, std::function<KeyID(bool &)> region_picker) {
        this->region_resolver = region_resolver;
        this->region_picker = region_picker;
    }

    bool inputForm(Species &new_object) override {

        new_object.name=getStringFromUser("Write species name", true);
        clearConsole();
        new_object.latin_name=getStringFromUser("Write species latin name", true);
        clearConsole();
        new_object.icon=getOptionFromUser(SpeciesEmojiIcons, "Pick species icon");

        bool success;

        new_object.RegionID=ExternalKey(region_picker(success), region_resolver);
        if (!success) return false;

        return true;
    };


    std::vector< std::pair< std::string, char > > getBaseViewColums()  override {
        return {{"ID", 'i'}, {"Name", 's'}, {"Latin name", 's'}, {"Icon", 's'}, {"Region", 's'}};
    };

    std::vector<std::string> getAsStrings(Species &ref) override {
        std::vector<std::string> result;


        result.push_back(std::to_string(ref.ID));
        result.push_back(ref.name);
        result.push_back(ref.latin_name);
        result.push_back(SpeciesEmojiIcons[ref.icon]);

        Region *region=ref.RegionID.get();

        if (region==nullptr) {
            result.push_back("Unvalid key");
        }else {
            result.push_back(region->name);
        };

        return result;
    }

        std::string convertToCSVLine(int index) override {
            std::stringstream ss;
            auto di=data[index];
            ss<<di.ID<<CSV_SEP<<di.name<<CSV_SEP<<di.latin_name<<CSV_SEP<<di.icon<<CSV_SEP<<di.RegionID.ID;
            return ss.str();
        };

        std::string getCSVHeader() override {
            std::stringstream ss;
            ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Latin name" << CSV_SEP << "Icon" << CSV_SEP << "Region ID";
            return ss.str();
        };

        Species getFromCSVline(std::string CSV_line) override {

            std::string name, temp;
            std::stringstream ss(CSV_line);
            Species new_species;

            std::getline(ss, temp, CSV_SEP);
            new_species.ID = std::stoi(temp);

            std::getline(ss, temp, CSV_SEP);
            new_species.name = temp;

            std::getline(ss, temp, CSV_SEP);
            new_species.latin_name = temp;

            std::getline(ss, temp, CSV_SEP);
            new_species.icon = std::stoi(temp);

            std::getline(ss, temp, CSV_SEP);
            new_species.RegionID= ExternalKey(std::stoi(temp), region_resolver);

            return new_species;
        };
};


#endif //BESTIARYCPP_SPECIES_H