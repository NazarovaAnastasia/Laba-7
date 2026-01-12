#include <gtest/gtest.h>
#include <memory>
#include "application/use_cases.h"
#include "infrastructure/in_memory_order_repository.h"
#include "infrastructure/fake_payment_gateway.h"

class PayOrderUseCaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        order_repo_ = std::make_shared<infrastructure::InMemoryOrderRepository>();
        payment_gateway_ = std::make_shared<infrastructure::FakePaymentGateway>(true);
        use_case_ = std::make_unique<application::PayOrderUseCase>(order_repo_, payment_gateway_);
    }
    
    std::shared_ptr<infrastructure::InMemoryOrderRepository> order_repo_;
    std::shared_ptr<infrastructure::FakePaymentGateway> payment_gateway_;
    std::unique_ptr<application::PayOrderUseCase> use_case_;
};

TEST_F(PayOrderUseCaseTest, SuccessfulPaymentOfCorrectOrder) {
    auto order = std::make_shared<domain::Order>("order-1", "customer-1");
    order->add_line(domain::OrderLine("product-1", domain::Money(1000), 2));
    order_repo_->save(order);
    
    auto result = use_case_->execute("order-1");
    
    EXPECT_TRUE(result.success);
    EXPECT_FALSE(result.transaction_id.empty());
    EXPECT_TRUE(result.error_message.empty());
    
    auto saved_order = order_repo_->get_by_id("order-1");
    EXPECT_EQ(saved_order->get_status(), domain::OrderStatus::PAID);
}

TEST_F(PayOrderUseCaseTest, ErrorWhenPayingEmptyOrder) {
    auto order = std::make_shared<domain::Order>("order-2", "customer-2");
    order_repo_->save(order);
    
    auto result = use_case_->execute("order-2");
    
    EXPECT_FALSE(result.success);
    EXPECT_TRUE(result.error_message.find("empty") != std::string::npos || 
                result.error_message.find("Cannot pay") != std::string::npos);
}

TEST_F(PayOrderUseCaseTest, ErrorWhenPayingAlreadyPaidOrder) {
    auto order = std::make_shared<domain::Order>("order-3", "customer-3");
    order->add_line(domain::OrderLine("product-1", domain::Money(500), 1));
    order_repo_->save(order);
    
    use_case_->execute("order-3");
    auto result = use_case_->execute("order-3");
    
    EXPECT_FALSE(result.success);
    EXPECT_TRUE(result.error_message.find("already") != std::string::npos);
}

TEST_F(PayOrderUseCaseTest, CannotModifyOrderAfterPayment) {
    auto order = std::make_shared<domain::Order>("order-4", "customer-4");
    order->add_line(domain::OrderLine("product-1", domain::Money(1000), 1));
    order_repo_->save(order);
    
    use_case_->execute("order-4");
    
    auto saved_order = order_repo_->get_by_id("order-4");
    EXPECT_THROW(
        saved_order->add_line(domain::OrderLine("product-2", domain::Money(500), 1)),
        std::logic_error
    );
    EXPECT_EQ(saved_order->get_lines().size(), 1);
}

TEST_F(PayOrderUseCaseTest, CorrectTotalAmountCalculation) {
    auto order = std::make_shared<domain::Order>("order-5", "customer-5");
    order->add_line(domain::OrderLine("product-1", domain::Money(1000), 2));
    order->add_line(domain::OrderLine("product-2", domain::Money(2500), 1));
    order_repo_->save(order);
    
    auto result = use_case_->execute("order-5");
    
    EXPECT_TRUE(result.success);
    auto saved_order = order_repo_->get_by_id("order-5");
    auto total = saved_order->calculate_total();
    
    EXPECT_EQ(total.get_amount(), 4500);
    EXPECT_EQ(total.to_string(), "USD 45.00");
}
