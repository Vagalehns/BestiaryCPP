cmake_minimum_required(VERSION 4.0)
project(BestiaryCPP)

set(CMAKE_CXX_STANDARD 20)

add_executable(BestiaryCPP main.cpp
        info.h
        TUI_functions/TUI_functions.cpp
        TUI_functions/TUI_functions.h
        TUI_functions/styling_functions.h
        App.cpp
        App.h
        data_storing/genericDB.h
        data_storing/databases/Region.h
        data_storing/dbParameters.h
        TUI_functions/Menu.cpp
        TUI_functions/Menu.h
        data_storing/databases/Species.h
        data_storing/databases/Keeper.h
        data_storing/databases/Enclosure.h
        data_storing/databases/Animal.h
        file_functions/fileFunctions.cpp
        file_functions/fileFunctions.h
        data_storing/genericDBDisplay.tpp
        data_storing/databases/Enclosure.cpp
        data_storing/databases/Region.cpp
        data_storing/databases/Animal.cpp
        data_storing/databases/Keeper.cpp
        data_storing/databases/Species.cpp
)
