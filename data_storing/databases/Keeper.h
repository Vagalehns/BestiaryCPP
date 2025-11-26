//
// Created by arman on 11/24/2025.
//

#ifndef BESTIARYCPP_KEEPERS_H
#define BESTIARYCPP_KEEPERS_H

#include "../genericDataBase.h"
#include "../tableV2.h"
#include "../../TUI_functions.h"


struct PhoneNumber {
    std::string number;

    PhoneNumber(){};

    PhoneNumber(std::string number) : number(number) {};

    static const inline std::string Error_Message="Number should be in format +###...#";

    static bool checkPhoneNumber(std::string number) {

        if (number[0]!='+') {
            return false;
        }

        for (int i = 1; i < number.length(); ++i) {
            if (!std::isdigit(number[i])) {
                return false;
            }
        }

        return true;
    }

};

struct Address {
    std::string street;
    std::string city;
    std::string zip_code;

    Address(){};
    Address( std::string city, std::string street, std::string zip_code) :
            city(std::move(city)), street(std::move(street)), zip_code(std::move(zip_code)) {};

    std::string toString() {
        std::stringstream ss;
        ss << city << "," << street << "," << zip_code;
        return ss.str();
    }
};

struct Keeper : DefaultStruct {

    std::string name;
    std::string surname;
    PhoneNumber phone_number;
    Address address;

    void display() {
        std::cout << "\n Name: " << name << " " << surname;
        std::cout << "\n Phone: " << phone_number.number;
        std::cout << "\n Address: " << address.city << ", " << address.street << ", " << address.zip_code;
    };

    Keeper(){};

    Keeper(std::string name, std::string surname, PhoneNumber phone_number, Address address) {
        this->name = name;
        this->surname = surname;
        this->phone_number = phone_number;
        this->address = address;
    };
};


class KeeperDB : public DB<Keeper, MAX_KEEPERS> {
public:

        bool inputForm(Keeper &new_object) override {

                new_object.name=getStringFromUser("Write name you want to add", true);
                clearConsole();
                new_object.surname=getStringFromUser("Write surname you want to add", true);
                clearConsole();
                new_object.phone_number=getStringFromUserWithPattern("Input phone number", PhoneNumber::Error_Message, PhoneNumber::checkPhoneNumber);
                clearConsole();
                std::cout << "Enter address:\n";
                new_object.address.city=getStringFromUser("Write city", true);
                new_object.address.street=getStringFromUser("Write street and number", true);
                new_object.address.zip_code=getStringFromUser("Write zip code", true);
                return true;
        };



        std::vector<TableV2Column> getBaseViewColums() override {
                return {{"ID", 'i'}, {"Name", 's'}, {"Surname", 's'}, {"Phone number", 's'},  {"Address", 's'}};
            };

        std::function<void(TableV2&, Keeper&)> getBaseViewAddItemsFunc() override {
            return [](TableV2& t, Keeper& r) {
                t.addItem(r.ID);
                t.addItem(r.name);
                t.addItem(r.surname);
                t.addItem(r.phone_number.number);
                t.addItem(r.address.toString());
            };
        };

        std::string convertToCSVLine(int index) override {
            std::stringstream ss;
            auto di=data[index];
            ss<<di.ID<<CSV_SEP<<di.name<<CSV_SEP<<di.surname << CSV_SEP
            << di.address.city << CSV_SEP << di.address.street << CSV_SEP << di.address.zip_code
            <<CSV_SEP<<di.phone_number.number;
            return ss.str();
        };

        std::string getCSVHeader() override {
            std::stringstream ss;
            ss  << "ID" << CSV_SEP
                << "Name" << CSV_SEP
                << "Surname" << CSV_SEP
                << "Address.city" << CSV_SEP << "Address.street" << CSV_SEP << "Address.zipcode" << CSV_SEP
                << "Phone number";
            return ss.str();
        };

        Keeper getFromCSVline(std::string CSV_line) override {
            std::string name, temp;
            std::stringstream ss(CSV_line);
            Keeper new_keeper;

            std::getline(ss, temp, CSV_SEP);
            new_keeper.ID = std::stoi(temp);

            std::getline(ss, temp, CSV_SEP);
            new_keeper.name = temp;

            std::getline(ss, temp, CSV_SEP);
            new_keeper.surname = temp;

            std::getline(ss, temp, CSV_SEP);
            new_keeper.address.city= temp;
            std::getline(ss, temp, CSV_SEP);
            new_keeper.address.street= temp;
            std::getline(ss, temp, CSV_SEP);
            new_keeper.address.zip_code= temp;

            std::getline(ss, temp, CSV_SEP);
            new_keeper.phone_number= temp;

            return new_keeper;
        };
};

#endif //BESTIARYCPP_KEEPERS_H