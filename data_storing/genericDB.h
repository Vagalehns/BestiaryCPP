//
// Created by armands on 18.11.25.
//

#ifndef BESTIARYCPP_ANIMALFUNCTIONS_H
#define BESTIARYCPP_ANIMALFUNCTIONS_H

#include <chrono>
#include <random>
#include <string>
#include "../file_functions/fileFunctions.h"
#include "../TUI_functions/TUI_functions.h"


struct TableStyle;
struct Render;

typedef unsigned int KeyID;

struct DefaultStruct {
    bool filtered_out = false;
    KeyID ID;

    DefaultStruct() {
        filtered_out = false;
        ID = 0;
    };

    explicit DefaultStruct(KeyID id) : ID(id) {
    };

    static void display() {
    };
};


template<typename T>
struct ExternalKey {
    static_assert(std::is_base_of_v<DefaultStruct, T>);

    KeyID ID = 0;
    std::function<T*(KeyID)> getFunc;

    T *get() {
        return getFunc(ID);
    }

    [[nodiscard]] std::string to_string() const {
        return "Key: " + std::to_string(ID);
    }

    ExternalKey() {
        getFunc = [](KeyID ID) -> T * {
            return nullptr;
        };
        ID = 0;
    };

    ExternalKey(KeyID id) {
        ID = id;
    }

    ExternalKey(KeyID id, std::function<T*(KeyID)> getFunc) : ID(id), getFunc(getFunc) {
    };
};


template<typename DT, unsigned int MaxData>
class DB {
    static_assert(std::is_base_of_v<DefaultStruct, DT>);

protected:
    ~DB() = default;

    unsigned int counter = 0;
    unsigned int seed_value = std::chrono::system_clock::now().time_since_epoch().count();

    std::mt19937 rand_engine;
    std::uniform_int_distribution<unsigned int> key_dist;

    DT data[MaxData];
    unsigned int iter = 0;

public:
    DB() : rand_engine(seed_value) {
    }

    template<typename T, typename Pred>
    void sort(T DT::*memberPtr, Pred pred);

    template<typename T, typename Pred>
    void filterByField(T DT::*memberPtr, Pred pred);


    void resetFilter();

    void deleteFiltered();

    unsigned int countFiltered();

    unsigned int getByFilteredIndex(unsigned int filtered_index);

    DT *getByID(KeyID ID);

    bool removeAtIndex(unsigned int index);

    bool removeByID(KeyID ID);

    bool append(DT new_data);

    bool appendAutoID(DT new_data);

    bool checkIUniqueID(KeyID new_ID);

    KeyID getRandID();


    [[nodiscard]] bool isFull() const;

    [[nodiscard]] bool isEmpty() const;


    void clearData();

    bool addByForm();

    bool editByForm();

    KeyID pickByUser(bool &successful, const std::string &question);


    void iterStart();

    void iterNext();

    DT *iterGet();

    [[nodiscard]] bool iterEnded() const;

    // Display operations
    virtual std::vector<std::pair<std::string, char> > getViewColums(char view) =0;

    virtual std::vector<std::string> getAsStrings(DT &ref, char view) =0;

    KeyID display(char view = 0, bool picker = false, const std::string &title = "");

    Render renderTable(TableStyle ts, char view);

    virtual bool inputForm(DT &new_object, bool edit) =0;

    // File operations
    virtual std::string convertToCSVLine(int index) =0;

    virtual std::string getCSVHeader() =0;

    virtual DT getFromCSVline(std::string CSV_line) =0;

    virtual void sortWithOptions() =0;

    virtual void preparedSort(char option) =0;

    virtual void filterWithOptions() =0;

    virtual bool isRecordOrphan(DT &ref) =0;

    void deleteOrphanedRecords();

    bool saveToCSVFile(const std::filesystem::path &filename, const std::filesystem::path &location);

    bool readFromCSVFile(const std::filesystem::path &filename, const std::filesystem::path &location,
                         bool clear = true);
};

#include "genericDB.tpp"
#include "genericDBFile.tpp"
#include "genericDBDisplay.tpp"

#endif //BESTIARYCPP_ANIMALFUNCTIONS_H
