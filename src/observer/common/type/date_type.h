#pragma once

#include "common/sys/rc.h"
#include "common/type/data_type.h"

class DateType : public DataType {
public:
  DateType() : DataType(AttrType::DATES) {}

  virtual ~DateType() = default;

  int compare(const Value &left, const Value &right) const override;

  RC cast_to(const Value &val, AttrType type, Value &result) const override;

  int cast_cost(AttrType type) override;

  RC set_value_from_str(Value &val, const string &data) const override;

  RC to_string(const Value &val, string &result) const override;

  static bool check_date(string date_str);
  static bool check_date(int date);
  static bool check_date(const Value *value);

};