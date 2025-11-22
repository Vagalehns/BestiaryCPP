//
// Created by arman on 11/21/2025.
//

#ifndef BESTIARYCPP_TABLESPARAMETERS_H
#define BESTIARYCPP_TABLESPARAMETERS_H

#define CSV_SEPERATOR ','
#define TOO_MANY_ITERS 1000

#ifdef _WIN32
    #define SLASH '\\'
#else
    #define SLASH '/'
#endif

#define MAX_REGIONS 40


enum FirstLevelRegion {
    ASIA,
    AFRICA,
    NORTH_AMERICA,
    SOUTH_AMERICA,
    ANTARCTICA,
    EUROPE,
    AUSTRALIA_OCEANIA,
    PACIFIC_OCEAN,
    ATLANTIC_OCEAN,
    INDIAN_OCEAN,
    SOUTHERN_OCEAN,
    ARCTIC_OCEAN,
    OCEANS
};

constexpr std::array<const char*, 13> FirstLevelRegionStrings = {
    "ASIA",
    "AFRICA",
    "NORTH_AMERICA",
    "SOUTH_AMERICA",
    "ANTARCTICA",
    "EUROPE",
    "AUSTRALIA_OCEANIA",
    "PACIFIC_OCEAN",
    "ATLANTIC_OCEAN",
    "INDIAN_OCEAN",
    "SOUTHERN_OCEAN",
    "ARCTIC_OCEAN",
    "OCEANS"
};



#endif //BESTIARYCPP_TABLESPARAMETERS_H