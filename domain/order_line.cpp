#include "order_line.h"
#include <stdexcept>

namespace domain {

OrderLine::OrderLine(const std::string& product_id, const Money& price, int quantity)
    : product_id_(product_id), price_(price), quantity_(quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Quantity must be positive");
    }
}

Money OrderLine::calculate_total() const {
    return price_ * quantity_;
}

} // namespace domain
