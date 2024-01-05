#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <unordered_map>
#include <map>
#include <sstream>
#include "fields.h"
#include "methods.h"

std::unordered_map<std::string, std::string> loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return {};
    }

    std::unordered_map<std::string, std::string> data;
    std::string line;
    while (std::getline(file, line)) {
        // Assume each line in the file is in the format key=value
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            data[key] = value;
        }
    }
    return data;
}

std::wstring openFileW() {
    OPENFILENAMEW ofn;
    WCHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        return std::wstring(ofn.lpstrFile);
    }
    else {
        return L""; 
    }
}

std::string wideToString(const std::wstring& wstr) {
    if (wstr.empty()) {
        return "";
    }

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1, NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], -1, &strTo[0], size_needed, NULL, NULL);

    return strTo;
}

std::string openFile() {
    std::wstring wFilePath = openFileW();
    return wideToString(wFilePath);
}

void saveProcessedText(const std::string& originalFilename, const std::string& processedContent) {
    std::string outputFilename = originalFilename.substr(0, originalFilename.find_last_of('.')) + "-done.txt";
    std::ofstream outputFile(outputFilename);
    if (outputFile.is_open()) {
        outputFile << processedContent;
        std::cout << "Processed content saved to " << outputFilename << std::endl;
    }
    else {
        std::cerr << "Error saving processed content to file." << std::endl;
    }
}

std::string searchKey(const std::unordered_map<std::string, std::string>& data, const std::string& targetKey) {
    auto it = data.find(targetKey);
    if (it != data.end()) {
        return it->second;
    }
    return "";
}

template<typename Map>
std::string searchKey(const Map& data, const std::string& targetKey) {
    auto it = data.find(targetKey);
    if (it != data.end()) {
        return it->second;
    }
    return "";
}

int main() {
    std::map<std::string, std::string> fieldsData = fields_Data();
    std::map<std::string, std::string> methodsData = methods_Data();

    int choice = -1;
    while (choice != 0) {
        std::cout << "Choose an option:\n";
        std::cout << "0. Exit\n";
        std::cout << "1. Check only text\n";
        std::cout << "2. Check whole file\n\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        std::cin.ignore(); // Clear the newline character from the input buffer

        switch (choice) {
        case 1: {
            system("cls");

            std::string userInput;
            std::cout << "Enter a text to search ('0' to exit): ";
            std::getline(std::cin >> std::ws, userInput);

            while (userInput != "0") {
                bool found = false;

                if (methodsData.find(userInput) != methodsData.end()) {
                    system("cls");
                    // Found in methods.txt: 
                    std::cout << "Answer is: " << methodsData[userInput] << "\n" << std::endl;
                    found = true;
                }

                if (fieldsData.find(userInput) != fieldsData.end()) {
                    system("cls");
                    // Found in fields.txt: 
                    std::cout << "Answer is: " << fieldsData[userInput] << "\n" << std::endl;
                    found = true;
                }

                if (!found) {
                    system("cls");
                    // Key not found in both files.
                    std::cout << "No answer found." << "\n\n" << std::endl;
                }

                std::cout << "Enter a text to search ('0' to exit): ";
                std::getline(std::cin >> std::ws, userInput);
            }

            std::cout << "Exiting the text search." << "\n\n" << std::endl;
            system("cls");
            break;
        }
        case 2: {
            std::string selectedFile = openFile();
            if (!selectedFile.empty()) {
                std::ifstream file(selectedFile);
                if (!file.is_open()) {
                    std::cerr << "Error opening file: " << selectedFile << std::endl;
                    break;
                }

                std::ostringstream processedContent;
                std::string line;
                bool modified = true;

                while (std::getline(file, line)) {
                    modified = true;

                    while (modified) {
                        modified = false;

                        size_t pos = 0;
                        while ((pos = line.find("func_", pos)) != std::string::npos) {
                            // Check if func_ is followed by digits and an underscore
                            size_t numStart = pos + 5;
                            while (numStart < line.size() && isdigit(line[numStart])) {
                                ++numStart;
                            }
                            if (numStart < line.size() && line[numStart] == '_' && numStart + 1 < line.size() && isalpha(line[numStart + 1])) {
                                size_t endPos = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789", numStart + 1);
                                if (endPos != std::string::npos) {
                                    std::string key = line.substr(pos, endPos - pos);
                                    std::string replacement = searchKey(methodsData, key);
                                    if (!replacement.empty()) {
                                        line.replace(pos, endPos - pos, replacement);
                                        modified = true;
                                    }
                                    pos += replacement.length(); // Move position after replaced text
                                }
                            }
                            else {
                                pos += 5; // Move position to search for the next occurrence
                            }
                        }

                        pos = 0;
                        while ((pos = line.find("field_", pos)) != std::string::npos) {
                            // Check if field_ is followed by digits and an underscore
                            size_t numStart = pos + 6;
                            while (numStart < line.size() && isdigit(line[numStart])) {
                                ++numStart;
                            }
                            if (numStart < line.size() && line[numStart] == '_' && numStart + 1 < line.size() && isalpha(line[numStart + 1])) {
                                size_t endPos = line.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789", numStart + 1);
                                if (endPos != std::string::npos) {
                                    std::string key = line.substr(pos, endPos - pos);
                                    std::string replacement = searchKey(fieldsData, key);
                                    if (!replacement.empty()) {
                                        line.replace(pos, endPos - pos, replacement);
                                        modified = true;
                                    }
                                    pos += replacement.length(); // Move position after replaced text
                                }
                            }
                            else {
                                pos += 6; // Move position to search for the next occurrence
                            }
                        }
                    }

                    processedContent << line << std::endl;
                }

                system("cls");
                saveProcessedText(selectedFile, processedContent.str());
                std::cout << "Modified content saved to a new file." << std::endl;
            }
            else {
                system("cls");
                std::cout << "File selection canceled or error occurred." << std::endl;
            }
            std::cout << "\n";
            break;
        }
        case 0:
            std::cout << "Exiting the program." << std::endl;
            break;
        default:
            system("cls");
            std::cout << "Invalid choice. Please enter a valid option." << "\n\n" << std::endl;
            break;
        }
    }

    return 0;
}