//
// Created by armands on 18.11.25.
//

#ifndef BESTIARYCPP_ANIMALFUNCTIONS_H
#define BESTIARYCPP_ANIMALFUNCTIONS_H

#include <string>
#include <iostream>

#define SEPERATOR ';'



class Animal {
    private:
        std::string name;
        std::string species;
        unsigned int ID;
        unsigned char age;

    public:

        Animal() : name(""), species(""), ID(0), age(0) {}

        Animal(const std::string& name, const std::string& species, unsigned int ID, unsigned char age)
            : name(name), species(species), ID(ID), age(age) {}

        Animal(std::string saved_string) {

            size_t start = 0;
            size_t end = saved_string.find(SEPERATOR);
            char part_counter=0;

            while (end != std::string::npos) {

                part_counter++;

                std::string part = saved_string.substr(start, end - start);

                switch (part_counter) {
                    case 1:
                        name=part;
                        break;
                    case 2:
                        species=part;
                        break;
                    case 3:
                        ID=std::stoi(part);
                        break;
                    case 4:
                        age=std::stoi(part);
                        break;
                }

                start = end + 1;
                end = saved_string.find(SEPERATOR, start);
            }
        }

        std::string getName() const { return name; }
        std::string getSpecies() const { return species; }
        unsigned int getID() const { return ID; }
        unsigned char getAge() const { return age; }

        void setName(const std::string& newName) { name = newName; }
        void setSpecies(const std::string& newSpecies) { species = newSpecies; }
        void setID(unsigned int newID) { ID = newID; }
        void setAge(unsigned char newAge) { age = newAge; }

        void display() const {
            std::cout << "Name: " << name << "\n"
                      << "Species: " << species << "\n"
                      << "ID: " << ID << "\n"
                      << "Age: " << static_cast<int>(age) << "\n";
        }

        std::string savableString() const {
            return name + SEPERATOR + species + SEPERATOR + std::to_string(ID) + SEPERATOR + std::to_string(age) + SEPERATOR;
        }
};

#endif //BESTIARYCPP_ANIMALFUNCTIONS_H