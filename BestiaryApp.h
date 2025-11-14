//
// Created by armands on 14.11.25.
//

#ifndef BESTIARYCPP_BESTIARYAPP_H
#define BESTIARYCPP_BESTIARYAPP_H

#include "TUI_functions.h"

inline bool echo(const std::string &str );

typedef struct{
    bool nofuzz=false;
    bool vocal=false;
    bool help=false;
} AppState ;

class BestiaryApp {

    public:

        Menu StartMenu;
        AppState State;

        explicit BestiaryApp(AppState state);

        bool Run();

        bool Splashscreen() const ;
};

#endif //BESTIARYCPP_BESTIARYAPP_H