#pragma once
#include <memory>
#include <unordered_map>
#include <mutex>
#include "../interfaces/order_repository.h"

namespace infrastructure {

class InMemoryOrderRepository : public interfaces::OrderRepository {
public:
    std::shared_ptr<domain::Order> get_by_id(const std::string& order_id) override;
    void save(const std::shared_ptr<domain::Order>& order) override;

    void clear();
    size_t size() const;
    
private:
    std::unordered_map<std::string, std::shared_ptr<domain::Order>> orders_;
    mutable std::mutex mutex_;
};

} // namespace infrastructure
