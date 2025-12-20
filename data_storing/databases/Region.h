//
// Created by arman on 11/21/2025.
//

#ifndef BESTIARYCPP_REGION_H
#define BESTIARYCPP_REGION_H

#include <utility>

#include "../genericDB.h"
#include "../../TUI_functions/TUI_functions.h"


struct Region : DefaultStruct {
    FirstLevelRegion first_level_region;
    std::string name;

    void display() const {
        std::cout << "\n Name: " << name;
        std::cout << "\n First level region: " << FirstLevelRegionStrings[first_level_region] << std::endl;
    };

    Region() : first_level_region() {
    } ;

    Region(FirstLevelRegion first_level_region, std::string name) {
        this->first_level_region = first_level_region;
        this->name = std::move(name);
    }
};


class RegionDB : public DB<Region, MAX_REGIONS> {
public:
    ~RegionDB() = default;

    RegionDB(bool add_defaults = true);

    bool inputForm(Region &new_object, bool edit) override;

    void filterWithOptions() override;


    void preparedSort(char option) override;

    void sortWithOptions() override;


    std::vector<std::pair<std::string, char> > getViewColums(char view) override;

    std::vector<std::string> getAsStrings(Region &ref, char view) override;

    std::string convertToCSVLine(int index) override;

    std::string getCSVHeader() override;

    Region getFromCSVline(std::string CSV_line) override;

    bool isRecordOrphan(Region &ref) override;
};

#endif //BESTIARYCPP_REGION_H
