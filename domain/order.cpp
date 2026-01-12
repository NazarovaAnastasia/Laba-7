#include "order.h"

namespace domain {

Order::Order(const std::string& id, const std::string& customer_id)
    : id_(id), customer_id_(customer_id), 
      status_(OrderStatus::PENDING), created_at_(std::chrono::system_clock::now()) {}

void Order::pay() {
    if (lines_.empty()) {
        throw std::logic_error("Cannot pay empty order");
    }
    
    if (status_ == OrderStatus::PAID) {
        throw std::logic_error("Order already paid");
    }
    
    status_ = OrderStatus::PAID;
    paid_at_ = std::chrono::system_clock::now();
}

void Order::add_line(const OrderLine& line) {
    if (status_ == OrderStatus::PAID) {
        throw std::logic_error("Cannot modify paid order");
    }
    lines_.push_back(line);
}

void Order::remove_line(int index) {
    if (status_ == OrderStatus::PAID) {
        throw std::logic_error("Cannot modify paid order");
    }
    if (index < 0 || index >= static_cast<int>(lines_.size())) {
        throw std::out_of_range("Invalid line index");
    }
    lines_.erase(lines_.begin() + index);
}

Money Order::calculate_total() const {
    if (lines_.empty()) {
        return Money::zero();
    }
    
    Money total = Money::zero(lines_[0].get_price().get_currency());
    for (const auto& line : lines_) {
        total = total + line.calculate_total();
    }
    return total;
}

const std::string& Order::get_id() const {
        return id_;
    }

const std::string& Order::get_customer_id() const {
        return customer_id_;
    }

OrderStatus Order::get_status() const {
        return status_;
    }

const std::vector<OrderLine>& Order::get_lines() const {
        return lines_;
    }

} // namespace domain
