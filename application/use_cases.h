#pragma once
#include <memory>
#include <string>
#include "../interfaces/order_repository.h"
#include "../interfaces/payment_gateway.h"

namespace application {

struct PayOrderResult {
    bool success;
    std::string transaction_id;
    std::string error_message;
    
    PayOrderResult(bool s = false, const std::string& tid = "", const std::string& err = "")
        : success(s), transaction_id(tid), error_message(err) {}
};

class PayOrderUseCase {
public:
    PayOrderUseCase(
        std::shared_ptr<interfaces::OrderRepository> order_repo,
        std::shared_ptr<interfaces::PaymentGateway> payment_gateway);
    
    PayOrderResult execute(const std::string& order_id);
    
private:
    std::shared_ptr<interfaces::OrderRepository> order_repo_;
    std::shared_ptr<interfaces::PaymentGateway> payment_gateway_;
};

} // namespace application
