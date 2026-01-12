#pragma once
#include <memory>
#include <string>
#include "../domain/order.h"

namespace interfaces {

class OrderRepository {
public:
    virtual ~OrderRepository() = default;
    
    virtual std::shared_ptr<domain::Order> get_by_id(const std::string& order_id) = 0;
    virtual void save(const std::shared_ptr<domain::Order>& order) = 0;
};

} // namespace interfaces
