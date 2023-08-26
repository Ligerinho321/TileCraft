#ifndef STRING_UTILS_H_INCLUDE
#define STRING_UTILS_H_INCLUDE

#include <header.h>

int lenght(const char *const string);
char* append(char *const string,const char value,const int max);
char* cat(char *string,char *value_char,int value_int);
char* pop(char *const string);
char* new_string(const char *const string);
void trim(char *const string);
void erase(char *const string,const int start,const int end);
int find_first_not_of(const char *const string,const char *const niddle);
int find_last_not_of(const char *const string,const char *const niddle);
int find_first_of(const char *const string,const char niddle);
void substr(char *string,char *dst_string,int start,int end);
void remove_char(char *string,char *niddle);


#endif