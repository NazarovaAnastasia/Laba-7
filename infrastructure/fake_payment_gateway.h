#pragma once
#include <random>
#include <string>
#include "../interfaces/payment_gateway.h"

namespace infrastructure {

class FakePaymentGateway : public interfaces::PaymentGateway {
public:
    FakePaymentGateway(bool always_succeed = true);
    
    interfaces::PaymentResult charge(
        const std::string& order_id, 
        const domain::Money& amount) override;
    
    void set_always_succeed(bool value);
    void set_success_rate(double rate); // 0.0 - 1.0
    void set_fixed_response(const interfaces::PaymentResult& response);
    
private:
    bool always_succeed_;
    double success_rate_;
    std::optional<interfaces::PaymentResult> fixed_response_;
    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;
    
    std::string generate_transaction_id();
    bool should_succeed();
};

} // namespace infrastructure
