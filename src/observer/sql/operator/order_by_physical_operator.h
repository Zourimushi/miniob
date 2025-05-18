#pragma once

#include "common/sys/rc.h"
#include "sql/operator/physical_operator.h"
#include "storage/record/record_manager.h"
#include "common/types.h"

/**
 * @brief 排序物理算子
 * @ingroup PhysicalOperator
 */
class OrderByPhysicalOperator : public PhysicalOperator
{
public:
  OrderByPhysicalOperator(Field field, int order) : field_(field), order_(order) {}

  virtual ~OrderByPhysicalOperator() = default;

  string param() const override;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::ORDER_BY; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

private:
  Trx                           *trx_   = nullptr;
  // Record                         current_record_;
  Field                          field_;
  int                            order_;
  // vector<Record>               records_;

  vector<unique_ptr<Tuple>>                  tuples_;
  int                            index_;
};