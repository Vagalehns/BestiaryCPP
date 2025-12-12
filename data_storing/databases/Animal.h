//
// Created by arman on 11/25/2025.
//

#ifndef BESTIARYCPP_ANIMAL_H
#define BESTIARYCPP_ANIMAL_H

#include <iomanip>

#include "Enclosure.h"
#include "Keeper.h"
#include "Species.h"
#include "../genericDatabase.h"
#include "../../TUI_functions.h"

struct Weight {

    unsigned long int weight_in_milligrams;
    WeightMeasurement preferred_measurement;

    static std::string getPrefix(WeightMeasurement measurement) {
        return WeightMeasurementPrefix[measurement];
    }

    static std::string getFullName(WeightMeasurement measurement) {
        return WeightMeasurementFullName[measurement];
    }

    Weight(){};

    Weight(unsigned long int value, WeightMeasurement measurement)
        : preferred_measurement(measurement) {
        setWeight(value, measurement);
    }

    // Gemini 2.5
    void setWeight(unsigned long int value, WeightMeasurement measurement) {
        switch (measurement) {
            case Kilogram:
                // 1 Kg = 1,000,000 mg
                this->weight_in_milligrams = value * 1000000UL;
                break;
            case Gram:
                // 1 g = 1,000 mg
                this->weight_in_milligrams = value * 1000UL;
                break;
            case Milligram:
                // 1 mg = 1 mg
                this->weight_in_milligrams = value;
                break;
            case Pound:
                // 1 lb approx 453,592 mg
                this->weight_in_milligrams = value * 453592UL;
                break;
            case Ton:
                // 1 Ton = 1,000,000,000 mg
                this->weight_in_milligrams = value * 1000000000UL;
                break;
            case Ounce:
                // 1 Oz approx 28,350 mg (using integer approximation)
                this->weight_in_milligrams = value * 28350UL;
                break;
            default:
                this->weight_in_milligrams = 0;
                break;
        }
    }

    std::string display() const {
        double display_value = 0.0;
        const double mg_value = weight_in_milligrams;

        switch (preferred_measurement) {
            case Kilogram:
                display_value = mg_value / 1000000.0;
                break;
            case Gram:
                display_value = mg_value / 1000.0;
                break;
            case Milligram:
                display_value = mg_value;
                break;
            case Pound:
                display_value = mg_value / 453592.0;
                break;
            case Ton:
                display_value = mg_value / 1000000000.0;
                break;
            case Ounce:
                display_value = mg_value / 28350.0;
                break;
            default:
                return "Unknown Measurement";
        }

        std::string suffix = getPrefix(preferred_measurement);
        std::stringstream ss;

        ss << std::fixed << std::setprecision(2) << display_value << suffix;

        return ss.str();
    }
};

struct Animal : DefaultStruct {

    std::string name;

    Weight weight;

    bool sex;

    std::time_t date_of_birth;
    std::time_t last_veterinary_check;
    std::time_t date_of_weighting;

    ExternalKey<Species> species;
    ExternalKey<Keeper> keeper;
    ExternalKey<Enclosure> enclosure;

    Animal(){};

    Animal(std::string name,
       Weight weight,
       bool sex,
       std::time_t date_of_birth,
       std::time_t last_veterinary_check,
       std::time_t date_of_weighting,
       ExternalKey<Species> species_key,
       ExternalKey<Keeper> keeper_key,
       ExternalKey<Enclosure> enclosure_key)
        : name(std::move(name)),
          weight(weight),
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
        std::cout << "\nWeight: " << weight.display();
        std::cout << "\nSex: " << (sex ? "Male" : "Female");

        std::cout << "\nDOB: " << formatTime(date_of_birth);
        std::cout << "\nLast Veterinary Check: " << formatTime(last_veterinary_check);
        std::cout << "\nWeighted: " << formatTime(date_of_weighting);


        const Species* s_ptr = species.get();
        if (s_ptr != nullptr) {
            std::cout << "\nSpecies: " << s_ptr->name << "\n";
        } else {
            std::cout << "\nSpecies: [Unassigned]\n";
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

        clearConsole();
        new_object.date_of_birth = getTimeFromUser("Write DOB");
        clearConsole();
        new_object.last_veterinary_check = getTimeFromUser("Write Last vet check");
        clearConsole();
        new_object.date_of_weighting = getTimeFromUser("Write Last date of weighing");;

        new_object.sex=true;


        WeightMeasurement wm = static_cast<WeightMeasurement>(getOptionFromUser(WeightMeasurementFullName, "Pick measurement type!"));
        new_object.weight=Weight(getIntFromUser(0, -1, "Input weight:", false, true ), wm);

        bool success;

        new_object.species=ExternalKey(species_picker(success), species_resolver);
        if (!success) return false;

        new_object.keeper=ExternalKey(keeper_picker(success), keeper_resolver);
        if (!success) return false;

        new_object.enclosure=ExternalKey(enclosure_picker(success), enclosure_resolver);
        if (!success) return false;

        return true;
    };

    std::vector< std::pair< std::string, char > > getBaseViewColums() override {
        return {{"ID", 'i'}, {"Name", 's'}, {"Sex", 's'}, {"Species", 's'}, {"Keeper", 's'}, {"Enclosure", 's'}, {"DOB", 's'}, {"Weight", 'i'}, {"Last veterinary check", 's'} };
    };

    std::vector<std::string> getAsStrings(Animal &ref) override {
        std::vector<std::string> result;

        result.push_back(std::to_string(ref.ID));
        result.push_back(ref.name);
        result.push_back(ref.sex ? "Male" : "Female");


        Species *s = ref.species.get();
        if (s != nullptr) {
            result.push_back(s->name);
        }else {
            result.push_back("Invalid key");
        }

        Keeper *k = ref.keeper.get();
        if (k != nullptr) {
            result.push_back(k->name + " " + k->surname);
        }else {
            result.push_back("Invalid key");
        }

        Enclosure *e = ref.enclosure.get();
        if (e != nullptr) {
            result.push_back(e->name);
        }else {
            result.push_back("Invalid key");
        }

        result.push_back(formatTime(ref.date_of_birth));
        result.push_back(ref.weight.display());
        result.push_back(formatTime(ref.last_veterinary_check));

        return result;
    }

    std::string convertToCSVLine(int index) override {
        std::stringstream ss;
        auto di=data[index];


        ss << di.ID << CSV_SEP;
        ss << di.name << CSV_SEP;
        ss << di.weight.weight_in_milligrams << CSV_SEP;
        ss << di.weight.preferred_measurement << CSV_SEP;
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
        ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Weight"  << CSV_SEP << "Weight.preferred_measurements" << CSV_SEP << "Sex" << CSV_SEP
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
        new_animal.weight.weight_in_milligrams = std::stoul(temp);

        std::getline(ss, temp, CSV_SEP);
        new_animal.weight.preferred_measurement = static_cast<WeightMeasurement>(std::stoi(temp));

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