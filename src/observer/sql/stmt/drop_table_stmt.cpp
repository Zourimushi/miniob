#include "sql/stmt/drop_table_stmt.h"
#include "event/sql_debug.h"

RC DropTableStmt::create(Db *db, const DropTableSqlNode &create_table, Stmt *&stmt)
{
    stmt = new DropTableStmt(create_table.relation_name);
    return RC::SUCCESS;
}

