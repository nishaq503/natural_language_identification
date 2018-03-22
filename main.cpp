/**
 * Started by Najib 3/21/18
 **/

#include "functions/functions.h"
#include "functions/bigint.h"
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Need at least 2 filenames!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> language_names;

    for (int i = 1; i < argc; ++i) {
        language_names.push_back(std::string(argv[i]));
    }

    std::string test_language = language_names.back();
    language_names.pop_back();

    std::string best_match = detect_language(test_language, language_names);

    std::cout << best_match << std::endl;

    return 0;
}
