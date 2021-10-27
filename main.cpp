#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>

int main() {
    std::vector<std::string> paths {"md5_rainbow_table_part_1.txt", "md5_rainbow_table_part_2.txt",
                                    "md5_rainbow_table_part_3.txt", "md5_rainbow_table_part_4.txt",
                                    "md5_rainbow_table_part_5.txt", "md5_rainbow_table_part_6.txt",
                                    "md5_rainbow_table_part_7.txt", "md5_rainbow_table_part_8.txt",
                                    "md5_rainbow_table_part_9.txt", "md5_rainbow_table_part_10.txt",
                                    "md5_rainbow_table_part_11.txt", "md5_rainbow_table_part_12.txt"};
    std::map<std::string, std::string> buf;
    for (auto& path : paths) {
        std::ifstream fin(path);
        if (!fin) {
            std::cerr << "File open error!\n";
        }
        std::string str;
        while (std::getline(fin, str)) {
            if (std::count(str.begin(), str.end(), '\"' != 4)) {
                continue;
            }

            // split str into first and second
            std::string::iterator firstEnd = std::find(str.begin() + 1, str.end(), '\"');
            std::string::iterator secondBegin = std::find(firstEnd + 1, str.end(), '\"');
            std::string first = std::string(str.begin() + 1, firstEnd);
            std::string second = std::string(secondBegin + 1, str.end() - 1);

            buf.insert(std::make_pair(first, second));
        }
        fin.close();
    }

    std::vector<std::string> cashs
            {
                    "8a48173d4982104d360434ba20f894ef", // commissaRies
                    "fffffe7f65e2dce722f279e4573310cb", // Depend (last value in last file)
                    "0000005ec43798404240e1f700343445", // Paradenitis (first value in first file)
                    "8a48113d4982104c360424aa20f894ef", // Not exists
            };
    for (auto& cash : cashs) {
        std::cout << "Cash: " << cash << std::endl;
        auto it = buf.find(cash);
        if (it != buf.end()) {
            std::cout << "Finded! Value: " << it->second << std::endl;
        }
        else {
            std::cout << "Not exists cash!" << std::endl;
        }
    }
    return 0;
}
