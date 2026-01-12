#pragma once
#include "money.h"
#include <string>

namespace domain {

class OrderLine {
public:
    OrderLine(const std::string& product_id, const Money& price, int quantity);
    
    const std::string& get_product_id() const { return product_id_; }
    const Money& get_price() const { return price_; }
    int get_quantity() const { return quantity_; }
    
    Money calculate_total() const;
    
private:
    std::string product_id_;
    Money price_;
    int quantity_;
};

} // namespace domain
