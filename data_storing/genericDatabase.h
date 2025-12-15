//
// Created by armands on 18.11.25.
//

#ifndef BESTIARYCPP_ANIMALFUNCTIONS_H
#define BESTIARYCPP_ANIMALFUNCTIONS_H

#include <chrono>
#include <random>
#include <string>
#include <fstream>
#include "../TUI_functions/TUI_functions.h"

#include "dbParameters.h"


struct TableStyle;

typedef unsigned int KeyID;

struct DefaultStruct {
    bool filtered_out=false;
    KeyID ID;

    DefaultStruct() {
        filtered_out=false;
        ID=0;
    };

    explicit DefaultStruct(KeyID id) : ID(id) {};

    void display() {

    };

};


template<typename T>
struct ExternalKey {

    static_assert(std::is_base_of<DefaultStruct, T>::value);

    KeyID ID = 0;
    std::function<T*(KeyID)> getFunc;

    T* get() {
        return getFunc(ID);
    }

    std::string to_string() const {
        return "Key: " + std::to_string(ID);
    }

    ExternalKey() {
        getFunc = [](KeyID ID) -> T* { return nullptr; };
        ID=0;
    };

    ExternalKey (KeyID id) {
        ID=id;
    }

    ExternalKey(KeyID id, std::function<T*(KeyID)> getFunc) : ID(id), getFunc(getFunc) {};
};


template<typename DT, unsigned int MaxData>
class DB {

    static_assert(std::is_base_of<DefaultStruct, DT>::value);

    protected:
        unsigned int counter = 0;
        unsigned int seed_value = std::chrono::system_clock::now().time_since_epoch().count();

        std::mt19937 rand_engine;
        std::uniform_int_distribution<unsigned int> key_dist;

        DT data[MaxData];



    public:

        DB() : rand_engine(seed_value){}

        template <typename T, typename Pred>
        void sort(T DT::* memberPtr, Pred pred);

        template <typename T, typename Pred>
        void filterByField(T DT::* memberPtr, Pred pred);



        void resetFilter();
        void deleteFiltered();
        unsigned int countFiltered();
        unsigned int getByFilteredIndex( unsigned int filtered_index);

        DT* getByID(KeyID ID);
        bool removeAtIndex(unsigned int index);
        bool checkIUniqueID(KeyID new_ID);
        KeyID getRandID();
        bool appendAutoID(DT new_data);
        bool isFull();
        bool isEmpty();
        bool append(DT new_data);



        void clearData();

        bool addByForm();
        bool editByForm();
        KeyID pickByUser(bool &successful);




        // Display operations
        virtual std::vector<std::pair< std::string, char>> getViewColums(char view) =0;
        virtual std::vector<std::string> getAsStrings(DT &ref, char view)=0;

        KeyID display(char view = 0, bool picker = false);
        std::pair<std::vector<std::string>, std::string> renderTable(TableStyle table_style, char view);

        virtual bool inputForm(DT &new_object, bool edit)=0;

        // File operations
        virtual std::string convertToCSVLine(int index)=0;
        virtual std::string getCSVHeader()=0;
        virtual DT getFromCSVline(std::string CSV_line)=0;

        virtual void sortOptions()=0;
        virtual void filterOptions()=0;

        bool saveToCSVFile(std::string filename, std::string location);
        bool readFromCSVFile(std::string filename, std::string location, bool clear=true);


};

#include "genericDatabase.tpp"

#include "genericDatabaseFileOperations.tpp"
#include "genericDatabaseDisplay.tpp"

#endif //BESTIARYCPP_ANIMALFUNCTIONS_H