#include <string.h>
 
void main()
{  
    char s[] = "192.168.0.26";  
    char *delim = ".";  
    char *p;  
    printf("%s ", strtok(s, delim));  
 
    while((p = strtok(NULL, delim)))    
        printf("%s ", p);   
 
    printf("\n");
}