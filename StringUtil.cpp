
#include "StringUtil.h"
#include <string.h>
#include <ctype.h>
#include <cstdlib>
#include <stdio.h>

void strToLower(char* str) {
    int length = strlen(str);
    
    for(int i = 0; i < length; i++) {
        str[i] = tolower(str[i]);
    }
}

int trim(char* str) {
    int length = strlen(str);
    
    char* buffer = (char*)malloc(length);
    memset(buffer, 0, length);
    
    int endIndex = 0;
    int startIndex = 0;
    
    for(int i = 0; i < length; i++) {
        if(!isspace(str[i])) {
            startIndex = i;
            break;
        }
    }
    
    for(int i = length - 1; i > 0; --i) {
        if(!isspace(str[i])) {
            endIndex = i;
            break;
        }
    }
    
    int bufferIndex = 0;
    for(int i = startIndex; i <= endIndex; i++) {
        buffer[bufferIndex++] = str[i];
    }
    
    memset(str, 0, length);
    strcpy(str, buffer);
    free(buffer);
    
    return endIndex - startIndex;
}

void printOctals(char* str, int length) {
    
    printf("0x");
    
    for(int i = 0; i < length; i++) {
        printf("%02hhX", str[i]);
    }
    
    printf("\n");
    
}

const char* heapifyString(const char* n) {
    if(n == nullptr) return n;
    
    int length = strlen(n);
    char* result = new char[length];
    strcpy(result, n);
    
    return result;
}

const unsigned char* heapifyStringUnsigned(const unsigned char* n) {
    if(n == nullptr) return n;
    
    int length = strlen((const char*)n);
    unsigned char* result = new unsigned char[length];
    strcpy((char*)result, (const char*)n);
    
    return result;
}
