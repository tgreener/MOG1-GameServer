/* 
 * File:   DBStatement.h
 * Author: toddgreener
 *
 * Created on May 27, 2014, 4:44 PM
 */

#ifndef DBSTATEMENT_H
#define	DBSTATEMENT_H

#include <sqlite3.h>
#include <stdbool.h>

typedef sqlite3_stmt* StatementHandle;

class DBStatement {
private:
        StatementHandle statement;
        bool hasNext;
        bool finalized;
public:
    DBStatement(StatementHandle s);
    ~DBStatement();
    
    int step();
    int reset();
    int finalize();
    const char* sql();
    
    bool hasNextRow();
    int columnCount();
    
    double getColumnDouble(int index);
    int getColumnInt(int index);
    const unsigned char* getColumnText(int index);
    const void* getColumnBlob(int index);
    int getColumnNumBytes(int index);
    
    int bindInt(int index, int val);
    int bindText(int index, const char* str);
};

#endif	/* DBSTATEMENT_H */

