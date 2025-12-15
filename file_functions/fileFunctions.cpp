//
// Created by arman on 11/26/2025.
//

#include "fileFunctions.h"

#include <iostream>

#include "../TUI_functions/TUI_functions.h"

bool checkIfDirectoryExists(fs::path path) {
    return fs::is_directory(path);
};

bool checkIfFileExists(fs::path path) {
    return fs::is_regular_file(path);
};


bool createFolder(fs::path path) {
    return fs::create_directory(path);
};

bool ensureFolderExists(fs::path path) {
    if (checkIfDirectoryExists(path)) {
        return true;
    }

    return createFolder(path);
}

bool zipFolder(fs::path source_path, fs::path dest_path) {

    // tar command syntax: tar -a -c -f <output.zip> <folder/files>
    // -a: auto-detect archive format from extension (.zip)
    // -c: create archive
    // -f: specify archive file name
    // Enclosing paths in double quotes handles spaces
    std::string command = "tar.exe -a -c -f \"" + dest_path.string() + "\" -C \"" + source_path.parent_path().string() + "\" \"" + source_path.filename().string() + "\"";
    int result = std::system(command.c_str());

    return result == 0;
}

bool unzipFile(fs::path source_path, fs::path dest_path) {

    // tar command syntax: tar -a -x -f <input.zip> -C <destination_folder>
    // -a: auto-detect archive format from extension (.zip, .tar.gz, etc.)
    // -x: extract archive
    // -f: specify archive file name
    // -C: change to the destination directory before extracting
    // Enclosing paths in double quotes handles spaces

    if (!checkIfDirectoryExists(dest_path)) {
        return false;
    }

    std::string command = "tar.exe -a -x -f \"" + source_path.string() + "\" -C \"" + dest_path.string() + "\"";
    int result = std::system(command.c_str());

    return result == 0;
};

void removeFile(fs::path path) {
    std::filesystem::remove_all(path);
}

fs::path getPathFromUser(std::string question, bool ignore_last_part) {

    return getStringFromUserWithPattern(question, "Not a valid path", [ignore_last_part](std::string path)->bool {
        fs::path p(path);
        if (ignore_last_part) {
            return checkIfDirectoryExists(p.parent_path());
        }else {
            return checkIfDirectoryExists(p);
        }
    });
};
