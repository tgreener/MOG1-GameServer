/* 
 * File:   DAO.h
 * Author: toddgreener
 *
 * Created on May 27, 2014, 11:03 AM
 */

#ifndef DAO_H
#define	DAO_H

#include <sqlite3.h>
#include <stdbool.h>
#include "DBStatement.h"
#include "sys/Semaphore.h"

#define DB_FILE "game_data.db"

typedef sqlite3* DBHandle;

class DBConnection {
private:

    DBHandle db;
    Semaphore& updateLock;
    
    void printTrace(bool trace);
    
public:
    DBConnection(Semaphore& lock);
    ~DBConnection();
    
    int open(const char* dbfile);
    int close();
    
    DBStatement prepare(const char* query, const char** nextQuery);
    unsigned int lastInsertRowId();
    
    void obtainWriteLock();
    void releaseWriteLock();
    void printError();
};

#endif	/* DAO_H */

