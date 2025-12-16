//
// Created by arman on 11/25/2025.
//

#ifndef BESTIARYCPP_ENCLOSURE_H
#define BESTIARYCPP_ENCLOSURE_H


#include "Species.h"
#include "../genericDatabase.h"
#include "../../TUI_functions/TUI_functions.h"
#include "../../TUI_functions/Menu.h"


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

    void sortOptions() override {
        Menu sortMenu("Pick sort option", "Don't sort");

        sortMenu.addItem({
            "Sort by name",
            ([this]() -> MenuReturn {
                this->sort(&Enclosure::name, genericStringSort);
                return {BACK, ""};
            })
        });

        sortMenu.addItem({
            "Sort by section",
            ([this]() -> MenuReturn {
                this->sort(&Enclosure::section, genericStringSort);
                return {BACK, ""};
            })
        });

        sortMenu.addItem({
            "Sort by type",
            ([this]() -> MenuReturn {
                this->sort(&Enclosure::type, [](unsigned int A, unsigned int B) {
                    return genericStringSort(EnclosuresTypes[A], EnclosuresTypes[B]);
                });
                return {BACK, ""};
            })
        });


        sortMenu.open();
    };


    void filterOptions() override {

        Menu filterMenu("Add filter/search", "View");

        filterMenu.addItem({
            "Search by name",
            ([this]() -> MenuReturn {

                std::string search_term = getStringFromUser("Please enter search term:", true);
                bool full_match = getConfirmationFromUser("Does term must be full match?");

                this->filterByField(&Enclosure::name, makeGenericStringFilter(search_term, full_match));

                return {BACK, ""};
            })
        });

        filterMenu.addItem({
            "Filter by type",
            ([this]() -> MenuReturn {


                auto type =  getOptionFromUser(EnclosuresTypes, "Pick enclosure type!");

                this->filterByField(&Enclosure::type, [type](int cur_type) -> bool {
                    return type==cur_type;
                });

                return {STAY, ""};
            })
        });


        filterMenu.addItem({
            "Clear filters",
            ([this]() -> MenuReturn {

                this->resetFilter();

                return {BACK, ""};
            })
        });

        filterMenu.open();
    }

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

    bool isRecordOrphan(Enclosure &ref) override {
        return false;
    }
};


#endif //BESTIARYCPP_ENCLOSURE_H