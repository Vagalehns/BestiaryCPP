//
// Created by arman on 11/25/2025.
//

#ifndef BESTIARYCPP_ENCLOSURE_H
#define BESTIARYCPP_ENCLOSURE_H


#include "../genericDatabase.h"
#include "../../TUI_functions.h"


struct Enclosure : DefaultStruct {

    std::string name;
    std::string section;
    unsigned int type;

    void display() {
        std::cout << "\n Name: " << name;
        std::cout << "\n Section: " << section;
        std::cout << "\n Type: " << EnclosuresTypes[type];
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

    bool inputForm(Enclosure &new_object, bool edit) override {

        if (!edit || getConfirmationFromUser("Do you want to edit name?")) {
            new_object.name=getStringFromUser("Write name of enclosure you want to add", true);
        }
        clearConsole();

        if (!edit || getConfirmationFromUser("Do you want to edit section?")) {
            new_object.section=getStringFromUser("Write of section you want to add", true);
        }
        clearConsole();

        if (!edit || getConfirmationFromUser("Do you want to edit type?")) {
            new_object.type=  getOptionFromUser(EnclosuresTypes, "Pick enclosure type!");
        }
        return true;
    };


        std::vector< std::pair< std::string, char > > getViewColums(char view)  override {
                return {{"ID", 'i'}, {"Name", 's'}, {"Section", 's'}, {"Type", 's'}};
            };

        std::vector<std::string> getAsStrings(Enclosure &ref, char view) override {
            std::vector<std::string> result;
            result.push_back(std::to_string(ref.ID));
            result.push_back(ref.name);
            result.push_back(ref.section);
            result.push_back(EnclosuresTypes[ref.type]);
            return result;
        }


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