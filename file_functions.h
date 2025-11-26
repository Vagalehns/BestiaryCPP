//
// Created by arman on 11/26/2025.
//

#ifndef BESTIARYCPP_FILE_FUNCTIONS_H
#define BESTIARYCPP_FILE_FUNCTIONS_H

#include <filesystem>

namespace fs = std::filesystem;

bool checkIfDirectoryExists(fs::path path);
bool checkIfFileExists(fs::path path);

bool createFolder(fs::path path);
bool ensureFolderExists(fs::path path);

bool zipFolder(fs::path source_path, fs::path dest_path);
bool unzipFile(fs::path source_path, fs::path dest_path);

void removeFile(fs::path path);

fs::path getPathFromUser(std::string question, bool ignore_last_part=false);

#endif //BESTIARYCPP_FILE_FUNCTIONS_H