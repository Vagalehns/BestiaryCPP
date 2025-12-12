

#pragma once

template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::readFromCSVFile(std::string filename, std::string location, bool clear) {

    std::ifstream in_file(location+SLASH+filename);

    if (!in_file) {
        std::cerr << "Failed to open file." << std::endl;
        return false;
    }

    if (clear) {
        clearData();
    }

    std::string line;
    std::getline(in_file, line); // Read header

    if (!(line==getCSVHeader())) {
        return false;
    }

    while (std::getline(in_file, line)) {
        DT new_data;
        try {
            new_data=getFromCSVline(line);
        }catch(...) {
            return false;
        }
        append(new_data);
    }

    in_file.close();

    return true;

}



template<typename DT, unsigned int MaxData>
bool DB<DT, MaxData>::saveToCSVFile(std::string filename, std::string location) {

    std::ofstream out_file(location+SLASH+filename);

    if (!out_file) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return false;
    }

    out_file<<getCSVHeader()<<std::endl;
    for (int i = 0; i < counter; i++) {
        out_file << convertToCSVLine(i) << std::endl;
    }

    out_file.close();

    return true;
}

