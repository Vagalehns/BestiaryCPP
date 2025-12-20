//
// Created by arman on 11/21/2025.
//

#pragma once

#include <array>

#ifndef BESTIARYCPP_TABLESPARAMETERS_H
#define BESTIARYCPP_TABLESPARAMETERS_H

#define CSV_SEP ','
#define TOO_MANY_ITERS 100000

#define MAX_REGIONS 50
#define MAX_SPECIES 100
#define MAX_KEEPERS 80
#define MAX_ENCLOSURE 80
#define MAX_ANIMALS 200


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

constexpr std::array<const char *, 13> FirstLevelRegionStrings = {
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

constexpr std::array<const char *, 30> SpeciesEmojiIcons = {
    "🦁",
    "🐘",
    "🐒",
    "🐼",
    "🦒",
    "🦓",
    "🐧",
    "🐊",
    "🐍",
    "🦩",
    "🐺",
    "🐨",
    "🦘",
    "🦅",
    "🦉",
    "🐬",
    "🐋",
    "🦈",
    "🐙",
    "🐢",
    "🐸",
    "🦋",
    "🕷️",
    "🦥",
    "🦦",
    "🦭",
    "🦇",
    "🦚",
    "🐪",
    "🦍"
};

constexpr std::array<const char *, 12> EnclosuresTypes = {
    "Aquarium",
    "Cage",
    "Aviary",
    "Terrarium",
    "Paddock",
    "Tank",
    "Vivarium",
    "Pool",
    "Insectarium",
    "Petting Zoo",
    "Safari Plains",
    "Nocturnal House"
};

enum WeightMeasurement {
    Kilogram,
    Gram,
    Milligram,
    Pound,
    Ton,
    Ounce
};

constexpr std::array<const char *, 6> WeightMeasurementPrefix = {
    "kg",
    "g",
    "mg",
    "lb",
    "t",
    "oz"
};

constexpr std::array<const char *, 6> WeightMeasurementFullName = {
    "Kilogram",
    "Gram",
    "Milligram",
    "Pound",
    "Ton",
    "Ounce",
};

auto static genericStringSort = [](const std::string &A, const std::string &B) -> int {
    return B.compare(A);
};

auto static genericTimeSort = [](std::time_t A, std::time_t B) -> int {
    if (A > B) return 1;
    if (A < B) return -1;
    return 0;
};


auto static makeGenericStringFilter(const std::string &search_term, bool full_match) {
    return [search_term, full_match](const std::string &name) -> bool {
        if (full_match) {
            return name == search_term;
        }
        return name.substr(0, search_term.length()) == search_term;
    };
}


#endif //BESTIARYCPP_TABLESPARAMETERS_H
