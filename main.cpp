#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <ctime>
#include <cassert>

// return either value that matches hash or defaultRet if there is no such hash
std::string findHash(const std::string& hash, std::string defaultRet = {});

int main() {
    int start = clock();
    assert(findHash("0000005ec43798404240e1f700343445") == "Paradenitis"); // file 1, first hash
    assert(findHash("fffffe7f65e2dce722f279e4573310cb") == "Depend"); // file 12, last hash
    assert(findHash("7343346e7d47275456f7abcbeb346b92") == "HYPERanarchic"); // file 5
    assert(findHash("1699251252c0638cfb6cdd15a125e151") == "barnstorm0"); // file 1
    assert(findHash("8a481197bfd5dcd57af298a4994dbc44") == "epicenters0"); // file 7
    assert(findHash("0000005ec43798404240e1f700313444") == ""); // no such hash (less than first hash in file 1)
    assert(findHash("fffffeff65e2dce722f279e4573310cb") == ""); // no such hash (greater than last hash in file 12)
    assert(findHash("ffffe7cc1b599b147891ba77703db2d3") == ""); // no such hash 
    assert(findHash("16991b9fa40bb8eeea927171aa4180aa", "nosuch") == "nosuch"); // no such hash
    int end = clock();
    std::cout << "Time - " << (double)(end - start) / 1000 << " sec" << std::endl;
    return 0;
}

std::string findHash(const std::string& hash, std::string defaultRet) {
    std::vector<std::string> pathsArr {"md5_rainbow_table_part_1.txt", "md5_rainbow_table_part_2.txt",
                                    "md5_rainbow_table_part_3.txt", "md5_rainbow_table_part_4.txt",
                                    "md5_rainbow_table_part_5.txt", "md5_rainbow_table_part_6.txt",
                                    "md5_rainbow_table_part_7.txt", "md5_rainbow_table_part_8.txt",
                                    "md5_rainbow_table_part_9.txt", "md5_rainbow_table_part_10.txt",
                                    "md5_rainbow_table_part_11.txt", "md5_rainbow_table_part_12.txt"};

    // Find the path to the file that contains the correct hash
    // Use binary search
    std::string rightPath {};
    long long int left = 0, right = pathsArr.size() - 1, middle;
    while (left <= right) {
        middle = (left + right) / 2;

        std::ifstream fin(pathsArr[middle]);
        if (!fin) {
            throw std::logic_error("File open error!");
        }
        std::string str {};
        while (std::count(str.begin(), str.end(), '\"') != 4) {
            std::getline(fin, str);
        }
        std::string firstHash(str.begin() + 1, std::find(str.begin() + 1, str.end(), '\"'));

        std::string lastHash {};
        while (std::getline(fin, str)) {
            if (std::count(str.begin(), str.end(), '\"') == 4) {
                lastHash = std::string(str.begin() + 1, std::find(str.begin() + 1, str.end(), '\"'));
            }
        }
        fin.close();

        if (hash < firstHash) {
            right = middle - 1;
        }
        else if (hash > lastHash) {
            left = middle + 1;
        }
        else {
            rightPath = pathsArr[middle];
            break;
        }
    }

    // read file in two vectors
    if (rightPath.empty()) {
        return defaultRet;
    }
    std::ifstream fin(rightPath);
    if (!fin) {
        throw std::logic_error("File open error!");
    }
    std::vector<std::string> hashsArr {};
    std::vector<std::string> valuesArr {};
    std::string str;
    while (std::getline(fin, str)) {
        if (std::count(str.begin(), str.end(), '\"' != 4)) {
            continue;
        }

        std::string::iterator hashEnd = std::find(str.begin() + 1, str.end(), '\"');
        hashsArr.push_back(std::string(str.begin() + 1, hashEnd));

        std::string::iterator valueBegin = std::find(hashEnd + 1, str.end(), '\"');
        valuesArr.push_back(std::string(valueBegin + 1, str.end() - 1));
    }
    fin.close();

    // Search hash in file
    left = 0, right = hashsArr.size() - 1, middle = 0;
    while (left <= right) {
        middle = (left + right) / 2;
        if (hash < hashsArr[middle]) {
            right = middle - 1;
        }
        else if (hash > hashsArr[middle]) {
            left = middle + 1;
        }
        else {
            return valuesArr[middle];
        }
    }
    return defaultRet;
}