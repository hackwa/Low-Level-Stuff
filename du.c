#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// Run with proper perms else you get segfault

long long unsigned traverse(char*);

int checkifdir(mode_t str){
    if((str & S_IFMT) == S_IFDIR)
        return 1;
    return 0;
}

int main(int argc, char* argv[])
{
    char *path;
    if(argc > 1) {
        path = argv[1];
    }
    else {
        printf("Usage: du <path>\n");
        return 1;
    }
    long long unsigned size;
    size = traverse(path);
    //KiloBytes
    size = size/1024;
    printf("size is %llu\n",size);
    return 0;
}

long long unsigned sizeparser(long long unsigned arg)
{
    int mod = arg%4096;
    if(mod)
        arg = arg + (4096 - mod);
    return arg;

}

long long unsigned traverse(char* path)
{
    DIR *wa = opendir(path);
    struct dirent *mystruct = NULL;
    struct stat mystat;
    long long unsigned actualsize,dirsize=0,returnval;
    while(1) {
        mystruct = readdir(wa);
        if (mystruct == NULL) {
            //closedir(wa);
            break;
        }
        char *filename = mystruct->d_name;
        if(strcmp("..",filename) == 0)
            continue;
        //get full path
        char *str = malloc(strlen(filename)+strlen(path)+2);
        strcat(str,path);
        strcat(str,"/");
        strcat(str,filename);
        //printf("%s \t",str);
        stat(str,&mystat);
        if(checkifdir(mystat.st_mode)) {
           //no need to traverse current file
            if (strcmp(".",filename) == 0){
                actualsize = sizeparser(mystat.st_size);
                dirsize +=actualsize;
                continue;
            }
            returnval = traverse(str);
            dirsize += returnval;
            //printf("%s\t%llu\n",str,returnval/1024);
            continue;
        }
        actualsize = sizeparser(mystat.st_size);
        //printf("%llu\n",actualsize/1025);
        dirsize +=actualsize;
    }
    return dirsize;
}
