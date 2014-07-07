
#include "DBStatement.h"
#include <cstdlib>
#include <cstring>
#include <stdio.h>

DBStatement::DBStatement(StatementHandle s) : statement(s), hasNext(false) {
    if(statement == NULL) {
        finalized = true;
    }
    else {
        finalized = false;
    }
}

DBStatement::~DBStatement() {
    finalize();
}

int DBStatement::step() {
    if(finalized) return 0;
    
    int result = sqlite3_step(statement);
    hasNext = result == SQLITE_ROW;
    
    return (result == SQLITE_ROW) || (result == SQLITE_DONE);
}

int DBStatement::reset() {
    if(finalized) return 0;
    return sqlite3_reset(statement) == SQLITE_OK;
}

int DBStatement::finalize() {
    if(finalized) return true;
    finalized = true;
    
    return sqlite3_finalize(statement) == SQLITE_OK;
}

bool DBStatement::hasNextRow() {
    return hasNext;
}

const char* DBStatement::sql() {
    if(finalized) return NULL;
    return sqlite3_sql(statement);
}

int DBStatement::columnCount() {
    if(finalized) return 0;
    return sqlite3_column_count(statement);
}

double DBStatement::getColumnDouble(int index) {
    if(finalized) return 0.0;
    
    return sqlite3_column_double(statement, index);
}

int DBStatement::getColumnInt(int index) {
    if(finalized) return 0;
    
    return sqlite3_column_int(statement, index);
}

const unsigned char* DBStatement::getColumnText(int index) {
    if(finalized) return NULL;
    
    return sqlite3_column_text(statement, index);;
}

const void* DBStatement::getColumnBlob(int index) {
    if(finalized) return NULL;
    
    return sqlite3_column_blob(statement, index);;
}
    
int DBStatement::getColumnNumBytes(int index) {
    if(finalized) return 0;
    
    return sqlite3_column_bytes(statement, index);;
}

int DBStatement::bindInt(int index, int val) {
    if(finalized) return 0;
    
    return sqlite3_bind_int(statement, index, val) == SQLITE_OK;
}

int DBStatement::bindText(int index, const char* str) {
    if(finalized) return 0;
    
    return sqlite3_bind_text(statement, index, str, strlen(str), SQLITE_TRANSIENT);
}
