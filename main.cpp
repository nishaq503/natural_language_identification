/**
 * Started by Najib 3/21/18
 **/

#include "functions/functions.h"
#include "functions/bigint.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if (argc < 11) {
        std::cerr << "Need all 14 training files and at least one testing file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> training_languages, test_languages;

    for (int i = 1; i < 15; ++i)
        training_languages.push_back(std::string(argv[i]));
    
    for (int i = 15; i < argc; ++i)
        test_languages.push_back(std::string(argv[i]));
    
    detect_language(training_languages, test_languages);

    return 0;
}
