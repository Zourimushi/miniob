#pragma once

#include "sql/operator/logical_operator.h"

class OrderByLogicalOperator : public LogicalOperator
{
public:
  OrderByLogicalOperator(vector<unique_ptr<Expression>> &&order_by_exprs)
  : order_by_expressions_(std::move(order_by_exprs)) {}

  virtual ~OrderByLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::ORDER_BY; }

  vector<unique_ptr<Expression>> &order_by_expressions() { return order_by_expressions_; }

private:
  vector<unique_ptr<Expression>> order_by_expressions_;
};