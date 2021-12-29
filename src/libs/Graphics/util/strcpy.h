char* strcpy(char *target, char *source){
    int i = 0;
    
    while(source[i] != '\0'){
        target[i] = source[i];
        i++;
    }

    return target;
}