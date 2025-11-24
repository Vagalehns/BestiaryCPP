//
// Created by armands on 14.11.25.
//

#ifndef BESTIARYCPP_BESTIARYAPP_H
#define BESTIARYCPP_BESTIARYAPP_H

#include "TUI_functions.h"
#include "tables/Region.h"

#include "TUI_functions/Menu.h"

inline bool echo(const std::string &str );

typedef struct{
    bool nofuzz=false;
    bool vocal=false;
    bool help=false;
} AppState ;

class BestiaryApp {

    public:

        Menu menu_start, menu_add_data, menu_view_data;

        AppState State;

        RegionDB regionDB;

        explicit BestiaryApp(AppState state);

        bool Run();

        static void Splashscreen();
};

#endif //BESTIARYCPP_BESTIARYAPP_H