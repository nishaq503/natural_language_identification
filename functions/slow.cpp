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
        dot += (a.multiply(b));
    }
    return dot;
}

double get_similarity(const bigint norm_A, const std::vector<unsigned int> &frequency_A, const std::vector<unsigned int> &frequency_B) {
    bigint norm_B = get_dot(frequency_B, frequency_B);
    std::cout << "norm = " << norm_B << std::endl << std::flush;

    bigint dot = get_dot(frequency_A, frequency_B);
    std::cout << "dot = " << dot << std::endl << std::flush;

    bigint top = dot.pow(2);
    std::cout << "top = " << top << std::endl << std::flush;

    std::vector<vec_bin> temp;
    for (unsigned int i = 0; i < 6; ++i)
        temp.push_back(0);
    for (size_t i = 0; i < top.getNumber().size(); ++i)
        temp.push_back(top.getNumber()[i]);

    top = bigint(temp);
    std::cout << "scaled top = " << top << std::endl << std::flush;
    
    bigint bottom = norm_A.multiply(norm_B);
    std::cout << "bottom = " << bottom << std::endl << std::flush;

    bigint sim = top.divide(bottom);
    std::cout << "scaled similarity = " << sim << std::endl << std::flush;

    std::string num = sim.to_string();
    double similarity = (double)std::stoi(num) / (double)1000000;

    return sqrt(similarity);
}

void detect_language(const std::vector<std::string> &training_languages, const std::vector<std::string> &testing_languages) {
    std::string best_match = "Nothing to see here!";

    std::vector< std::vector<unsigned int> > frequency_vectors;

    std::cout << "Started training!" << std::endl << std::flush;
    for (size_t i = 0; i < training_languages.size(); ++i) {
        frequency_vectors.push_back(get_frequency_vector(training_languages[i]));
        std::cout << "Read language " << training_languages[i] << std::endl << std::flush;
    }   std::cout << "Finished training!" << std::endl << std::flush;

    for (size_t i = 0; i < testing_languages.size(); ++i) {
        double similarity = 0.0;

        std::cout << "Reading " << testing_languages[i] << std::endl << std::flush;
        std::vector<unsigned int> frequency_A = get_frequency_vector(testing_languages[i]);
        std::cout << "Finished reading " << testing_languages[i] << std::endl << std::flush;
        
        std::cout << "Getting norm of " << testing_languages[i] << std::endl << std::flush;
        bigint norm_A = get_dot(frequency_A, frequency_A);
        std::cout << norm_A << std::endl << std::flush;

        for (size_t j = 0; j < frequency_vectors.size(); ++j) {
            std::cout << "Claculating similarity with " << training_languages[i] << std::endl << std::flush;
            double temp = get_similarity(norm_A, frequency_A, frequency_vectors[j]);
            std::cout << "similarity = " << temp << std::endl << std::flush;

            if (temp > similarity) {
                similarity = temp;
                best_match = training_languages[j];
            }
        }
        std::cout << testing_languages[i] << std::endl;
        std::cout << "silimarity = " << similarity << std::endl;
        std::cout << best_match << std::endl << std::endl << std::flush;
    }
    return;
}
