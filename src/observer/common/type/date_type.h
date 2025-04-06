#pragma once

#include "common/sys/rc.h"
#include "common/type/data_type.h"

class DateType : public DataType {
public:
  //构造函数
  DateType() : DataType(AttrType::DATES) {}
  //析构函数
  virtual ~DateType() = default;
   //比较两个 DATE 类型的值大小
  int compare(const Value &left, const Value &right) const override;
  //尝试将 val 从 DATE 类型转换成其他类型（但是不能转换）
  RC cast_to(const Value &val, AttrType type, Value &result) const override;
  //计算从 DATE 类型转换为其他类型的“代价”（代价无限大，表示不能转换）
  int cast_cost(AttrType type) override;
  //将字符串形式的日期（如 "2023-04-02"）解析为整数 yyyymmdd 形式，并设置到 Value 中。
  RC set_value_from_str(Value &val, const string &data) const override;
  //将 Value 中以整数表示的日期转为字符串形式（如 "2023-04-02"）。
  RC to_string(const Value &val, string &result) const override;
  //检查一个日期是否是合法的日期。
  static bool check_date(string date_str);
  static bool check_date(int date);
  static bool check_date(const Value *value);

};