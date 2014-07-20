
#include "DBConnection.h"
#include <cstdlib>
#include <cstring>
#include <stdio.h>

DBConnection::DBConnection() {}
DBConnection::~DBConnection() {}

int DBConnection::open(const char* dbfile) {
    int result = sqlite3_open_v2(dbfile, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) == SQLITE_OK;
    
    if(result) {
        DBStatement s = prepare("PRAGMA foreign_keys = ON;", NULL);
        if(!s.step()) {
            printf("Error turning on foreign_keys!\n");
        }
        s.finalize();
    }
    else {
        printf("Error turning on foreign_keys!\n");
    }
    
    return result;
}

int DBConnection::close() {
    return sqlite3_close(db) == SQLITE_OK;
}

DBStatement DBConnection::prepare(const char* query, const char** nextQuery) {
    StatementHandle statement;
    int result = sqlite3_prepare_v2(db, query, strlen(query), &statement, nextQuery);
    
    if(result != SQLITE_OK) {
        printf("Prepare error: %d\n", result);
        printf("%s\n", query);
        statement = NULL;
    }
    
    return statement;
}

unsigned int DBConnection::lastInsertRowId() {
    return sqlite3_last_insert_rowid(db);
}


