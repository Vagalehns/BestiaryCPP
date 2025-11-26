//
// Created by arman on 11/21/2025.
//

#include <array>

#ifndef BESTIARYCPP_TABLESPARAMETERS_H
#define BESTIARYCPP_TABLESPARAMETERS_H

#define CSV_SEP ','
#define TOO_MANY_ITERS 100000

#ifdef _WIN32
    #define SLASH '\\'
#else
    #define SLASH '/'
#endif

#define MAX_REGIONS 40
#define MAX_SPECIES 30
#define MAX_KEEPERS 25
#define MAX_ENCLOSURE 50
#define MAX_ANIMALS 100


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

constexpr std::array<const char*, 10> SpeciesEmojiIcons = {
    "🦁", // Lion (Big Cats/Predators)
    "🐘", // Elephant (Large Herbivores)
    "🐒", // Monkey (Primates)
    "🐼", // Panda (Bears/Specialty)
    "🦒", // Giraffe (Savanna animals)
    "🦓", // Zebra (Equines)
    "🐧", // Penguin (Aquatic Birds/Cold climate)
    "🐊", // Crocodile (Amphibious Reptiles)
    "🐍", // Snake (Reptiles/Vivariums)
    "🦩"  // Flamingo (Avian/Exotic Birds)
};

constexpr std::array<const char*, 2> EnclosuresTypes = {
    "Aquarium",
    "Cage"
};

enum WeightMeasurement {
    _First = 0,
    Kilogram,
    Gram,
    Milligram,
    Pound,
    Ton,
    Ounce,
    _Last = Ounce
};

constexpr std::array<const char*, 6> WeightMeasurementPrefix = {
    "kg",
    "g",
    "mg",
    "lb",
    "t",
    "oz"
};

constexpr std::array<const char*, 6> WeightMeasurementFullName = {
    "Kilogram",
    "Gram",
    "Milligram",
    "Pound",
    "Ton",
    "Ounce",
};



#endif //BESTIARYCPP_TABLESPARAMETERS_H