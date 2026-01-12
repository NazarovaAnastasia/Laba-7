#include "money.h"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace domain {

Money::Money(int amount, const std::string& currency)
    : amount_(amount), currency_(currency) {
    validate_amount(amount);
}

void Money::validate_amount(int amount) const {
    if (amount < 0) {
        throw std::invalid_argument("Money amount cannot be negative");
    }
}

void Money::validate_currency(const Money& other) const {
    if (currency_ != other.currency_) {
        throw std::invalid_argument("Currency mismatch: " + currency_ + 
                                   " vs " + other.currency_);
    }
}

Money Money::operator+(const Money& other) const {
    validate_currency(other);
    return Money(amount_ + other.amount_, currency_);
}

Money Money::operator-(const Money& other) const {
    validate_currency(other);
    if (amount_ < other.amount_) {
        throw std::invalid_argument("Insufficient funds for subtraction");
    }
    return Money(amount_ - other.amount_, currency_);
}

Money Money::operator*(int multiplier) const {
    if (multiplier < 0) {
        throw std::invalid_argument("Multiplier cannot be negative");
    }
    return Money(amount_ * multiplier, currency_);
}

bool Money::operator==(const Money& other) const {
    return amount_ == other.amount_ && currency_ == other.currency_;
}

bool Money::operator!=(const Money& other) const {
    return !(*this == other);
}

bool Money::operator<(const Money& other) const {
    validate_currency(other);
    return amount_ < other.amount_;
}

bool Money::operator>(const Money& other) const {
    validate_currency(other);
    return amount_ > other.amount_;
}

std::string Money::to_string() const {
    std::stringstream ss;
    ss << currency_ << " " 
       << std::fixed << std::setprecision(2) 
       << (amount_ / 100.0);
    return ss.str();
}

Money Money::zero(const std::string& currency) {
    return Money(0, currency);
}

Money Money::from_double(double amount, const std::string& currency) {
    int cents = static_cast<int>(std::round(amount * 100));
    return Money(cents, currency);
}

std::ostream& operator<<(std::ostream& os, const Money& money) {
    os << money.to_string();
    return os;
}

} // namespace domain
