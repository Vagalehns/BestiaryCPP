#pragma once




template<typename DT, unsigned int MaxData>
KeyID DB<DT, MaxData>::pickByUser(bool &successful) {

    if (isEmpty()) {
        successful = false;
        return 0;
    }

    successful = true;

    KeyID id_picked= display(0, true);
    return data[id_picked].ID;
};


template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::editByForm() {
    if (isEmpty()) {
        return false;
    }

    unsigned int data_to_edit_index = display(0, true);

    auto &data_to_edit = data[data_to_edit_index];

    bool confirm = false;
    bool impossible = false;

    do {
        clearConsole();

        bool form_success = inputForm(data_to_edit, true);

        if (!form_success) {
            impossible=true;
            continue;
        }

        clearConsole();
        std::cout<<"Confirm edit?\n\n";

        data_to_edit.display();
        std::cout << "\n\n";
        confirm = getConfirmationFromUser(">");

        if (!confirm) {
            std::cout<<"Do you want to cancel?";
            if (getConfirmationFromUser(">")) {
                return false;
            }
        }

    } while (!confirm && !impossible);

    if (impossible) {
        return false;
    }

    return true;
};



template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::addByForm() {

    if (isFull()) {
        return false;
    }

    DT new_dt;
    bool confirm = false;
    bool impossible = false;

    do {
        clearConsole();

        bool form_success = inputForm(new_dt, false);

        if (!form_success) {
            impossible=true;
            continue;
        }

        clearConsole();
        std::cout<<"Do you want to add?\n\n";

        new_dt.display();
        std::cout << "\n\n";
        confirm = getConfirmationFromUser(">");

        if (!confirm) {
            std::cout<<"Do you want to cancel?";
            if (getConfirmationFromUser(">")) {
                return false;
            }
        }

    } while (!confirm && !impossible);

    if (impossible) {
        return false;
    }

    appendAutoID(new_dt);
    return true;
}






template<typename DT, unsigned int MaxData>
void DB<DT, MaxData>::clearData() {
    counter = 0;
}



template<typename DT, unsigned int MaxData>
DT* DB<DT, MaxData>::getByID(KeyID ID) {

    DT *obj=nullptr;

    for (int i = 0; i < counter; i++) {
        if (data[i].ID == ID) {
            obj = &data[i];
            return obj;
        }
    }

    return obj;
}

template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::removeAtIndex(unsigned int index) {

    if (index>=counter) {
        return false;
    }

    //moves last to the index of deleted so there isnt any gap
    data[index] = data[counter-1];
    counter--;

    return true;
}

template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::checkIUniqueID(KeyID new_ID) {

    for (int i = 0; i < counter; i++) {
        if (data[i].ID == new_ID) {
            return false;
        }
    }

    return true;
}

template<typename DT, unsigned int MaxData>
KeyID DB<DT, MaxData>::getRandID() {
    return key_dist(rand_engine);
};

template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::appendAutoID(DT new_data) {

    do {
        new_data.ID = getRandID();
    } while (!checkIUniqueID(new_data.ID));

    return append(new_data);
}

template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::isFull() {
    return counter>=MaxData;
}

template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::isEmpty() {
    return counter==0;
}

template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::append(DT new_data) {

    if (isFull()) {
        return false;
    }

    if (!checkIUniqueID(new_data.ID)) {
        return false;
    }


    data[counter] = new_data;
    counter++;

    return true;

};

template<typename DT, unsigned int MaxData>
void DB<DT, MaxData>::resetFilter() {
    for (int i=0; i<counter; i++) {
        data[i].filtered_out = false;
    }
}

template<typename DT, unsigned int MaxData>
void DB<DT, MaxData>::deleteFiltered() {
    for (int i=0; i<counter;) {
        if (data[i].filtered_out==false) {
            removeAtIndex(i);
        }else {
            i++;
        }
    }
}

template<typename DT, unsigned int MaxData>
unsigned int DB<DT, MaxData>::countFiltered() {

    unsigned int count = 0;

    for (int i=0; i<counter; i++) {
        if (data[i].filtered_out==false) count++;
    }

    return count;
};


template<typename DT, unsigned int MaxData>
unsigned int DB<DT, MaxData>::getByFilteredIndex(unsigned int filtered_index) {


    unsigned int correct_index = 0;
    unsigned int filtered_counter = 0;

    for (int i=0; i<counter; i++) {
        if (data[i].filtered_out==false) {


            if (filtered_counter == filtered_index) {
                correct_index = i;
                break;
            }

            filtered_counter++;


        }
    }

    return correct_index;
};



template<typename DT, unsigned int MaxData>
template <typename T, typename Pred>
void DB<DT, MaxData>::sort(T DT::* memberPtr, Pred pred) {

    //Bubble sort for now
    static_assert(std::is_invocable_r_v<int, Pred, T, T>);

    bool sorted=false;
    int iter_counter=0;

    while (!sorted) {

        if (iter_counter>TOO_MANY_ITERS) break;

        sorted=true;

        for (int i = 0; i < counter-1; i++) {

            const T valueA = data[i].*memberPtr;
            const T valueB = data[i+1].*memberPtr;

            if (pred(valueA, valueB)<0) {
                DT temp=data[i];
                data[i]=data[i+1];
                data[i+1]=temp;

                sorted=false;
            }

        }

        iter_counter++;
    }
}

template<typename DT, unsigned int MaxData>
template <typename T, typename Pred>
void DB<DT, MaxData>::filterByField(T DT::* memberPtr, Pred pred) {

    for (int i = 0; i < counter; i++) {

        const T& value = data[i].*memberPtr;
        bool check_result = pred(value);

        if (!check_result) data[i].filtered_out = true;
    }
}







