#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

// Function to load data from a file into a map
std::unordered_map<std::string, std::string> loadData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return {};
    }

    std::unordered_map<std::string, std::string> data;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            data[key] = value;
        }
    }
    return data;
}

// Function to search for a given key in the map
std::string searchKey(const std::unordered_map<std::string, std::string>& data, const std::string& targetKey) {
    auto it = data.find(targetKey);
    if (it != data.end()) {
        return it->second;
    }
    return "";
}

int main() {
    std::unordered_map<std::string, std::string> methodsData = loadData("methods.txt");
    std::unordered_map<std::string, std::string> fieldsData = loadData("fields.txt");

    std::string userInput;
    while (true) {
        std::cout << "Enter a serge name: ";
        std::getline(std::cin, userInput);

        std::string foundMethod = searchKey(methodsData, userInput);
        std::string foundField = searchKey(fieldsData, userInput);

        if (!foundMethod.empty()) {
            std::cout << "Found in methods.txt: " << foundMethod << std::endl;
        }
        else if (!foundField.empty()) {
            std::cout << "Found in fields.txt: " << foundField << std::endl;
        }
        else {
            std::cout << "Key not found in both files." << std::endl;
        }
        std::cout << "\n";
    }

    return 0;
}
