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

typedef sqlite3* DBHandle;

class DBConnection {
private:

    DBHandle db;
    
public:
    DBConnection();
    ~DBConnection();
    
    int open(const char* dbfile);
    DBStatement prepare(const char* query, const char** nextQuery);
    int close();
};

#endif	/* DAO_H */

