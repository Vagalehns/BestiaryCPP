//
// Created by arman on 11/21/2025.
//

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

constexpr std::array<const char*, 30> SpeciesEmojiIcons = {
    "🦁", // Lion (Big Cats/Predators)
    "🐘", // Elephant (Large Herbivores)
    "🐒", // Monkey (Primates)
    "🐼", // Panda (Bears/Specialty)
    "🦒", // Giraffe (Savanna animals)
    "🦓", // Zebra (Equines)
    "🐧", // Penguin (Aquatic Birds/Cold climate)
    "🐊", // Crocodile (Amphibious Reptiles)
    "🐍", // Snake (Reptiles/Vivariums)
    "🦩", // Flamingo (Avian/Exotic Birds)
    // --- New Additions ---
    "🐺", // Wolf (Canines/Pack animals)
    "🐨", // Koala (Marsupials/Arboreal)
    "🦘", // Kangaroo (Marsupials/Grazers)
    "🦅", // Eagle (Birds of Prey/Raptors)
    "🦉", // Owl (Nocturnal Birds)
    "🐬", // Dolphin (Cetaceans/Marine Mammals)
    "🐋", // Whale (Large Marine Mammals)
    "🦈", // Shark (Predatory Fish)
    "🐙", // Octopus (Cephalopods/Intelligence)
    "🐢", // Turtle/Tortoise (Shelled Reptiles)
    "🐸", // Frog (Amphibians)
    "🦋", // Butterfly (Insects/Pollinators)
    "🕷️", // Spider (Arachnids)
    "🦥", // Sloth (Slow Arboreal Mammals)
    "🦦", // Otter (Semi-aquatic Mammals)
    "🦭", // Seal (Pinnipeds)
    "🦇", // Bat (Flying Mammals/Nocturnal)
    "🦚", // Peacock (Ornamental Birds)
    "🐪", // Camel (Desert Dwellers)
    "🦍"  // Gorilla (Great Apes)
};

constexpr std::array<const char*, 12> EnclosuresTypes = {
    "Aquarium",     // Large building for aquatic life
    "Cage",         // General containment
    "Aviary",       // Large flight space for birds
    "Terrarium",    // Small glass enclosure for reptiles/bugs
    "Paddock",      // Fenced outdoor field for grazers
    "Tank",         // Water vessel (smaller than full Aquarium)
    "Vivarium",     // Integrated plant/animal ecosystem
    "Pool",         // Water enclosure for seals/otters
    "Insectarium",  // Specific facility for insects
    "Petting Zoo",    // Interactive low-fence area
    "Safari Plains",  // Large open-range area
    "Nocturnal House" // Darkened enclosure for night animals
};

enum WeightMeasurement {
    Kilogram,
    Gram,
    Milligram,
    Pound,
    Ton,
    Ounce
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

auto static genericStringSort = [](std::string A, std::string B) -> int {
    return B.compare(A);
};

auto static genericTimeSort = [](std::time_t A, std::time_t B) -> int {
    if (A>B) return 1;
    if (A<B) return -1;
    return 0;
};


auto static makeGenericStringFilter(std::string search_term, bool full_match) {
    return [search_term, full_match](std::string name) -> bool {
        if (full_match) {
            return name == search_term;
        }
        return name.substr(0, search_term.length())==search_term;
    };
}


#endif //BESTIARYCPP_TABLESPARAMETERS_H