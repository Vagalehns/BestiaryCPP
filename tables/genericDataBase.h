//
// Created by armands on 18.11.25.
//

#ifndef BESTIARYCPP_ANIMALFUNCTIONS_H
#define BESTIARYCPP_ANIMALFUNCTIONS_H

#include <fstream>
#include <sstream>
#include <string>

#include "../TUI_functions.h"

#include "dbParameters.h"
#include "tableV2.h"


typedef unsigned int KeyID;

struct DefaultStruct {
    bool filtered_out=false;
    KeyID ID;

    DefaultStruct() {
        filtered_out=false;
        ID=0;
    };

    DefaultStruct(KeyID id) : ID(id) {};

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
        KeyID ID_counter = 1;


    public:
        DT data[MaxData];


        template <typename T, typename Pred>
        void order(T DT::* memberPtr, Pred pred) {

                //Bubble sort for now
                static_assert(std::is_invocable_r_v<int, Pred, T, T>);

                bool sorted=false;
                int iter_counter=0;

                while (!sorted) {

                    if (iter_counter>TOO_MANY_ITERS) break;

                    sorted=true;

                    for (int i = 0; i < counter-1; i++) {

                        const T valueA = data[i].*memberPtr;
                        const T valueB = data[i+1].*memberPtr;

                        if (pred(valueA, valueB)<0) {
                            DT temp=data[i];
                            data[i]=data[i+1];
                            data[i+1]=temp;

                            sorted=false;
                        }

                    }

                    iter_counter++;
                }
            }

        template <typename T, typename Pred>
        void filterByField(T DT::* memberPtr, Pred pred) {

                for (int i = 0; i < counter; i++) {

                    const T& value = data[i].*memberPtr;
                    bool check_result = pred(value);

                    if (!check_result) data[i].filtered_out = true;
                }
            }

        DT* getByID(KeyID ID) {

            DT *obj=nullptr;

            for (int i = 0; i < counter; i++) {
                if (data[i].ID == ID) {
                    obj = &data[i];
                    return obj;
                }
            }

            return obj;
        }

        bool removeAtIndex(unsigned int index) {

            if (index>=counter) {
                return false;
            }

            //moves last to the index of deleted so there isnt any gap
            data[index] = data[counter-1];
            counter--;

            return true;
        };

        bool checkIUniqueID(KeyID new_ID) {

            for (int i = 0; i < counter; i++) {
                if (data[i].ID == new_ID) {
                    return false;
                }
            }

            return true;
        }

        bool appendAutoID(DT new_data) {

            do {
                new_data.ID = ID_counter++;
            } while (!checkIUniqueID(new_data.ID));

            return append(new_data);
        }

        bool isFull() {
            return counter>=MaxData;
        }



        bool append(DT new_data) {

            if (isFull()) {
                return false;
            }

            if (!checkIUniqueID(new_data.ID)) {
                return false;
            }


            data[counter] = new_data;
            counter++;

            return true;

        };

        void resetFilter() {
            for (int i=0; i<counter; i++) {
                data[i].filtered_out = false;
            }
        }

        virtual DT getFromCSVline(std::string CSV_line)=0;

        bool saveToCSVFile(std::string filename, std::string location) {

            std::ofstream out_file(location+SLASH+filename+".csv");

            if (!out_file) {
                std::cerr << "Error opening file for writing!" << std::endl;
                return false;
            }

            out_file << convertToCSV();
            out_file.close();

            return true;
        }

        void clearData() {
                counter = 0;
                ID_counter = 0;
            }

        bool readFromCSVFile(std::string filename, std::string location, bool clear=true) {

                std::ifstream in_file(location+SLASH+filename+".csv");

                if (!in_file) {
                    std::cerr << "Failed to open file." << std::endl;
                    return false;
                }

                if (clear) {
                    clearData();
                }

                std::string line;
                while (std::getline(in_file, line)) {
                    append(getFromCSVline(line));
                }

                in_file.close();

                return true;

            }

        int display(std::vector<TableV2Column> cols, std::function<void(TableV2&, DT&)> addItems, bool hide_filtered=true, bool picker=false, std::string question="") {

            TableV2 table(cols);

            for (int i = 0; i < counter; i++) {

                if (data[i].filtered_out && hide_filtered) continue;

                addItems(table, data[i]);
            }


            return table.show(picker, question);

        };

        bool addByForm() {
            if (isFull()) {
                return false;
            }

            DT new_dt;
            bool confirm;

            do {
                clearConsole();

                inputForm(new_dt);

                clearConsole();
                std::cout<<"Do you want to add?";
                new_dt.display();
                confirm = getConfirmationFromUser(">");

                if (!confirm) {
                    std::cout<<"Do you want to cancel?";
                    if (getConfirmationFromUser(">")) {
                        return false;
                    }
                }

            } while (!confirm);

            appendAutoID(new_dt);
            return true;
        }


        virtual std::string convertToCSV()=0;
        virtual KeyID pickByUser()=0;
        virtual bool inputForm(DT &new_object)=0;

};



#endif //BESTIARYCPP_ANIMALFUNCTIONS_H