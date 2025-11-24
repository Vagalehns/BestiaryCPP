//
// Created by arman on 11/24/2025.
//

#ifndef BESTIARYCPP_SPECIES_H
#define BESTIARYCPP_SPECIES_H


#include "genericDataBase.h"
#include "tableV2.h"
#include "../TUI_functions.h"
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
    std::function<KeyID()> region_picker;
public:

    SpeciesDB (std::function<Region*(KeyID)> region_resolver, std::function<KeyID()> region_picker) {
        this->region_resolver = region_resolver;
        this->region_picker = region_picker;
    }

        bool inputForm(Species &new_object) override {

            new_object.name=getStringFromUser("Write species name", true);
            clearConsole();
            new_object.latin_name=getStringFromUser("Write species latin name", true);
            clearConsole();
            new_object.icon=getOptionFromUser(SpeciesEmojiIcons, "Pick species icon");

            new_object.RegionID=ExternalKey(region_picker(), region_resolver);

            return true;
        };

        void baseView() {
            display({{"ID", 'i'}, {"Name", 's'}, {"Latin name", 's'}, {"Icon", 's'}, {"Region", 's'}},
                [](TableV2& t, Species& r) {

                    t.addItem(r.ID);
                    t.addItem(r.name);
                    t.addItem(r.latin_name);
                    t.addItem(SpeciesEmojiIcons[r.icon]);

                    Region *region=r.RegionID.get();

                    if (region==nullptr) {
                        t.addItem("Unvalid key");
                    }else {
                        t.addItem(region->name);
                    }
            });
        }

        KeyID pickByUser() override {
                // KeyID id_picked=display({{"ID", 'i'}, {"Name", 's'}, {"First Level Region", 's'}},
                //         [](TableV2& t, Region& r) {
                //         t.addItem(r.ID);
                //         t.addItem(r.name);
                //         t.addItem(FirstLevelRegionStrings[r.first_level_region]);
                //     }, false, true);
                //
                KeyID id_picked=0;
                return data[id_picked].ID;
            }

        std::string convertToCSV() override {return "";};
        Species getFromCSVline(std::string CSV_line) override {return Species();};
};


#endif //BESTIARYCPP_SPECIES_H