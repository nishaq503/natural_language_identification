/**
 * Started by Najib 3/21/18
 **/

#include "functions.h"
#include "bigint.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>

bool read_next_char(std::ifstream &infile, char &ch, unsigned int &i) {
    if (!infile.get(ch)) return false;

    if (ch == ' ') i = 0;
    else if (ch >= 'a' && ch <= 'z') i = (unsigned int)(ch - 'a' + 1);
    else if (ch >= 'A' && ch <= 'Z') i = (unsigned int)(ch - 'A' + 1);
    else return false;

    return true;
}

std::vector<unsigned int> get_frequency_vector(const std::string file_name) {
    std::vector <unsigned int> frequency(27*27*27, 0);
    char c;
    unsigned int i1, i2, i3;
    
    std::ifstream infile(file_name);

    if (!(read_next_char(infile, c, i1) && read_next_char(infile, c, i2) && read_next_char(infile, c, i3))) {
        std::cerr << "Not enougn characters in " << file_name << std::endl;
        exit(EXIT_FAILURE);
    }

    do {
        ++frequency[27*27*i1 + 27*i2 + i3];
        i1 = i2;
        i2 = i3;
    } while (read_next_char(infile, c, i3));

    return frequency;
}

bigint get_dot(const std::vector<unsigned int> &frequency_A, const std::vector<unsigned int> &frequency_B) {
    bigint dot;
    for (size_t i = 0; i < frequency_A.size(); ++i) {
        bigint a = frequency_A[i];
        bigint b = frequency_B[i];
        dot += (a * b);
    }
    return dot;
}

void scale_bigint(bigint & a, unsigned int n) {
    std::vector<vec_bin> number = a.getNumber();
    std::reverse(number.begin(), number.end());

    for (unsigned int i = 0; i < n; ++i)
        number.push_back(0);
    
    std::reverse(number.begin(), number.end());
    a = bigint(number);

    return;
}

double get_similarity(const bigint norm_A, const std::vector<unsigned int> &frequency_A, const std::vector<unsigned int> &frequency_B) {
    bigint norm_B = get_dot(frequency_B, frequency_B);
    bigint dot = get_dot(frequency_A, frequency_B);

    bigint top = dot.fast_pow(2);
    scale_bigint(top, 6);

    bigint bottom = norm_A * norm_B;

    bigint sim = top / bottom;

    std::string num = sim.to_string();
    double similarity = (double)std::stoi(num) / (double)1000000;

    return sqrt(similarity);
}

std::string detect_language(const std::string &test_language, const std::vector<std::string> &language_names) {
    std::string best_match = "Nothing to see here!";
    double similarity = 0.0;

    std::vector<unsigned int> frequency_A = get_frequency_vector(test_language);
    bigint norm_A = get_dot(frequency_A, frequency_A);

    for (size_t i = 0; i < language_names.size(); ++i) {
        std::vector<unsigned int> frequency_B = get_frequency_vector(language_names[i]);
        double temp = get_similarity(norm_A, frequency_A, frequency_B);

        if (temp > similarity) {
            similarity = temp;
            best_match = language_names[i];
        }
    }
    std::cout << test_language << std::endl;
    std::cout << "similarity = " << similarity << std::endl;
    return best_match;
}
