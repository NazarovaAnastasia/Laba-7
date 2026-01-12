#include "use_cases.h"
#include <stdexcept>

namespace application {

PayOrderUseCase::PayOrderUseCase(
    std::shared_ptr<interfaces::OrderRepository> order_repo,
    std::shared_ptr<interfaces::PaymentGateway> payment_gateway)
    : order_repo_(order_repo), payment_gateway_(payment_gateway) {
    
    if (!order_repo_) {
        throw std::invalid_argument("OrderRepository cannot be null");
    }
    if (!payment_gateway_) {
        throw std::invalid_argument("PaymentGateway cannot be null");
    }
}

PayOrderResult PayOrderUseCase::execute(const std::string& order_id) {
    auto order = order_repo_->get_by_id(order_id);
    if (!order) {
        return PayOrderResult(false, "", "Order not found: " + order_id);
    }
    
    try {
        order->pay();
        
        domain::Money amount_to_charge = order->calculate_total();
        interfaces::PaymentResult payment_result = 
            payment_gateway_->charge(order_id, amount_to_charge);
        
        if (payment_result.success) {
            order_repo_->save(order);
            
            return PayOrderResult(true, payment_result.transaction_id, "");
        } else {
            return PayOrderResult(false, "", 
                "Payment failed: " + payment_result.error_message);
        }
        
    } catch (const std::logic_error& e) {
        return PayOrderResult(false, "", "Business rule violation: " + std::string(e.what()));
        
    } catch (const std::exception& e) {
        return PayOrderResult(false, "", "Unexpected error: " + std::string(e.what()));
    }
}

} // namespace application
