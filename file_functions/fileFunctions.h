//
// Created by arman on 11/26/2025.
//

#ifndef BESTIARYCPP_FILE_FUNCTIONS_H
#define BESTIARYCPP_FILE_FUNCTIONS_H

#include <filesystem>

namespace fs = std::filesystem;

bool checkIfDirectoryExists(const fs::path &path);

bool checkIfFileExists(const fs::path &path);

bool createFolder(const fs::path &path);

bool ensureFolderExists(const fs::path &path);

bool zipFolder(const fs::path &source_path, const fs::path &dest_path);

bool unzipFile(const fs::path &source_path, const fs::path &dest_path);

void removeFile(const fs::path &path);

fs::path getPathFromUser(const std::string &question, bool ignore_last_part = false);

#endif //BESTIARYCPP_FILE_FUNCTIONS_H
