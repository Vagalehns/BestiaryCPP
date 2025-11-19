//
// Created by armands on 18.11.25.
//

#ifndef BESTIARYCPP_DATABASE_H
#define BESTIARYCPP_DATABASE_H
#include <string>
#include <any>

#define MAX_COLUMNS 10



class Field {
    public:
        std::string name;

};

class Row {
    public:
        Field fields[10];
};

class Column {
    public:
        std::string name;



        bool addData(std::string data);

};

class Table {

    public:
        Column columns[MAX_COLUMNS];
        int columnCounter=0;

};

class Database {
    public:
        Table table;
};


#endif //BESTIARYCPP_DATABASE_H