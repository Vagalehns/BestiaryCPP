//
// Created by armands on 14.11.25.
//

#ifndef BESTIARYCPP_TUI_FUNCTIONS_H
#define BESTIARYCPP_TUI_FUNCTIONS_H

#define MAX_MENU_ITEMS 10

#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "styling_functions.h"

void clearConsole();

std::string generateTable(std::vector<std::string> data, int columns, bool borders=false);

int getIntFromUser(int min, int max, const std::string &question, bool has_upper_bound, bool has_lower_bound);
bool getConfirmationFromUser(const std::string &question);

bool checkForChar(const std::string &string, char c);
bool validateString(const std::string &string, const std::string &unallowed_chars);
std::string padString(std::string string_to_pad, unsigned short new_size, char pad_char, bool center);


std::string getStringFromUser(const std::string &question, bool with_confirmation, std::string unallowed_chars);
std::string getStringFromUser(const std::string &question, bool with_confirmation);


typedef struct  {
    std::string title;
    std::function<bool()> screen_to_call;
} MenuItem;

class Menu {
    public:
        MenuItem menu_items[MAX_MENU_ITEMS];
        char item_count=0;
        std::string menu_title;
        bool should_close=false;

        explicit Menu(std::string title, std::string exit_name="Back");


        bool addItem(MenuItem item);

        void open();
        void open(bool clear);

        bool close();

    private:
        int translateFromUserToIndex(int user_sel) const {
            return item_count-user_sel;
        }
};

struct TableColumn {
    std::string title;
    char type;
};

struct TableStyle {

    bool has_header=true;
    bool has_borders=true;
    bool type_colored=true;
    bool centered=true;


    char inner_border_ver_symb='|';
    char outer_border_ver_symb='#';
    char inner_border_hor_symb='=';
    char outer_border_hor_symb='#';

    char header_padding_symb=' ';
    char padding_symb=' ';

    std::string outer_border_col = WHITESTY;
    std::string inner_border_col = WHITESTY;
    std::string column_title_style = BGBLACKSTY BOLDSTY GREENSTY;

};

class Table {
public:
    std::vector<TableColumn> columns;
    std::vector<std::string> items;
    std::string last_render;
    TableStyle ts;

    Table() = default;

    Table(const std::vector<TableColumn>& cols) : columns(cols) {}

    template<typename T>
    void addItem(const T& value) {
        items.push_back(std::to_string(value));
    }

    void addItem(const std::string &value) {
        items.push_back(value);
    }

    void addItem(const char* value) {
        items.push_back(value);
    }

    void clearData() {
        items.clear();
        last_render.clear();
    }

    void render() {

        unsigned short longest_item_len=0;
        std::string table;

        for (const auto& elem : items) {

             if (elem.length()>longest_item_len) {
                 longest_item_len = elem.length();
             }

         }

        for (const auto& elem : columns) {

            if (elem.title.length()>longest_item_len) {
                longest_item_len = elem.title.length();
            }

        }

        longest_item_len+=4;

        int item_amount=items.size();
        int column_amount=columns.size();
        int table_hor_size= (longest_item_len+1) * column_amount + 1;


        std::string ver_div = ts.inner_border_col + ts.inner_border_ver_symb + ENDSTY;
        std::string outer_ver_div = ts.outer_border_col+ts.outer_border_ver_symb+ENDSTY;

        std::string hor_div = outer_ver_div + ts.inner_border_col + std::string(table_hor_size-2, ts.inner_border_hor_symb) + outer_ver_div;
        std::string outer_hor_div = ts.outer_border_col + std::string(table_hor_size, ts.outer_border_hor_symb) + ENDSTY;

        int row_amount = ( item_amount + column_amount - 1) / column_amount;



        //RENDER STARTS
        if (ts.has_borders) table.append(outer_hor_div + "\n");

        if (ts.has_header) {

            if (ts.has_borders) table.append(outer_ver_div);

            for (int i = 0; i < column_amount; ++i) {


                table.append( ts.column_title_style + padString(columns[i].title, longest_item_len, ts.header_padding_symb, ts.centered) + ENDSTY);

                if (i<column_amount-1) {
                    table.append(ver_div);
                }
            }


            if (ts.has_borders) table.append( outer_ver_div + "\n" + outer_hor_div + "\n");
        }




        for (int i = 0; i < row_amount; ++i) {

            if (ts.has_borders) table+=  outer_ver_div ;

            for (int j = 0; j < column_amount; ++j) {

                if (ts.has_borders && j) table+= ver_div;

                int table_index = i*column_amount+j ;

                if (table_index<item_amount) {

                    if (ts.type_colored) {
                        switch (columns[j].type) {
                            case 'i':
                                table.append(CYANSTY);
                                break;
                            case 'f':
                                table.append(YELLOWSTY);
                                break;
                            case 's':
                                table.append(REDSTY);
                                break;
                        }


                    }

                    table.append(padString(items[table_index], longest_item_len, ts.padding_symb, ts.centered));

                    if (ts.type_colored) {
                        table.append(ENDSTY);
                    }

                }else {
                    table.append(std::string(longest_item_len, ' '));
                }

            }

            if (ts.has_borders) table+=  outer_ver_div;

            if (ts.has_borders) {

                table.append("\n");

                if (i<row_amount-1) {
                    table.append(hor_div);
                }else {
                    table.append(outer_hor_div);
                }

            }

            table.append("\n");
        }

        last_render=table;
    }
};

#endif //BESTIARYCPP_TUI_FUNCTIONS_H