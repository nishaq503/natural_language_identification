//
// Started by Tom on 2/9/18.
//

/** Bigint
 */

#ifndef __BIGINT_H__
#define __BIGINT_H__


#include <vector>
#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>

typedef uint_least8_t vec_bin;

class bigint {
private:
    std::vector<vec_bin> number;
    void strip_zeros();

public:
    /** Constructors 
     * 
     * Bigints can be built via:
     *      - Zero
     *      - Vectors
     *      - Unsigned integers
     *      - Strings
     *      - files
     *      - Other Bigints
     * */
    bigint();
    bigint(const std::vector<vec_bin> &that);
    bigint(unsigned long long i);
    bigint(std::string str);
    bigint(std::ifstream &infile);
    bigint(const bigint &that);

    /** Number access.
     *
     * */
    const std::vector<vec_bin> &getNumber() const;
    vec_bin operator[](size_t idx) const;

    /** Comparators
     *
     * */
    bool operator==(const bigint &that) const;
    bool operator!=(const bigint &that) const;
    bool operator<(const bigint &that) const;
    bool operator>(const bigint &that) const;
    bool operator<=(const bigint &that) const;
    bool operator>=(const bigint &that) const;

    /** Arithmetic
     *
     * */

    //  Addition
    bigint add(const bigint &that) const; // returns a new bigiint that is the sum of the two bigints *this and that
    bigint operator+(const bigint &that) const; // returns a new bigiint that is the sum of the two bigints *this and that
    bigint &operator+=(const bigint &that); // adds bigint that to bigint *this and overwrites *this
    bigint &operator++(); // Prefix increment i.e. ++i
    bigint operator++(int); // Postfix increment i.e. i++

    //  Subtraction (note: bigint *this must be greater than bigint that)
    bigint subtract(const bigint &that) const; // returns a new bigint that is the difference between the two bigints *this and that
    bigint operator-(const bigint &that) const; // returns a new bigiint that is the difference between the two bigints *this and that
    bigint &operator-=(const bigint &that); // subtracts bigint that from bigint *this and overwrites *this.
    bigint &operator--(); // Prefix decrement i.e. --i
    bigint operator--(int); // Postfix decrement i.e. i--

    // Multiplication
    bigint multiply(const bigint &that) const; // returns a new bigint that is the product the two bigints *this and that
    bigint fast_multiply(const bigint &that) const; // divides in O(nlogn) time.
    bigint operator*(const bigint &that) const; // returns a new bigint that is the product the two bigints *this and that
    bigint &operator*=(const bigint &that); // multiplys bigint *this and bigint that, and overwrites *this

    //  Division
    bigint divide(const bigint &that) const; // returns a new bigint that is the quotient from the division of bigint *this by bigint that
    bigint fast_divide(const bigint &that) const; // multiplies in O(nlogn) time.
    bigint operator/(const bigint &that) const; // returns a new bigint that is the quotient from the division of bigint *this by bigint that
    bigint &operator/=(const bigint &that); // gets the quotient from the division of bigint *this by bigint that, and overwrites *this

    // Modulo
    bigint mod(const bigint &that) const; // returns a new bigint that is the remainder from the division of bigint *this by bigint that
    bigint fast_mod(const bigint &that) const; // You get the idea.
    bigint operator%(const bigint &that) const; // returns a new bigint that is the remainder from the division of bigint *this by bigint that
    bigint &operator%=(const bigint &that); // gets the remainder from the division of bigint *this by bigint that, and overwrites *this

    // Power
    bigint pow(unsigned long long n);
    bigint fast_pow(unsigned long long n);


    /** Display
     *
     * << operator, to_string, and pprint.
     * */
    std::string to_string(bool commas = false) const; // returns a string representation of the bigint with or without commas.
    friend std::ostream &operator<<(std::ostream &os, const bigint &bigint1); // use << operator to print the bigint ass a string with commas to the ostream.
    std::string scientific(unsigned int decimal_points = 3) const; // returns a string of the form "1.123E25" i.e. scientific notation with the gievn number of decimal points.
    void to_file(std::ofstream &outfile, unsigned int warp = 80);
};

#endif // __BIGINT_H__
