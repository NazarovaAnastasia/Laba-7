#pragma once
#include "money.h"
#include "order_status.h"
#include "order_line.h"
#include <vector>
#include <chrono>
#include <string>

namespace domain {

class Order {
public:
    Order(const std::string& id, const std::string& customer_id);
    
    void pay();
    void add_line(const OrderLine& line);
    void remove_line(int index);
    Money calculate_total() const;
    
    const std::string& get_id() const;
    const std::string& get_customer_id() const;
    OrderStatus get_status() const;
    const std::vector<OrderLine>& get_lines() const;
    std::chrono::system_clock::time_point get_created_at() const;
    std::chrono::system_clock::time_point get_paid_at() const;
    
private:
    std::string id_;
    std::string customer_id_;
    OrderStatus status_;
    std::vector<OrderLine> lines_;
    std::chrono::system_clock::time_point created_at_;
    std::chrono::system_clock::time_point paid_at_;
};

} // namespace domain
