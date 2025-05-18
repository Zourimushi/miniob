#include "sql/operator/order_by_physical_operator.h"
#include "storage/trx/trx.h"

using namespace std;

RC OrderByPhysicalOperator::open(Trx *trx)
{
  if (children_.empty()) {
    return RC::SUCCESS;
  }
  unique_ptr<PhysicalOperator> &child = children_[0];

  RC rc = child->open(trx);
  if (rc != RC::SUCCESS) {
    LOG_WARN("failed to open child operator: %s", strrc(rc));
    return rc;
  }


  trx_ = trx;

  while (OB_SUCC(rc = child->next())) {
    Tuple *tuple = child->current_tuple();
    if (nullptr == tuple) {
      LOG_WARN("failed to get current record: %s", strrc(rc));
      return rc;
    }
    // RowTuple *row_tuple = static_cast<RowTuple *>(tuple);
    Tuple *new_tuple = tuple->clone();
    tuples_.emplace_back(new_tuple);
  }

  child->close();
  // const FieldMeta *field_meta   = field_.meta();
  // const int        value_offset = field_meta->offset();
  // const int        value_len    = field_meta->len();

  // 冒泡排序，因为是稳定的排序，所以不会影响之前的相对位置
  for (int i = 0; i < (int)(tuples_.size()) - 1; ++i) {
    for (int j = 0; j < (int)(tuples_.size()) - 1 - i; ++j) {
      // 获取当前元素的值
      Value       value1;
      // const char *data1 = records_[j].data() + value_offset;
      // value1.set_type(field_meta->type());
      // value1.set_data(data1, value_len);
      // value1.set_null(data1[value_len]);
      rc = tuples_[j]->find_cell(TupleCellSpec(field_.table()->name(), field_.field_name()), value1);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value from tuple: %s", strrc(rc));
        return rc;
      }

      // 获取下一个元素的值
      Value       value2;
      rc = tuples_[j+1]->find_cell(TupleCellSpec(field_.table()->name(), field_.field_name()), value2);
      if (rc != RC::SUCCESS) {
        LOG_WARN("failed to get value from tuple: %s", strrc(rc));
        return rc;
      }
      
      // value2.set_type(field_meta->type());
      // value2.set_data(data2, value_len);
      // value2.set_null(data2[value_len]);

      // 比较两个值
      //TODO: 考虑null的情况
      int result = order_*value1.compare(value2);
      if (result>0) {
        std::swap(tuples_[j], tuples_[j + 1]);
      }
    }
  }
  index_ = -1;
  return RC::SUCCESS;
}

RC OrderByPhysicalOperator::next() { 
  if (++index_ < (int)tuples_.size()) {
    return RC::SUCCESS;
  }
  return RC::RECORD_EOF;
}

Tuple* OrderByPhysicalOperator::current_tuple() {
  if (index_ < (int)tuples_.size()) {
    return tuples_[index_].get();
  }
  // 不应该到这里
  LOG_WARN("failed to get current record: %s", strrc(RC::RECORD_EOF));
  return nullptr;
}

RC OrderByPhysicalOperator::close() { return RC::SUCCESS; }

string OrderByPhysicalOperator::param() const { return field_.field_name(); }