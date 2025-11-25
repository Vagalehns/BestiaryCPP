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
    std::string address;

    Address(){};
    Address(std::string address) : address(address) {};
};

struct Keeper : DefaultStruct {

    std::string name;
    std::string surname;
    PhoneNumber phone_number;
    Address address;

    void display() {
        std::cout << "\n Name: " << name << " " << surname;
        std::cout << "\n Phone: " << phone_number.number;
        std::cout << "\n Address: " << address.address;
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
                new_object.surname=getStringFromUser("Write surname you want to add", true);
                new_object.phone_number=getStringFromUserWithPattern("Input phone number", PhoneNumber::Error_Message, PhoneNumber::checkPhoneNumber);
                new_object.address=getStringFromUser("Write address you want to add", true);

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
                t.addItem(r.address.address);
            };
        };

        std::string convertToCSVLine(int index) override {
            std::stringstream ss;
            auto di=data[index];
            ss<<di.ID<<CSV_SEP<<di.name<<CSV_SEP<<di.surname<<CSV_SEP<<di.address.address<<CSV_SEP<<di.phone_number.number;
            return ss.str();
        };

        std::string getCSVHeader() override {
            std::stringstream ss;
            ss << "ID" << CSV_SEP << "Name" << CSV_SEP << "Surname" << CSV_SEP << "Address" << CSV_SEP << "Phone number";
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
            new_keeper.address= temp;

            std::getline(ss, temp, CSV_SEP);
            new_keeper.phone_number= temp;

            return new_keeper;
        };
};

#endif //BESTIARYCPP_KEEPERS_H