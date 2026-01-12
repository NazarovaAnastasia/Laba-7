#include "fake_payment_gateway.h"
#include <chrono>
#include <sstream>
#include <iomanip>

namespace infrastructure {

FakePaymentGateway::FakePaymentGateway(bool always_succeed) 
    : always_succeed_(always_succeed), 
      success_rate_(1.0),
      rng_(std::chrono::system_clock::now().time_since_epoch().count()),
      dist_(0.0, 1.0) {}

interfaces::PaymentResult FakePaymentGateway::charge(
    const std::string& order_id, 
    const domain::Money& amount) {
    
    if (fixed_response_.has_value()) {
        return fixed_response_.value();
    }
     
    bool success = always_succeed_ ? true : should_succeed();
    
    if (success) {
        return interfaces::PaymentResult(
            true, 
            generate_transaction_id(),
            ""
        );
    } else {
        return interfaces::PaymentResult(
            false,
            "",
            "Payment declined: Insufficient funds or network error"
        );
    }
}

void FakePaymentGateway::set_always_succeed(bool value) {
    always_succeed_ = value;
}

void FakePaymentGateway::set_success_rate(double rate) {
    if (rate < 0.0 || rate > 1.0) {
        throw std::invalid_argument("Success rate must be between 0.0 and 1.0");
    }
    success_rate_ = rate;
    always_succeed_ = false;
}

void FakePaymentGateway::set_fixed_response(const interfaces::PaymentResult& response) {
    fixed_response_ = response;
}

std::string FakePaymentGateway::generate_transaction_id() {
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();
    
    std::stringstream ss;
    ss << "TXN_" 
       << timestamp 
       << "_" 
       << std::hex 
       << rng_();
    
    return ss.str();
}

bool FakePaymentGateway::should_succeed() {
    return dist_(rng_) < success_rate_;
}

} // namespace infrastructure
