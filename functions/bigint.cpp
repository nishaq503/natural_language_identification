//
// Started by Tom on 2/9/18.
//

#include "bigint.h"

void bigint::strip_zeros() {
    // remove and leading 0s
    while (!number.empty() && number.back() == 0) number.pop_back();
    // ensure number is not empty. Must be >= 0.
    if (number.empty()) number.push_back(0);
}

/* Constructors
 * 
 * */

bigint::bigint() {
    number.clear();
    number.push_back(0);
}

bigint::bigint(const std::vector<vec_bin> &that) {
    number.clear();
    number = that;
    this->strip_zeros(); // same as (*this).strip_zeros();
}

bigint::bigint(unsigned long long i) {
    number.clear();
    // No matter what, record a digit (even if it's 0).
    // Continue to do so while the number leftover isn't 0.
    do {
        number.push_back((vec_bin)(i % 10));
    } while (i /= 10);
}

bigint::bigint(std::string str) {
    number.clear();

    // Get each digit, starting from the end, and append to number.
    for (int i = str.length(); i > 0; --i) number.push_back((vec_bin)(str[i - 1]) - '0');
    
    this->strip_zeros();
}

bigint::bigint(std::ifstream &infile) {
    if (!infile.fail()) {
        std::string line, result;
        while (getline(infile, line)) result += line;
        *this = result;
    }
    else std::cerr << "Could not open the file!" << std::endl;
}

bigint::bigint(const bigint &that) {
    number.clear();
    number = that.getNumber();
}


/* Number Access
 * 
 * */
const std::vector<vec_bin> &bigint::getNumber() const {
    return number;
}

vec_bin bigint::operator[](size_t index) const {
    return number[index];
}


/** Comparators
 *
 * */
bool bigint::operator==(const bigint &that) const {
    return number == that.number;
}

bool bigint::operator!=(const bigint &that) const {
    return !(that == *this);
}

bool bigint::operator<(const bigint &that) const {
    // First compare the number of digits.
    if (number.size() < that.number.size()) return true;
    else if (that.number.size() < number.size()) return false;
        // Else, compare each digit individually.
    else {
        for (size_t i = number.size(); i > 0; --i) {
            if (number[i - 1] < that[i - 1]) return true;
            else if (that[i - 1] < number[i - 1]) return false;
        }
    }
    return false;
}

bool bigint::operator<=(const bigint &that) const {
    return !(that < *this);
}

bool bigint::operator>(const bigint &that) const {
    return that < *this;
}

bool bigint::operator>=(const bigint &that) const {
    return !(*this < that);
}


/** Addition
 *
 * */

bigint bigint::add(const bigint &that) const {
    // Find the size of each number.
    size_t n_this = number.size();
    size_t n_that = that.number.size();
    size_t n = n_this;

    std::vector<vec_bin> result = number;
    if (n_this < n_that) {
        n = n_that;
        for (size_t i = n_this; i < n_that; ++i) result.push_back(0);
    }

    // The addition is done here.
    int carry = 0, temp = 0;
    for (size_t i = 0; i < n; ++i) {
        // temp is the sum of both numbers,
        // if any number is already all used up,
        // it replaces that number with 0.
        temp = (i < n_this ? number[i] : 0) + (i < n_that ? that[i] : 0) + carry;
        // Now, save the digit, and fix the carry.
        result[i] = temp % 10;
        carry = temp / 10;
    }
    // 90 + 100 leaves a carry, add that to the back of number here.
    if (carry > 0) result.push_back(carry);

    bigint ret = result;
    return ret;
}

bigint bigint::operator+(const bigint &that) const {
    bigint ret = (*this).add(that);
    return ret;
}

bigint &bigint::operator+=(const bigint &that) {
    *this = (*this).add(that);
    return *this;
}

bigint &bigint::operator++() {
    return *this += bigint(1);
}

bigint bigint::operator++(int) {
    // Postfix must return the current value.
    bigint temp = *this;
    ++*this;
    return temp;
}


/** Subtraction
 *
 * */

bigint bigint::subtract(const bigint &that) const {
    // Only subtract if the remaining number will be >= 0.
    if (*this < that) throw "Cannot perform Subtraction.";

    size_t num_this = number.size();
    size_t num_that = that.getNumber().size();
    std::vector<vec_bin> result = number;

    // Do the actual subtraction.
    int take = 0, temp = 0;
    for (size_t i = 0; i < num_this; ++i) {
        // Since number is larger we don't have to check if it's out of digits.
        // We are not guaranteed num_that will be long enough,
        // if it's not, replace it with 0.
        temp = number[i] - (i < num_that ? that[i] : 0) - take;
        // If temp is negative, add 10 and set take to 1.
        if (temp < 0) {
            temp += 10;
            take = 1;
        } else {
            take = 0;
        }
        result[i] = temp;
    }
    bigint ret = result;
    ret.strip_zeros();
    return ret;
}

bigint bigint::operator-(const bigint &that) const {
    bigint ret = (*this).subtract(that);
    return ret;
}

bigint &bigint::operator-=(const bigint &that) {
    *this = (*this).subtract(that);
    return *this;
}

bigint &bigint::operator--() {
    return *this -= bigint(1);
}

bigint bigint::operator--(int) {
    // Postfix must return the current value.
    bigint temp = *this;
    --*this;
    return temp;
}

/**
 * Double and Halve bigints
 **/
void dbl(bigint &a) {
    std::vector<vec_bin> number = a.getNumber();
    vec_bin carry = 0, temp;
    for (size_t i = 0; i < number.size(); i++) {
        temp = number[i] + number[i] + carry;
        number[i] = temp % 10;
        carry = temp / 10;
    }
    if (carry) number.push_back(carry);
    a = bigint(number);
}

void halve(bigint &a) {
    vec_bin carry = 0;
    std::vector<vec_bin> number = a.getNumber();
    for (size_t i = number.size() - 1; i > 0; --i) {
        vec_bin temp = number[i] % 2;
        number[i] = (number[i] / 2) + (carry * 5);
        carry = temp;
    }
    number[0] = (number[0] / 2) + (carry * 5);
    a = bigint(number);
}


/** Multiplication
 *
 * */

bigint bigint::multiply(const bigint &that) const {
    // Iterative addition:
    bigint zero = 0;
    // Catch 0 multiplication.
    if (*this == zero || that == zero) return zero;
    // Otherwise.
    bigint result = number;
    bigint temp = that.getNumber();
    while (--temp != zero) {
        result += *this;
    }
    return result;
}

bigint r_multiply(bigint &a, bigint &b) {
    if (b == 1) return a;
    bigint temp = b[0] % 2 ? a : 0;
    dbl(a); halve(b);
    return temp + r_multiply(a, b);
}

bigint bigint::fast_multiply(const bigint &that) const {
    if (*this == 0 || that == 0) return 0;
    bigint a = *this, b = that;
    return r_multiply(a, b);
}

bigint bigint::operator*(const bigint &that) const {
    bigint ret = this->fast_multiply(that);
    return ret;
}

bigint &bigint::operator*=(const bigint &that) {
    *this = this->fast_multiply(that);
    return *this;
}


/** Division
 *
 * */

bigint bigint::divide(const bigint &that) const {
    bigint result = 0, temp = *this;
    if (that == result) throw "Cannot divide by 0!";
    while (temp >= that) {
        temp -= that;
        ++result;
    }
    return result;
}

unsigned int r_divide_helper(bigint &a, bigint &b) {
    dbl(b);
    if (a < b) {
        halve(b);
        return 1;
    }
    return 2 * r_divide_helper(a, b);
}

bigint r_divide(bigint &a, bigint & b) {
    unsigned int temp = 1;
    bigint result;
    do {
        temp = r_divide_helper(a, b);
        result += temp;
        a -= b;
        for (unsigned int i = 1; i < temp; i *= 2) halve(b);
    } while (temp > 1 && a != 0 && a >= b);
    return result;
}

bigint bigint::fast_divide(const bigint &that) const {
    if (*this == 0) return 0;
    else if (that == 0) throw "Cannot Divide by 0!";
    else if (*this < that) return 0;

    bigint a = *this, b = that;

    return r_divide(a, b);
}

bigint bigint::operator/(const bigint &that) const {
    bigint ret = this->fast_divide(that);
    return ret;
}

bigint &bigint::operator/=(const bigint &that) {
    *this = this->fast_divide(that);
    return *this;
}


/** Modulo
 *
 * */

bigint bigint::mod(const bigint &that) const {
    if (that == 0) throw "Cannot mod by 0!";
    bigint result = *this;
    while (result >= that) result -= that;
    return result;
}

bigint r_mod(bigint &a, bigint & b) {
    unsigned int temp = 1;
    bigint result;
    do {
        temp = r_divide_helper(a, b);
        result += temp;
        a -= b;
        for (unsigned int i = 1; i < temp; i *= 2) halve(b);
    } while (temp > 1 && a != 0);
    return a;
}

bigint bigint::fast_mod(const bigint &that) const {
    if (*this == 0) return 0;
    else if (that == 0) throw "Cannot Divide by 0!";
    else if (*this < that) return 0;

    bigint a = *this, b = that;

    return r_mod(a, b);
}

bigint bigint::operator%(const bigint &that) const {
    bigint ret = this->mod(that);
    return ret;
}

bigint &bigint::operator%=(const bigint &that) {
    *this = this->mod(that);
    return *this;
}


/** Power
 *
 * */

bigint bigint::pow(unsigned long long n) {
    if (n == 0) return bigint(1);
    bigint ret = *this;
    while (n-- > 1) ret *= *this;
    return ret;
}

bigint r_pow(bigint &a, unsigned long long n) {
    if (n == 1) return a;
    bigint temp = a;
    a *= a;
    return n % 2
           ? temp * r_pow(a, n/2)
           : r_pow(a, n/2);
}

bigint bigint::fast_pow(unsigned long long n) {
    if (n) {
        bigint a = *this;
        return r_pow(a, n);
    }
    return 1;
}


/** Display methods
 *
 * */

std::ostream &operator<<(std::ostream &os, const bigint &bigint1) {
    return os << bigint1.to_string(true);
}

std::string bigint::to_string(bool commas) const {
    // Build a string representation of the bigint.
    std::string result;
    result += (static_cast<char>(number[number.size() - 1] + '0' ));

    for (int i = number.size() - 2; i >= 0; --i) {
        if (i % 3 == 2 && commas) result += ',';
        result += (static_cast<char>(number[i] + '0' ));
    }

    return result;
}

std::string bigint::scientific(unsigned int decimal_points) const {
    std::string result;
    for (unsigned int i = 0; i <= decimal_points; ++i) {
        if (i == 1) result += '.';
        result += std::to_string((i < number.size()) ? number[number.size()-i-1] : 0);
    }
    result += "E";
    result += std::to_string(number.size() - 1);
    return result;
}

void bigint::to_file(std::ofstream &outfile, unsigned int wrap) {
    std::string result = this->to_string();
    for (int i = 0; i < (int)result.length(); ++i) {
        outfile << result[i];
        if (i % wrap == wrap - 1) outfile << std::endl;
    }
}
