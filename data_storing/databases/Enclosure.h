//
// Created by arman on 11/25/2025.
//

#ifndef BESTIARYCPP_ENCLOSURE_H
#define BESTIARYCPP_ENCLOSURE_H


#include "../genericDataBase.h"
#include "../tableV2.h"
#include "../../TUI_functions.h"


struct Enclosure : DefaultStruct {

    std::string name;
    std::string section;
    unsigned int type;

    void display() {
        std::cout << "\n Name: " << name;
        std::cout << "\n Section: " << section;
    };

    Enclosure(){};

    Enclosure(std::string name, std::string section, unsigned int type) {
        this->name = name;
        this->section = section;
        this->type = type;
    };
};


class EnclosureDB : public DB<Enclosure, MAX_ENCLOSURE> {
public:

        bool inputForm(Enclosure &new_object) override {

                new_object.name=getStringFromUser("Write name you want to add", true);
                new_object.section=getStringFromUser("Write section you want to add", true);
                new_object.type=  getOptionFromUser(EnclosuresTypes, "Pick enclosure type!");
                return true;
        };


        std::vector<TableV2Column> getBaseViewColums() override {
                return {{"ID", 'i'}, {"Name", 's'}, {"Section", 's'}, {"Type", 's'}};
            };

        std::function<void(TableV2&, Enclosure&)> getBaseViewAddItemsFunc() override {
            return [](TableV2& t, Enclosure& r) {
                t.addItem(r.ID);
                t.addItem(r.name);
                t.addItem(r.section);
                t.addItem(EnclosuresTypes[r.type]);
            };
        };


        std::string convertToCSVLine(int index) override {
            std::stringstream ss;
            auto di=data[index];
            ss<<di.ID<<CSV_SEP<<di.name<<CSV_SEP<<di.section<<CSV_SEP<<di.type;
            return ss.str();
        };

        std::string getCSVHeader() override {
            std::stringstream ss;
            ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Section" << CSV_SEP << "Type";
            return ss.str();
        }

        Enclosure getFromCSVline(std::string CSV_line) override {
            std::string name, temp;
            std::stringstream ss(CSV_line);
            Enclosure new_enclosure;

            std::getline(ss, temp, CSV_SEP);
            new_enclosure.ID = std::stoi(temp);

            std::getline(ss, temp, CSV_SEP);
            new_enclosure.name = temp;

            std::getline(ss, temp, CSV_SEP);
            new_enclosure.section = temp;

            std::getline(ss, temp, CSV_SEP);
            new_enclosure.type= std::stoi(temp);

            return new_enclosure;
        };
};


#endif //BESTIARYCPP_ENCLOSURE_H