//
// Created by arman on 11/25/2025.
//

#ifndef BESTIARYCPP_ANIMAL_H
#define BESTIARYCPP_ANIMAL_H

#include <iomanip>
#include <utility>

#include "Enclosure.h"
#include "Keeper.h"
#include "Species.h"

struct Weight {
    unsigned long int weight_in_mg{};
    WeightMeasurement preferred_measurement;

    static std::string getPrefix(WeightMeasurement measurement) {
        return WeightMeasurementPrefix[measurement];
    }

    static std::string getFullName(WeightMeasurement measurement) {
        return WeightMeasurementFullName[measurement];
    }

    Weight() : weight_in_mg(0), preferred_measurement(Kilogram) {
    } ;

    Weight(unsigned long int value, WeightMeasurement measurement)
        : preferred_measurement(measurement) {
        setWeight(value, measurement);
    }

    // Gemini 2.5
    void setWeight(unsigned long int value, WeightMeasurement measurement) {
        switch (measurement) {
            case Kilogram:
                // 1 Kg = 1,000,000 mg
                this->weight_in_mg = value * 1000000UL;
                break;
            case Gram:
                // 1 g = 1,000 mg
                this->weight_in_mg = value * 1000UL;
                break;
            case Milligram:
                // 1 mg = 1 mg
                this->weight_in_mg = value;
                break;
            case Pound:
                // 1 lb approx 453,592 mg
                this->weight_in_mg = value * 453592UL;
                break;
            case Ton:
                // 1 Ton = 1,000,000,000 mg
                this->weight_in_mg = value * 1000000000UL;
                break;
            case Ounce:
                // 1 Oz approx 28,350 mg (using integer approximation)
                this->weight_in_mg = value * 28350UL;
                break;
            default:
                this->weight_in_mg = 0;
                break;
        }
    }

    [[nodiscard]] std::string display() const {
        double display_value = 0.0;
        const double mg_value = weight_in_mg;

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

    Animal() : sex(false), date_of_birth(0), last_veterinary_check(0), date_of_weighting(0) {
    } ;

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
          enclosure(std::move(enclosure_key)) {
    }

    void display() {
        std::cout << "\nName : " << name;
        std::cout << "\nWeight: " << weight.display();
        std::cout << "\nSex: " << (sex ? "Male" : "Female");

        std::cout << "\nDOB: " << formatTime(date_of_birth);
        std::cout << "\nLast Veterinary Check: " << formatTime(last_veterinary_check);
        std::cout << "\nWeighted: " << formatTime(date_of_weighting);


        const Species *s_ptr = species.get();
        if (s_ptr != nullptr) {
            std::cout << "\nSpecies: " << s_ptr->name << "\n";
        } else {
            std::cout << "\nSpecies: [Unassigned]\n";
        }

        const Keeper *k_ptr = keeper.get();
        if (k_ptr != nullptr) {
            std::cout << "Keeper: " << k_ptr->name << "\n";
        } else {
            std::cout << "Keeper: [Unassigned]\n";
        }

        const Enclosure *e_ptr = enclosure.get();
        if (e_ptr != nullptr) {
            std::cout << "Enclosure: " << e_ptr->name << "\n";
        } else {
            std::cout << "Enclosure: [Unassigned]\n";
        }
    }
};


class AnimalDB : public DB<Animal, MAX_ANIMALS> {
    std::function<Species*(KeyID)> species_resolver;
    std::function<KeyID(bool &, std::string)> species_picker;

    std::function<Keeper*(KeyID)> keeper_resolver;
    std::function<KeyID(bool &, std::string)> keeper_picker;

    std::function<Enclosure*(KeyID)> enclosure_resolver;
    std::function<KeyID(bool &, std::string)> enclosure_picker;

public:
    ~AnimalDB() = default;

    AnimalDB(
        std::function<Species*(KeyID)> species_resolver,
        std::function<KeyID(bool &, std::string)> species_picker,
        std::function<Keeper*(KeyID)> keeper_resolver,
        std::function<KeyID(bool &, std::string)> keeper_picker,
        std::function<Enclosure*(KeyID)> enclosure_resolver,
        std::function<KeyID(bool &, std::string)> enclosure_picker) {
        this->species_resolver = std::move(species_resolver);
        this->species_picker = std::move(species_picker);
        this->keeper_resolver = std::move(keeper_resolver);
        this->keeper_picker = std::move(keeper_picker);
        this->enclosure_resolver = std::move(enclosure_resolver);
        this->enclosure_picker = std::move(enclosure_picker);
    };


    bool inputForm(Animal &new_object, bool edit) override;


    void filterWithOptions() override;

    void preparedSort(char option) override;

    void sortWithOptions() override;


    std::vector<std::pair<std::string, char> > getViewColums(char view) override;

    std::vector<std::string> getAsStrings(Animal &ref, char view) override;

    std::string convertToCSVLine(int index) override;

    std::string getCSVHeader() override;

    Animal getFromCSVline(std::string CSV_line) override;

    bool isRecordOrphan(Animal &ref) override;
};


#endif //BESTIARYCPP_ANIMAL_H
