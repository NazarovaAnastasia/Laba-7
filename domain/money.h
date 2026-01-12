#pragma once
#include <string>
#include <stdexcept>
#include <iostream>

namespace domain {

class Money {
public:
    Money(int amount, const std::string& currency = "USD");
    
    int get_amount() const { return amount_; }
    const std::string& get_currency() const { return currency_; }
    
    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money operator*(int multiplier) const;
    bool operator==(const Money& other) const;
    bool operator!=(const Money& other) const;
    bool operator<(const Money& other) const;
    bool operator>(const Money& other) const;
    
    std::string to_string() const;
    
    static Money zero(const std::string& currency = "USD");
    static Money from_double(double amount, const std::string& currency = "USD");
    
private:
    int amount_;
    std::string currency_;
    
    void validate_amount(int amount) const;
    void validate_currency(const Money& other) const;
};

std::ostream& operator<<(std::ostream& os, const Money& money);

} // namespace domain
