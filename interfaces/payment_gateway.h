#pragma once
#include <string>
#include "../domain/money.h"

namespace interfaces {

struct PaymentResult {
    bool success;
    std::string transaction_id;
    std::string error_message;
    
    PaymentResult(bool s, const std::string& tid = "", const std::string& err = "")
        : success(s), transaction_id(tid), error_message(err) {}
};

class PaymentGateway {
public:
    virtual ~PaymentGateway() = default;
    
    virtual PaymentResult charge(const std::string& order_id, const domain::Money& amount) = 0;
};

} // namespace interfaces
