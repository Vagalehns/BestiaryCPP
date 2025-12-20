//
// Created by arman on 11/24/2025.
//

#ifndef BESTIARYCPP_KEEPERS_H
#define BESTIARYCPP_KEEPERS_H

#include <utility>

#include "../genericDB.h"
#include "../../TUI_functions/TUI_functions.h"


struct PhoneNumber {
    std::string number;

    PhoneNumber() = default;

    PhoneNumber(std::string number) : number(std::move(number)) {
    };

    static const inline std::string Error_Message = "Number should be in format +###...#";

    static bool checkPhoneNumber(const std::string &number) {
        if (number[0] != '+') {
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

    Address() = default;

    Address(std::string city, std::string street, std::string zip_code) : street(std::move(street)),
                                                                          city(std::move(city)),
                                                                          zip_code(std::move(zip_code)) {
    };

    [[nodiscard]] std::string toString() const {
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

    void display() const {
        std::cout << "\n Name: " << name << " " << surname;
        std::cout << "\n Phone: " << phone_number.number;
        std::cout << "\n Address: " << address.city << ", " << address.street << ", " << address.zip_code;
    };

    Keeper() = default;

    Keeper(const std::string &name, const std::string &surname, const PhoneNumber &phone_number,
           const Address &address) {
        this->name = name;
        this->surname = surname;
        this->phone_number = phone_number;
        this->address = address;
    };
};


class KeeperDB : public DB<Keeper, MAX_KEEPERS> {
protected:


public:
    ~KeeperDB() = default;

    bool inputForm(Keeper &new_object, bool edit) override;

    void filterWithOptions() override;

    void preparedSort(char option) override;

    void sortWithOptions() override;

    std::vector<std::pair<std::string, char> > getViewColums(char view) override;


    std::vector<std::string> getAsStrings(Keeper &ref, char view) override;


    std::string convertToCSVLine(int index) override;

    std::string getCSVHeader() override;

    Keeper getFromCSVline(std::string CSV_line) override;

    bool isRecordOrphan(Keeper &ref) override;
};

#endif //BESTIARYCPP_KEEPERS_H
