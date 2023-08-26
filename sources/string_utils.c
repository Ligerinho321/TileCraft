#include <string_utils.h>

int lenght(const char *const string){
    int lenght = 0;
    if(string != NULL){
        for(lenght; string[lenght] != '\0'; lenght++);
    }
    return lenght;
}

char* append(char *const string,const char value,const int max){
    int size = lenght(string);
    if(size < max && size > 0 && value != '\0'){
        char *new_string = calloc(size+2,sizeof(char));
        strncpy(new_string,string,size);
        free(string);
        new_string[lenght(new_string)] = value;
        return new_string;
    }
    else if(size == 0 && value != '\0'){
        char *new_string = calloc(2,sizeof(char));
        new_string[0] = value;
        return new_string;
    }
    return string;
}

char* cat(char *string,char *value_char,int value_int){
    size_t size_string = lenght(string);
    if(value_char){
        size_t size_value = lenght(value_char);
        
        char *new_string = calloc(size_string+size_value+1,sizeof(char));
        if(size_string > 0){
            strncpy(new_string,string,size_string);
            strncat(new_string,value_char,size_value);
            free(string);
        }
        else{
            strncpy(new_string,value_char,size_value);
        }

        return new_string;
    }
    else{
        char buffer[MAX_DIGIT];
        snprintf(buffer,MAX_DIGIT,"%d",value_int);
        size_t size_buffer = lenght(buffer);

        char *new_string = calloc(size_string+size_buffer+1,sizeof(char));
        if(size_string > 0){
            strncpy(new_string,string,size_string);
            strncat(new_string,buffer,size_buffer);
            free(string);
        }
        else{
            strncpy(new_string,buffer,size_buffer);
        }

        return new_string;
    }
}

char* pop(char *const string){
    int size = lenght(string);
    if(size > 1){
        char *new_string = calloc(size,sizeof(char));
        strncpy(new_string,string,size-1);
        free(string);
        return new_string;
    }
    else{
        free(string);
        return NULL;
    }
}

char* new_string(const char *const string){
    int size = lenght(string);
    if(size){
        char *buffer = calloc(size+1,sizeof(char));
        strncpy(buffer,string,size);
        return buffer;
    }
    else{
        return NULL;
    }
}

void trim(char *const string){
    erase(string,0,find_first_not_of(string," \n\r\t\f\v"));
    erase(string,find_last_not_of(string," \n\r\t\f\v")+1,lenght(string));
}

void erase(char *const string,const int start,const int end){

    size_t size_string = lenght(string);

    char *buffer = calloc(size_string+1,sizeof(char));

    size_t pos = 0;

    for(int i=0; i<lenght(string); i++){
        if(i<start || i>=end){
            buffer[pos] = string[i];
            ++pos;
        }
    }
    strncpy(string,buffer,size_string);
    free(buffer);
}

int find_first_not_of(const char *const string,const char *const niddle){
    int checked = 0;
    for(int i=0; i<lenght(string); i++){
        checked = 0;
        for(int j=0; j<lenght(niddle); j++){
            if(niddle[j] == string[i]){
                break;
            }
            else{
                checked++;
            }
        }
        if(checked == lenght(niddle)) return i;
    }

    return 0;
}

int find_last_not_of(const char *const string,const char *const niddle){
    int last_occurrence = 0;
    int checked = 0;

    for(int i=0; i<lenght(string); i++){
        checked = 0;
        for(int j=0; j<lenght(niddle); j++){
            if(niddle[j] == string[i]){
                break;
            }
            else{
                checked++;
            }
        }
        if(checked == lenght(niddle)) last_occurrence = i;
    }

    return last_occurrence;
}

int find_first_of(const char *const string,const char niddle){
    for(int i=0; i<lenght(string); i++){
        if(string[i] == niddle) return i;
    }

    return 0;
}

void substr(char *string,char *dst_string,int start,int end){

    memset(dst_string,'\0',lenght(dst_string));

    size_t pos = 0;

    for(int i=0; i<lenght(string); i++){
        if(i>=start && i<end){
            dst_string[pos] = string[i];
            ++pos;
        }
    }
}

void remove_char(char *string,char *niddle){

    int size_string = lenght(string);
    int size_niddle = lenght(niddle);

    char *buffer = calloc(size_string+1,sizeof(char));

    int check = 0;
    int pos = 0;

    for(int i=0; i<size_string; ++i){
        for(int j=0; j<size_niddle; ++j){
            if(string[i] != niddle[j]) ++check;
        }
        if(check == size_niddle){
            buffer[pos] = string[i];
            ++pos;
        };
        check = 0;
    }

    strncpy(string,buffer,size_string);
    free(buffer);
}