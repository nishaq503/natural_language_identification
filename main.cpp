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
    if (argc < 3) {
        std::cerr << "Need at least 2 filenames!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::string> training_languages, test_languages;

    for (int i = 1; i < 10; ++i)
        training_languages.push_back(std::string(argv[i]));
    
    for (int i = 10; i < argc; ++i)
        test_languages.push_back(std::string(argv[i]));
    
    for (size_t i = 0; i < test_languages.size(); ++i)
        std::cout << detect_language(test_languages[i], training_languages) << std::endl;
        std::cout << std::endl;
    
    return 0;
}
