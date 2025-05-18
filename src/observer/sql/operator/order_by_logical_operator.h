#pragma once

#include "sql/operator/logical_operator.h"

class OrderByLogicalOperator : public LogicalOperator
{
public:
  OrderByLogicalOperator(const Table *table, vector<unique_ptr<Expression>> &&order_by_exprs)
  : table_(table), order_by_expressions_(std::move(order_by_exprs)) {}

  virtual ~OrderByLogicalOperator() = default;

  LogicalOperatorType type() const override { return LogicalOperatorType::ORDER_BY; }

  vector<unique_ptr<Expression>> &order_by_expressions() { return order_by_expressions_; }

  const Table *table() const { return table_; }

private:
  const Table *table_;
  vector<unique_ptr<Expression>> order_by_expressions_;
};