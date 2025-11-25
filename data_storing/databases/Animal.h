//
// Created by arman on 11/25/2025.
//

#ifndef BESTIARYCPP_ANIMAL_H
#define BESTIARYCPP_ANIMAL_H

#include <iomanip>

#include "Enclosure.h"
#include "Keeper.h"
#include "Species.h"
#include "../genericDataBase.h"
#include "../tableV2.h"
#include "../../TUI_functions.h"

std::string inline formatTime(std::time_t t) {
    std::tm* tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M");
    return oss.str();
}


struct Animal : DefaultStruct {

    std::string name;

    unsigned long int weight_in_grams;
    bool sex;

    std::time_t date_of_birth;
    std::time_t last_veterinary_check;
    std::time_t date_of_weighting;

    ExternalKey<Species> species;
    ExternalKey<Keeper> keeper;
    ExternalKey<Enclosure> enclosure;

    Animal(){};

    Animal(std::string name,
       unsigned long int weight_in_grams,
       bool sex,
       std::time_t date_of_birth,
       std::time_t last_veterinary_check,
       std::time_t date_of_weighting,
       ExternalKey<Species> species_key,
       ExternalKey<Keeper> keeper_key,
       ExternalKey<Enclosure> enclosure_key)
        : name(std::move(name)),
          weight_in_grams(weight_in_grams),
          sex(sex),
          date_of_birth(date_of_birth),
          last_veterinary_check(last_veterinary_check),
          date_of_weighting(date_of_weighting),
          species(std::move(species_key)),
          keeper(std::move(keeper_key)),
          enclosure(std::move(enclosure_key))
            {}

    void display() {
        std::cout << "\nName : " << name;
        std::cout << "\nWeight: " << weight_in_grams << "g";
        std::cout << "\nSex: " << (sex ? "Male" : "Female");

        std::cout << "\nDOB: " << formatTime(date_of_birth);
        std::cout << "\nLast Veterinary Check: " << formatTime(last_veterinary_check);
        std::cout << "\nWeighted: " << formatTime(date_of_weighting);


        const Species* s_ptr = species.get();
        if (s_ptr != nullptr) {
            std::cout << "Species: " << s_ptr->name << "\n";
        } else {
            std::cout << "Species: [Unassigned]\n";
        }

        const Keeper* k_ptr = keeper.get();
        if (k_ptr != nullptr) {
            std::cout << "Keeper: " << k_ptr->name << "\n";
        } else {
            std::cout << "Keeper: [Unassigned]\n";
        }

        const Enclosure* e_ptr = enclosure.get();
        if (e_ptr != nullptr) {
            std::cout << "Enclosure: " << e_ptr->name << "\n";
        } else {
            std::cout << "Enclosure: [Unassigned]\n";
        }
    }


};


class AnimalDB : public DB<Animal, MAX_ANIMALS> {

    std::function<Species*(KeyID)> species_resolver;
    std::function<KeyID(bool&)> species_picker;

    std::function<Keeper*(KeyID)> keeper_resolver;
    std::function<KeyID(bool&)> keeper_picker;

    std::function<Enclosure*(KeyID)> enclosure_resolver;
    std::function<KeyID(bool&)> enclosure_picker;

public:

    AnimalDB(
        std::function<Species*(KeyID)> species_resolver,
        std::function<KeyID(bool&)> species_picker,
        std::function<Keeper*(KeyID)> keeper_resolver,
        std::function<KeyID(bool&)> keeper_picker,
        std::function<Enclosure*(KeyID)> enclosure_resolver,
        std::function<KeyID(bool&)> enclosure_picker ) {

        this->species_resolver = species_resolver;
        this->species_picker = species_picker;
        this->keeper_resolver = keeper_resolver;
        this->keeper_picker = keeper_picker;
        this->enclosure_resolver = enclosure_resolver;
        this->enclosure_picker = enclosure_picker;
    }

    bool inputForm(Animal &new_object) override {

        new_object.name=getStringFromUser("Write name you want to add", true);
        new_object.date_of_birth = 50000;
        new_object.last_veterinary_check = 50000;
        new_object.date_of_weighting = 50000;

        new_object.sex=true;
        new_object.weight_in_grams=10000;

        bool success;

        new_object.species=ExternalKey(species_picker(success), species_resolver);
        if (!success) return false;

        new_object.keeper=ExternalKey(keeper_picker(success), keeper_resolver);
        if (!success) return false;

        new_object.enclosure=ExternalKey(enclosure_picker(success), enclosure_resolver);
        if (!success) return false;

        return true;
    };

    std::vector<TableV2Column> getBaseViewColums() override {
        return {{"ID", 'i'}, {"Name", 's'}, {"Sex", 's'}, {"Species", 's'}, {"Keeper", 's'}, {"Enclosure", 's'}, {"DOB", 's'}, {"Weight", 'i'}, {"Last veterinary check", 's'} };
    };

    std::function<void(TableV2&, Animal&)> getBaseViewAddItemsFunc() override {
        return [](TableV2& t, Animal& r) {
            t.addItem(r.ID);
            t.addItem(r.name);
            t.addItem(r.sex ? "Male" : "Female");

            Species *s = r.species.get();
            if (s != nullptr) {
                t.addItem(s->name);
            }else {
                t.addItem("Invalid key");
            }

            Keeper *k = r.keeper.get();
            if (k != nullptr) {
                t.addItem(k->name + " " + k->surname);
            }else {
                t.addItem("Invalid key");
            }

            Enclosure *e = r.enclosure.get();
            if (e != nullptr) {
                t.addItem(e->name);
            }else {
                t.addItem("Invalid key");
            }

            t.addItem(formatTime(r.date_of_birth));
            t.addItem(r.weight_in_grams);
            t.addItem(formatTime(r.last_veterinary_check));

        };
    };

    std::string convertToCSVLine(int index) override {
        std::stringstream ss;
        auto di=data[index];


        ss << di.ID << CSV_SEP;
        ss << di.name << CSV_SEP;
        ss << di.weight_in_grams << CSV_SEP;
        ss << di.sex << CSV_SEP;
        ss << di.date_of_birth << CSV_SEP;
        ss << di.last_veterinary_check << CSV_SEP;
        ss << di.date_of_weighting << CSV_SEP;
        ss << di.species.ID << CSV_SEP;
        ss << di.keeper.ID << CSV_SEP;
        ss << di.enclosure.ID;

        return ss.str();
    };

    std::string getCSVHeader() override {
        std::stringstream ss;
        ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Weight g" << CSV_SEP << "Sex" << CSV_SEP
           << "Date of Birth" << CSV_SEP << "Last Vet Check" << CSV_SEP << "Date of Weighting" << CSV_SEP
           << "SpeciesID" << CSV_SEP << "KeeperID" << CSV_SEP << "EnclosureID";
        return ss.str();
    }

    Animal getFromCSVline(std::string CSV_line) override {

        std::string name, temp;
        std::stringstream ss(CSV_line);
        Animal new_animal;

        std::getline(ss, temp, CSV_SEP);
        new_animal.ID = std::stoi(temp);

        std::getline(ss, temp, CSV_SEP);
        new_animal.name = temp;

        std::getline(ss, temp, CSV_SEP);
        new_animal.weight_in_grams = std::stoi(temp);

        std::getline(ss, temp, CSV_SEP);
        new_animal.sex= (bool) std::stoi(temp);

        std::getline(ss, temp, CSV_SEP);
        new_animal.date_of_birth = std::stoi(temp);

        std::getline(ss, temp, CSV_SEP);
        new_animal.last_veterinary_check = std::stoi(temp);

        std::getline(ss, temp, CSV_SEP);
        new_animal.date_of_weighting = std::stoi(temp);

        std::getline(ss, temp, CSV_SEP);
        new_animal.species = ExternalKey(std::stoi(temp), species_resolver);

        std::getline(ss, temp, CSV_SEP);
        new_animal.keeper = ExternalKey(std::stoi(temp), keeper_resolver);

        std::getline(ss, temp, CSV_SEP);
        new_animal.enclosure = ExternalKey(std::stoi(temp), enclosure_resolver);

        return new_animal;
    };
};


#endif //BESTIARYCPP_ANIMAL_H