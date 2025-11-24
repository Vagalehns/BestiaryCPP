//
// Created by arman on 11/22/2025.
//

#ifndef BESTIARYCPP_TABLEV2_H
#define BESTIARYCPP_TABLEV2_H

#include <algorithm>
#include <string>
#include <vector>
#include <cmath>

#include "../styling_functions.h"

#define MAX_PAGE_SIZE 14


struct TableV2Column {
    std::string title;
    char type;
    unsigned short width;

    TableV2Column(std::string title, char type) {
        this->title = std::move(title);
        this->type = type;

        width = this->title.length();
    };
};

struct TableV2Style {

    bool has_header=true;
    bool has_borders=true;
    bool type_colored=true;
    bool centered=true;
    bool has_hor_div=false;

    unsigned char extra_padding=4;


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

class TableV2 {

private:

    unsigned int table_width=0;
    std::string header_render;
    std::string ver_div;
    std::string outer_ver_div;
    std::string hor_div;
    std::string outer_hor_div;
    std::vector<std::string> page_renders;

    void recalcTableWidth() {

        table_width = 0;
        if (columns.empty()) {
            return;
        }

        for (auto& col : columns) {
            table_width += col.width;
        };

        table_width += (ts.extra_padding)*columns.size();

        if (ts.has_borders) {
            table_width += 1 + columns.size();
        }

    };

    void recalcColumnWidths() {

        size_t column_amount=columns.size();

        for (size_t i = 0; i < items.size(); ++i) {

            char cur_col=i%column_amount;

            if (items[i].length() > columns[cur_col].width) {

                columns[cur_col].width = items[i].length();

            }
        }
    };

public:
    std::vector<TableV2Column> columns;
    std::vector<std::string> items;
    std::string last_render;
    TableV2Style ts;

    TableV2() {
        last_render = "";
        header_render = "";
        ver_div = "";
        outer_ver_div = "";
        hor_div = "";
        outer_hor_div = "";

    };

    TableV2(const std::vector<TableV2Column>& cols) : columns(cols) {

        TableV2();

        recalcTableWidth();
    }



    void addItem(const std::string &value) {

        unsigned int cur_col = items.size()%columns.size();

        items.push_back(value);




        if (value.size()>columns[cur_col].width) {

            columns[cur_col].width = value.size();
        }
    }

    template<typename T>
    void addItem(const T& value) {
        addItem(std::to_string(value));
    }

    void addItem(const char* value) {
        addItem(std::string(value));
    }

    void clearData() {
        items.clear();
        last_render.clear();

        for (size_t i = 0; i < columns.size(); ++i) {
            columns[i].width = columns[i].title.length();
        }
    }

    void render_borders(){

        if (table_width==0) {
            return;
        }

        ver_div = ts.inner_border_col + ts.inner_border_ver_symb + ENDSTY;
        outer_ver_div = ts.outer_border_col+ts.outer_border_ver_symb+ENDSTY;



        hor_div = outer_ver_div + ts.inner_border_col + std::string(table_width-2, ts.inner_border_hor_symb) + outer_ver_div;
        outer_hor_div = ts.outer_border_col + std::string(table_width, ts.outer_border_hor_symb) + ENDSTY;
    }

    void render_header() {

        header_render="";

        if (columns.empty()) {
            return;
        }

        if (ts.has_borders) {
            header_render.append( outer_hor_div + "\n" + outer_ver_div);
        };

        for (size_t i = 0; i < columns.size(); ++i) {

            header_render.append( ts.column_title_style + padString(columns[i].title, columns[i].width+ts.extra_padding, ts.header_padding_symb, ts.centered) + ENDSTY);


            if (i<columns.size()-1 && ts.has_borders) {
                header_render.append(ver_div);
            }
        }
        if (ts.has_borders) {

            header_render.append( outer_ver_div + "\n" + outer_hor_div + "\n");
        };
    }

    void render() {

        recalcTableWidth();

        if (ts.has_borders) {
            render_borders();
        }

        if (ts.has_header) {
            render_header();
        }

        page_renders.clear();

        unsigned short col_amount = columns.size();
        unsigned int rows = std::ceil(items.size()/col_amount);
        unsigned int items_amount = items.size();


        std::ostringstream cur_page("");

        unsigned int per_page_counter = 0;

        for (int r = 0; r < rows; ++r, ++per_page_counter) {

            if (ts.has_borders) cur_page <<  outer_ver_div ;

            for (int c = 0; c < col_amount; ++c) {

                if (ts.has_borders && c) cur_page << ver_div;

                int cur_index = r*col_amount + c;
                TableV2Column &cur_col = columns[c];

                if (cur_index<items_amount) {

                    if (ts.type_colored) {
                        switch (cur_col.type) {
                            case 'i':
                                cur_page<<CYANSTY;
                                break;
                            case 'f':
                                cur_page<<YELLOWSTY;
                                break;
                            case 's':
                                cur_page<<REDSTY;
                                break;
                        }


                    }

                    cur_page<< padString(items[cur_index], cur_col.width+ts.extra_padding, ts.padding_symb, ts.centered);

                    if (ts.type_colored) {
                        cur_page<<ENDSTY;
                    }

                }else {
                    cur_page<<"";
                }


            }

            if (ts.has_borders) cur_page << outer_ver_div;
            if (ts.has_borders && ts.has_hor_div && per_page_counter<MAX_PAGE_SIZE) {
                cur_page << "\n" << hor_div;
            }

            cur_page<<"\n";

            if (per_page_counter==MAX_PAGE_SIZE) {
                per_page_counter=0;
                page_renders.emplace_back(cur_page.str());
                cur_page.str("");
            }

        }

        page_renders.emplace_back(cur_page.str());

    }

    void show() {
        render();

        int cur_page=1;

        bool show_table = true;

        do {
            clearConsole();

            if (ts.has_header) {
                std::cout << header_render;
            }
            std::cout << page_renders[cur_page-1];

            if (ts.has_borders) std::cout << outer_hor_div;

            std::cout << GREENSTY BOLDSTY "\nPage " << cur_page << " from " << page_renders.size() << ENDSTY;
            std::cout <<  "\nSelect page number, or use 0 (next), -1 (prev), -2 (exit)" << std::endl;

            int user_choice=getIntFromUser(-2, page_renders.size(), ">", true, true);


            switch (user_choice) {
                case 0:
                    if (cur_page!=page_renders.size()) cur_page+=1;
                    break;
                case -1:
                    if (cur_page!=1) cur_page-=1;
                    break;
                case -2:
                    if (getConfirmationFromUser("Do you want to close the table?")) {
                        show_table=false;
                    }
                    break;
                default:
                    cur_page=user_choice;
                    break;
            }

        } while (show_table);

    }
};
#endif //BESTIARYCPP_TABLEV2_H