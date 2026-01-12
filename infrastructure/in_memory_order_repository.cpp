#include "in_memory_order_repository.h"

namespace infrastructure {

std::shared_ptr<domain::Order> InMemoryOrderRepository::get_by_id(const std::string& order_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = orders_.find(order_id);
    if (it != orders_.end()) {
        return it->second;
    }
    return nullptr;
}

void InMemoryOrderRepository::save(const std::shared_ptr<domain::Order>& order) {
    if (!order) {
        throw std::invalid_argument("Cannot save null order");
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    orders_[order->get_id()] = order;
}

void InMemoryOrderRepository::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    orders_.clear();
}

size_t InMemoryOrderRepository::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return orders_.size();
}

} // namespace infrastructure
