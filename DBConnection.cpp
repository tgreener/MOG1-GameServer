
#include "DBConnection.h"
#include "sys/Semaphore.h"
#include <cstdlib>
#include <cstring>
#include <stdio.h>

DBConnection::DBConnection(Semaphore& lock) : updateLock(lock) {}
DBConnection::~DBConnection() {}

int DBConnection::open(const char* dbfile) {
    int result = sqlite3_open_v2(dbfile, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL) == SQLITE_OK;
    
    if(result) {
        DBStatement s = prepare("PRAGMA foreign_keys = ON;", NULL);
        if(!s.step()) {
            printf("Error turning on foreign_keys!\n");
        }
        s.finalize();
        
//        printTrace(true);
    }
    else {
        printf("Error creating db connection!\n");
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

void DBConnection::obtainWriteLock() {
    updateLock.wait();
}

void DBConnection::releaseWriteLock() {
    updateLock.signal();
}

void DBConnection::printError() {
    printf("%s\n", sqlite3_errmsg(db));
}

static unsigned int traceCount = 0;
void traceCallback(void* something, const char* sql) {
    traceCount++;
    printf("%d=>%s\n", traceCount, sql);
}

void DBConnection::printTrace(bool trace) {
    if(trace) {
        sqlite3_trace(db, traceCallback, NULL);
    }
    else {
        sqlite3_trace(db, NULL, NULL);
    }
}

