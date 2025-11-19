//
// Created by armands on 18.11.25.
//

#ifndef BESTIARYCPP_DYNAMICLIST_H
#define BESTIARYCPP_DYNAMICLIST_H
#include <cstdlib>
#include <stdlib.h>


template <typename T>
class DynamicList {
public:

    T *list;
    unsigned int size = 0;
    unsigned int capacity = 0;



    bool addItem(T item) {

        if (capacity==0) {
            list = (T*) (malloc(sizeof(T)));
            capacity = 1;
        }

        if (size+1 > capacity) {
            list = (T*) realloc( list, sizeof(T) * capacity * 2);
            capacity *= 2;
        }

        list[size] = item;
        size++;

        return true;
    }

    T getItem(unsigned int index) {
        return list[index];
    }



};

#endif //BESTIARYCPP_DYNAMICLIST_H