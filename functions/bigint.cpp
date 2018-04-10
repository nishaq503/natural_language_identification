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

/**
 * Constructors
 **/

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
    for (int i = str.length(); i > 0; --i)
        number.push_back((vec_bin)(str[i - 1]) - '0');
    
    this->strip_zeros();
}

bigint::bigint(std::ifstream &infile) {
    if (!infile.fail()) {
        std::string line, result;
        while (getline(infile, line))
            result += line;

        *this = result;
    }
    else throw "Could not open the file!";
}

bigint::bigint(const bigint &that) {
    number.clear();
    *this = that;
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
    size_t n = n_this > n_that ? n_this : n_that;

    std::vector<vec_bin> result;

    // The addition is done here.
    int carry = 0, temp = 0;
    for (size_t i = 0; i < n; ++i) {
        // temp is the sum of both numbers,
        // if any number is already all used up,
        // it replaces that number with 0.
        temp = (i < n_this ? number[i] : 0) + (i < n_that ? that[i] : 0) + carry;
        // Now, save the digit, and fix the carry.
        result.push_back(temp % 10);
        carry = temp / 10;
    }
    // 90 + 100 leaves a carry, add that to the back of number here.
    if (carry > 0) result.push_back(carry);

    return bigint(result);
}

bigint bigint::operator+(const bigint &that) const {
    return this->add(that);
}

bigint &bigint::operator+=(const bigint &that) {
    *this = this->add(that);
    return *this;
}

bigint &bigint::operator++() {
    return *this += 1;
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
    std::vector<vec_bin> result;

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
        result.push_back(temp);
    }
    return bigint(result);
}

bigint bigint::operator-(const bigint &that) const {
    return this->subtract(that);
}

bigint &bigint::operator-=(const bigint &that) {
    *this = this->subtract(that);
    return *this;
}

bigint &bigint::operator--() {
    return *this -= 1;
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
    return;
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
    return;
}

/**
 * base-10 shifting for fast multiply
 **/

void bigint::shift_up(vec_bin n) {
    std::vector<vec_bin> result(n,0);
    for (size_t i = 0; i < number.size(); ++i)
        result.push_back(number[i]);

    number = result;
}

vec_bin bigint::shift_down() {
    vec_bin result = number.front();
    number.erase(number.begin());
    return result;
}

bigint bigint::mult_digit(vec_bin d) const {
    vec_bin carry = 0, temp;
    std::vector<vec_bin> result;
    for (size_t i = 0; i < number.size(); ++i) {
        temp = (number[i] * d) + carry;
        result.push_back(temp % 10);
        carry = temp / 10;
    }
    if (carry) result.push_back(carry);
    return result;
}


/** Multiplication
 *
 * */

bigint bigint::multiply(const bigint &that) const {
    // Catch 0 multiplication.
    if (*this == 0 || that == 0) return 0;
    // Otherwise.
    bigint result = number;
    bigint temp = that.getNumber();
    while (--temp != 0)
        result += *this;

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
    return a > b
           ? r_multiply(a, b)
           : r_multiply(b, a);
}

bigint bigint::r_multiply_10(bigint &that) {
    if (that.number.size() == 1) return this->mult_digit(that[0]);
    vec_bin d = that.shift_down();
    bigint temp = d ? this->mult_digit(d) : 0;
    this->shift_up();
    return temp + this->r_multiply_10(that);
}

bigint bigint::fast_mult_10(const bigint &that) const {
    if (*this == 0 || that == 0) return 0;
    bigint a = *this, b = that;
    return a > b
           ? a.r_multiply_10(b)
           : b.r_multiply_10(a);
}

bigint bigint::operator*(const bigint &that) const {
    bigint ret = this->fast_mult_10(that);
    return ret;
}

bigint &bigint::operator*=(const bigint &that) {
    *this = this->fast_mult_10(that);
    return *this;
}

/** Division
 *
 * */

bigint bigint::divide(const bigint &that) const {
    if (that == 0) throw "Cannot divide by 0!";
    bigint result = 0, temp = *this;
    while (temp >= that) {
        temp -= that;
        ++result;
    }
    return result;
}

bigint r_divide_helper(bigint &a, bigint &b) {
    dbl(b);
    if (a < b) {
        halve(b);
        return 1;
    }
    return r_divide_helper(a, b) * 2;
}

bigint r_divide(bigint &a, const bigint &that) {
    bigint temp = 1, b = that, result;
    do {
        temp = r_divide_helper(a, b);
        result += temp;
        a -= b;
        b = that;
    } while (a != 0 && a >= b && temp > 1);
    return result;
}

bigint bigint::fast_divide(const bigint &that) const {
    if (that == 0) throw "Cannot Divide by 0!";
    else if (*this < that) return 0;

    bigint a = *this;

    return r_divide(a, that);
}

bigint r_div_10(bigint &a, const bigint &that) {
    bigint result, b;
    vec_bin n = 0;
    do {
        vec_bin k = 0;
        b = that;
        n = a.getNumber().size() - b.getNumber().size();
        b.shift_up(n);
        if (a < b) {
            b.shift_down();
            --n;
        }
        while (a >= b) {
            a -= b;
            ++k;
        }
        bigint temp = k;
        temp.shift_up(n);
        result += temp;
    } while (n && a >= that);
    return result;
}

bigint bigint::fast_div_10(const bigint &that) const {
    if (that == 0) throw "Cannot Divide by 0!";
    else if (*this < that) return 0;

    bigint a = *this;

    return r_div_10(a, that);
}

bigint bigint::operator/(const bigint &that) const {
    bigint ret = this->fast_div_10(that);
    return ret;
}

bigint &bigint::operator/=(const bigint &that) {
    *this = this->fast_div_10(that);
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

bigint r_mod(bigint &a, const bigint &that) {
    bigint temp = 1, b = that;
    do {
        temp = r_divide_helper(a, b);
        a -= b;
        b = that;
    } while (a != 0 && a >= b && temp > 1);
    return a;
}

bigint bigint::fast_mod(const bigint &that) const {
    if (that == 0) throw "Cannot Divide by 0!";
    else if (*this < that) return *this;

    bigint a = *this;

    return r_mod(a, that);
}

bigint r_mod_10(bigint &a, const bigint &that) {
    vec_bin n = 0;
    do {
        bigint b = that;
        vec_bin k = 0;
        n = a.getNumber().size() - b.getNumber().size();
        b.shift_up(n);
        if (a < b) {
            b.shift_down();
            --n;
        }
        while (a >= b) {
            a -= b;
            ++k;
        }
    } while (n && a >= that);
    return a;
}

bigint bigint::fast_mod_10(const bigint &that) const {
    if (that == 0) throw "Cannot Divide by 0!";
    else if (*this < that) return 0;

    bigint a = *this;

    return r_mod_10(a, that);
}

bigint bigint::operator%(const bigint &that) const {
    bigint ret = this->fast_mod_10(that);
    return ret;
}

bigint &bigint::operator%=(const bigint &that) {
    *this = this->fast_mod_10(that);
    return *this;
}


/** Power
 *
 * */

bigint bigint::pow(unsigned long long n) {
    if (n == 0) return 1;

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
    result += (static_cast<char>(number[number.size() - 1] + '0'));

    for (size_t i = number.size() - 1; i > 0; --i) {
        if (i % 3 == 0 && commas) result += ',';
        result += (static_cast<char>(number[i-1] + '0'));
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
bigint result;
void bigint::to_file(std::ofstream &outfile, unsigned int wrap) {
    std::string result = this->to_string();
    for (int i = 0; i < (int)result.length(); ++i) {
        outfile << result[i];
        if (i % wrap == wrap - 1) outfile << std::endl;
    }
}
