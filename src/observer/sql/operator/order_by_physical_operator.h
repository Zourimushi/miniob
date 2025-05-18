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
  OrderByPhysicalOperator(const Table *table, Field field, int order) : table_(table), field_(field), order_(order) {}

  virtual ~OrderByPhysicalOperator() = default;

  string param() const override;

  PhysicalOperatorType type() const override { return PhysicalOperatorType::TABLE_SCAN; }

  RC open(Trx *trx) override;
  RC next() override;
  RC close() override;

  Tuple *current_tuple() override;

  void set_predicates(vector<unique_ptr<Expression>> &&exprs);

private:
  Trx                           *trx_   = nullptr;
  const Table                         *table_ = nullptr;
  // Record                         current_record_;
  Field                          field_;
  int                            order_;
  vector<Record>               records_;
  RowTuple                       tuple_;
  int                            index_;
};