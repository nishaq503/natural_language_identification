/**
 * Started by Najib 3/21/18
 **/

#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "bigint.h"

// reads the next letter, converts it to the right integer, returns true if able to read a character.
bool read_next_char(std::ifstream &infile, char &ch, unsigned int &i);

// given a filename, calculates the trigram frequency vector.
std::vector<unsigned int> get_frequency_vector(const std::string file_name);

// given 2 frequency vectors, calculates their dot product as a bigint.
bigint get_dot(const std::vector<unsigned int> &frequency_A, const std::vector<unsigned int> &frequency_B);

// calculates the similarity between 2 frequency vectors using scaled division of bigints.
double get_similarity(const bigint &norm_A, const std::vector<unsigned int> &frequency_A, const std::vector<unsigned int> &frequency_B);

// uses the above functions to find the best match in training languages for each testing language.
void detect_language(const std::vector<std::string> &training_languages, const std::vector<std::string> &testing_languages);

#endif
