//
// Created by armands on 20.11.25.
//

#ifndef BESTIARYCPP_ANIMAL_H
#define BESTIARYCPP_ANIMAL_H


#include "tables/genericDataBase.h"

#define MAX_ANIMALS 20
#define MAX_SPECIES 15


struct Species : DefaultStruct {
    std::string species;
    std::string latin_name;
    char type;

    Species(){};
    Species(std::string  s, std::string ln, char c):species(std::move(s)), latin_name(std::move(ln)), type(c){};
};


class SpeciesDB : public DB<Species, MAX_SPECIES> {

    public:

        void display(bool hide_filtered=true) override {
            Table table({ {"Species", 's'}, {"Latin name", 's'}, {"Type", 's'}, {"ID", 'i'} });


            for (int i = 0; i < counter; i++) {

                if (data[i].filtered_out && hide_filtered) continue;

                table.addItem(data[i].species);
                table.addItem(data[i].latin_name);
                table.addItem(data[i].type);
                table.addItem(data[i].ID);
            }
            table.render();
            std::cout<<table.last_render;
        };

        std::string convertToCSV() override {
            std::ostringstream oss;
            for (int i = 0; i < counter; i++) {
                oss << data[i].species << CSV_SEPERATOR << data[i].latin_name << CSV_SEPERATOR
                    << data[i].type ;
                if (i < counter - 1) oss << '\n';
            }
            return oss.str();
        }

        Species getFromCSVline(std::string CSV_line) override {
            std::string name, species, temp;

            std::stringstream ss(CSV_line);

            std::getline(ss, name, ',');
            std::getline(ss, species, ',');

            std::getline(ss, temp, ',');
            const unsigned int age = std::stoi(temp);

            std::getline(ss, temp, ',');
            const KeyID ID = std::stoi(temp);

            if (ID>ID_counter) ID_counter = ID+1;

            Species helpme;

            return helpme;
        }
};


struct Animal : DefaultStruct {
    std::string name;
    unsigned int age;
    ExternalKey<Species> SpeciesID;

    Animal(){};
    Animal(std::string  n, unsigned int a, ExternalKey<Species> id)
        : name(std::move(n)),
          age(a),
          SpeciesID(std::move(id)){};

    Animal(std::string  n, unsigned int a, KeyID id)
        : name(std::move(n)),
          age(a),
          SpeciesID(id){};

};

class AnimalDB : public DB<Animal, MAX_ANIMALS> {

private:
    std::function<Species*(KeyID)> species_resolver;


public:

    bool appendAutoID(Animal new_data) {

        new_data.SpeciesID.getFunc=species_resolver;

        return DB<Animal, MAX_ANIMALS>::appendAutoID(new_data);
    }


    AnimalDB(std::function<Species*(KeyID)> species_resolver) {
        this->species_resolver = species_resolver;
    };


    void display(bool hide_filtered=true) override {

        Table table({ {"Name", 's'}, {"Species", 's'}, {"Age", 'i'}, {"ID", 'i'} });


        for (int i = 0; i < counter; i++) {

            if (data[i].filtered_out && hide_filtered) continue;

            table.addItem(data[i].name);

            Species *species = data[i].SpeciesID.get();
            if (!(species==nullptr)) {
                table.addItem(species->latin_name);
            }else {
                table.addItem("Invalid species key");
            }

            table.addItem(data[i].age);
            table.addItem(data[i].ID);
        }
        table.render();
        std::cout<<table.last_render;

    };

    std::string convertToCSV() override {
        std::ostringstream oss;
        for (int i = 0; i < counter; i++) {

            Species *species = data[i].SpeciesID.get();

            oss << data[i].name << CSV_SEPERATOR;

            if (species==nullptr) {
                oss<<0;
            }else {
                oss<<species->ID;
            }

            oss << CSV_SEPERATOR << data[i].age << CSV_SEPERATOR << data[i].ID;

            if (i < counter - 1) oss << '\n';

        }
        return oss.str();
    }

    Animal getFromCSVline(std::string CSV_line) override {


        std::string name, temp;

        std::stringstream ss(CSV_line);

        std::getline(ss, name, ',');
        std::getline(ss, temp, ',');


        KeyID speciesID = std::stoi(temp);
        ExternalKey<Species> species;
        species.ID=speciesID;
        species.getFunc=species_resolver;

        std::getline(ss, temp, ',');
        const unsigned int age = std::stoi(temp);

        std::getline(ss, temp, ',');
        const KeyID ID = std::stoi(temp);



        if (ID>ID_counter) ID_counter = ID+1;

        Animal new_animal;
        new_animal.name = name;
        new_animal.age = age;
        new_animal.ID = ID;
        new_animal.SpeciesID=species;


        return new_animal;
    }

};



#endif //BESTIARYCPP_ANIMAL_H