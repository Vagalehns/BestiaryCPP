//
// Created by arman on 11/25/2025.
//

#ifndef BESTIARYCPP_ENCLOSURE_H
#define BESTIARYCPP_ENCLOSURE_H


#include "Species.h"
#include "../genericDB.h"
#include "../../TUI_functions/TUI_functions.h"
#include "../../TUI_functions/Menu.h"


struct Enclosure : DefaultStruct {
    std::string name;
    std::string section;
    unsigned int type;

    void display() const {
        std::cout << "\n Name: " << name;
        std::cout << "\n Section: " << section;
        std::cout << "\n Type: " << EnclosuresTypes[type];
    };

    Enclosure() : type(0) {
    } ;

    Enclosure(const std::string &name, const std::string &section, const unsigned int type) {
        this->name = name;
        this->section = section;
        this->type = type;
    };
};


class EnclosureDB : public DB<Enclosure, MAX_ENCLOSURE> {
public:
    ~EnclosureDB() = default;

    bool inputForm(Enclosure &new_object, bool edit) override;

    void preparedSort(char option) override;

    void sortWithOptions() override;


    void filterWithOptions() override;

    std::vector<std::pair<std::string, char> > getViewColums(char view) override;

    std::vector<std::string> getAsStrings(Enclosure &ref, char view) override;


    std::string convertToCSVLine(int index) override;

    std::string getCSVHeader() override;

    Enclosure getFromCSVline(std::string CSV_line) override;

    bool isRecordOrphan(Enclosure &ref) override;
};


#endif //BESTIARYCPP_ENCLOSURE_H
