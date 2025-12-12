#pragma once

#define MAX_PAGE_SIZE 14

#include "../TUI_functions.h"
#include "../styling_functions.h"

struct TableStyle {

        bool has_header=1;
        bool has_borders=1;
        bool type_colored=1;
        bool centered=1;
        bool has_hor_div=0;
        bool is_numbered=0;

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



template<typename DT, unsigned int MaxData>
std::pair<std::vector<std::string>, std::string> DB<DT, MaxData>::renderTable(TableStyle ts, char view) {

    std::vector<std::string> pages_rendered;
    std::string header;

    auto columns = this->getViewColums(view);
    std::vector<unsigned int> column_widths(columns.size());

    for (int i = 0; i < columns.size(); i++) {
        column_widths[i] = columns[i].first.length();
    }

    for (int i=0; i<counter; i++) {
        auto field_data = this->getAsStrings(data[i], view);

        for (int j=0; j<columns.size(); j++) {

            int len=field_data[j].length();

            if (len > column_widths[j]) {
                column_widths[j] = len;
            }

        }
    }

    int table_width = 0;

    for (auto& col : column_widths) {
        table_width += col;
    };

    table_width += (ts.extra_padding)*columns.size();

    if (ts.has_borders) {
        table_width += 1 + columns.size();
    }

    std::string ver_div = ts.inner_border_col + ts.inner_border_ver_symb + ENDSTY;;
    std::string outer_ver_div = ts.outer_border_col+ts.outer_border_ver_symb+ENDSTY;;
    std::string hor_div = outer_ver_div + ts.inner_border_col + std::string(table_width-2, ts.inner_border_hor_symb) + outer_ver_div; ;
    std::string outer_hor_div = ts.outer_border_col + std::string(table_width, ts.outer_border_hor_symb) + ENDSTY;

    unsigned int left_space_width;
    std::string empty_left_space;

    if (ts.is_numbered) {
        left_space_width = std::to_string(counter+1).length() + 1;
        empty_left_space = std::string(left_space_width, ' ');
    }

    if (ts.has_header) {

        if (ts.has_borders) {
            header.append( empty_left_space + outer_hor_div + "\n" + empty_left_space + outer_ver_div);
        };

        for (size_t i = 0; i < columns.size(); ++i) {

            header.append( ts.column_title_style + padString(columns[i].first, column_widths[i]+ts.extra_padding, ts.header_padding_symb, ts.centered) + ENDSTY);


            if (i<columns.size()-1 && ts.has_borders) {
                header.append(ver_div);
            }
        }

        if (ts.has_borders) {
            header.append( outer_ver_div + "\n" + empty_left_space + outer_hor_div);
        };

    }

    std::ostringstream cur_page("");
    unsigned int per_page_counter = -1;



    for (int i=0; i<counter; i++) {

        if (data[i].filtered_out) {continue;}

        per_page_counter++;

        if (ts.is_numbered) {
            cur_page << padString(std::to_string(i+1)+")", left_space_width, ts.padding_symb, true) ;
        }else {
            cur_page << empty_left_space;
        }

        if (ts.has_borders) cur_page << outer_ver_div ;



        auto field_data = getAsStrings(data[i], view);

        for (int j=0; j<columns.size(); j++) {

            if (ts.has_borders && j) cur_page << ver_div;

            if (ts.type_colored) {
                switch (columns[j].second) {
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


            cur_page << padString(field_data[j], column_widths[j]+ts.extra_padding, ts.padding_symb, ts.centered);

            if (ts.type_colored) {
                cur_page<<ENDSTY;
            }
        }

        if (ts.has_borders) cur_page << outer_ver_div;


        if (per_page_counter==MAX_PAGE_SIZE) {

            if (ts.has_borders) {
                cur_page << "\n" << empty_left_space <<  outer_hor_div;
            }

            per_page_counter=0;
            pages_rendered.emplace_back(cur_page.str());
            cur_page.str("");

        } else {
            if (ts.has_borders && ts.has_hor_div) {
                cur_page << "\n" << empty_left_space << hor_div;
            }
            cur_page<<"\n";
        }




    }

    if (ts.has_borders) {
        cur_page << empty_left_space << outer_hor_div;
    }

    pages_rendered.emplace_back(cur_page.str());

    return {pages_rendered, header};
};


template<typename DT, unsigned int MaxData>
KeyID DB<DT, MaxData>::display(char view, bool picker) {

    auto ts = TableStyle();

    if (picker) {
        ts.outer_border_ver_symb = '?';
        ts.outer_border_hor_symb = '?';
        ts.is_numbered = true;


        resetFilter();
    }


    auto [page_renders, header_render] = this->renderTable(ts, view);

    int cur_page=1;
    bool show_table = true;
    KeyID id_picked = 0;


    do {
        clearConsole();

        std::cout << header_render << std::endl;

        std::cout << page_renders[cur_page-1];

        std::cout << GREENSTY BOLDSTY "\nPage " << cur_page << " from " << page_renders.size() << ENDSTY;
        std::cout <<  "\nSelect page number, or use 0 (next), -1 (prev), -2 (sort), -3 (search/filter), -4 (delete currently selected), -5" ;

        if (picker) {
            std::cout << " (pick)";
        }else {
            std::cout << " (exit)";
        }

        std::cout << "\n";


        int user_choice=getIntFromUser(-5, page_renders.size(), ">", true, true);


        switch (user_choice) {
            case 0:
                if (cur_page!=page_renders.size()) cur_page+=1;
                break;
            case -1:
                if (cur_page!=1) cur_page-=1;
                break;

            case -2:
                this->sortOptions();
                page_renders= this->renderTable(ts, view).first;
                break;
            case -3:
                this->filterOptions();
                page_renders= this->renderTable(ts, view).first;
                break;

            case -4:
                if (getConfirmationFromUser("Confirm deletion")) {
                    if (counter==countFiltered()) {
                        if (getConfirmationFromUser("You have no filters. Do you want to delete everything?")) {
                            clearData();
                            show_table=false;
                        }
                    }else {
                        deleteFiltered();
                        resetFilter();
                        page_renders= this->renderTable(ts, view).first;

                    }
                };

                break;


            case -5:

                if (picker) {
                    std::cout << "Pick the item:\n";
                    id_picked=getIntFromUser(1, this->countFiltered(), ">", true, true);
                    id_picked=this->getByFilteredIndex(id_picked-1);
                    show_table=false;
                }else {
                    if (getConfirmationFromUser("Do you want to close the table?")) {
                        show_table=false;
                    }
                }



                break;
            default:
                cur_page=user_choice;
                break;
        }

    } while (show_table);

    return id_picked;
}

