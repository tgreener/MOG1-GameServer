/* 
 * File:   SystemCommandDelegate.h
 * Author: toddgreener
 *
 * Created on May 26, 2014, 1:21 PM
 */

#ifndef SYSTEMCOMMANDDELEGATE_H
#define	SYSTEMCOMMANDDELEGATE_H

#include "ByteCodeDelegate.h"

#define SCHEME_FILE "db_scheme.txt"
#define SCHEME_PURGE_FILE "db_purge.txt"

class SystemCommandDelegate : public ByteCodeDelegate {
public:
    void interpretCommand(char* bytes, int length);
};

#endif	/* SYSTEMCOMMANDDELEGATE_H */

