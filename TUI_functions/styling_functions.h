//
// Created by armands on 14.11.25.
//


#ifndef BESTIARYCPP_STYLING_FUNCTIONS_H
#define BESTIARYCPP_STYLING_FUNCTIONS_H


#ifdef NOSTYLING
// If NOSTYLING is enabled, define everything as empty
#define CLEARSTYLE    ""
#define BLACKSTY      ""
#define REDSTY        ""
#define GREENSTY      ""
#define YELLOWSTY     ""
#define BLUESTY       ""
#define MAGENTASTY    ""
#define CYANSTY       ""
#define WHITESTY      ""

#define BRIGHTBLACKSTY  ""
#define BRIGHTREDSTY    ""
#define BRIGHTGREENSTY  ""
#define BRIGHTYELLOWSTY ""
#define BRIGHTBLUESTY   ""
#define BRIGHTMAGENTASTY ""
#define BRIGHTCYANSTY   ""
#define BRIGHTWHITESTY  ""

#define BOLDSTY      ""
#define ITALICSTY    ""
#define UNDERLINESTY ""

#define BGBLACKSTY      ""
#define BGREDSTY        ""
#define BGGREENSTY      ""
#define BGYELLOWSTY     ""
#define BGBLUESTY       ""
#define BGMAGENTASTY    ""
#define BGCYANSTY       ""
#define BGWHITESTY      ""

#define BGBRIGHTBLACKSTY  ""
#define BGBRIGHTREDSTY    ""
#define BGBRIGHTGREENSTY  ""
#define BGBRIGHTYELLOWSTY ""
#define BGBRIGHTBLUESTY   ""
#define BGBRIGHTMAGENTASTY ""
#define BGBRIGHTCYANSTY   ""
#define BGBRIGHTWHITESTY  ""
#define ENDSTY       ""
#else
// ANSI codes (same as your original definitions)
// Foreground colors (regular)
// Foreground colors (regular)
#define CLEARSTYLE    "" // Not used for color, but for consistency if needed
#define BLACKSTY      "\033[30m"
#define REDSTY        "\033[31m"
#define GREENSTY      "\033[32m"
#define YELLOWSTY     "\033[33m"
#define BLUESTY       "\033[34m"
#define MAGENTASTY    "\033[35m"
#define CYANSTY       "\033[36m"
#define WHITESTY      "\033[37m"

// Foreground colors (bright)
#define BRIGHTBLACKSTY  "\033[90m"
#define BRIGHTREDSTY    "\033[91m"
#define BRIGHTGREENSTY  "\033[92m"
#define BRIGHTYELLOWSTY "\033[93m"
#define BRIGHTBLUESTY   "\033[94m"
#define BRIGHTMAGENTASTY "\033[95m"
#define BRIGHTCYANSTY   "\033[96m"
#define BRIGHTWHITESTY  "\033[97m"

// Background colors
#define BGBLACKSTY      "\033[40m"
#define BGREDSTY        "\033[41m"
#define BGGREENSTY      "\033[42m"
#define BGYELLOWSTY     "\033[43m"
#define BGBLUESTY       "\033[44m"
#define BGMAGENTASTY    "\033[45m"
#define BGCYANSTY       "\033[46m"
#define BGWHITESTY      "\033[47m"

// Bright background colors
#define BGBRIGHTBLACKSTY  "\033[100m"
#define BGBRIGHTREDSTY    "\033[101m"
#define BGBRIGHTGREENSTY  "\033[102m"
#define BGBRIGHTYELLOWSTY "\033[103m"
#define BGBRIGHTBLUESTY   "\033[104m"
#define BGBRIGHTMAGENTASTY "\033[105m"
#define BGBRIGHTCYANSTY   "\033[106m"
#define BGBRIGHTWHITESTY  "\033[107m"

// Text formatting
#define BOLDSTY      "\033[1m"
#define ITALICSTY    "\033[3m"
#define UNDERLINESTY "\033[4m"

// Reset
#define ENDSTY       "\033[0m"
#endif

// Reset

#endif //BESTIARYCPP_STYLING_FUNCTIONS_H
