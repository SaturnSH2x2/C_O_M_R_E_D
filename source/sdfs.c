/*
 * This is not my code.  The following code
 * is from MarcusD's REDThreeUp IPS Patcher.
 * All credit goes for this file goes to him.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sdfs.h"

#include <3ds.h>

FS_Archive sdroot;
FS_Path rootdir;
char root[262];

Result sdfs_init(char* r)
{
    memset(root, 0, sizeof(root));
    strcpy(root, r);
    rootdir = fsMakePath(PATH_ASCII, root);
    Result res = FSUSER_OpenArchive(&sdroot, ARCHIVE_SDMC, (FS_Path){PATH_EMPTY, 1, (u8*)""});
    if(R_FAILED(res))
    {
        puts("[SDFS] Can't open SDCard!");
        FSUSER_CloseArchive(sdroot);
        return res;
    }
    return 0;
}

void sdfs_free()
{
    if(FSUSER_CloseArchive(sdroot))
    {
        printf("[SDFS] Can't close SDCard!");
    }
}

Result sdfs_read2buf(const char* path, u64* size, char** bufptr)
{
    u64 temp;
    size_t len = strlen(root) + strlen(path);
    char concat[len + 1];
    memset(concat, 0, sizeof(concat));
    strcat(concat, root);
    strcat(concat, path);
    printf("[SDFS] Reading file \"%s\"\n", concat);
    FS_Path pth = fsMakePath(PATH_ASCII, concat);
    
    if(!size)
    {
        size = &temp;
    }
    
    Handle filehand;
    
    Result res;
    res = FSUSER_OpenFile(&filehand, sdroot, pth, FS_OPEN_READ, FS_ATTRIBUTE_NONE);
    if(res)
    {
        printf("[SDFS] OpenFileDirectly error: %08X\n", res);
        return res;
    }
    
    res = FSFILE_GetSize(filehand, size);
    
    if(res)
    {
        printf("[SDFS] GetSize error: %08X\n", res);
        return res;
    }
    
    printf("[SDFS] Allocating %llu bytes\n", *size);
    
    void* outbuf = malloc(*size);
    
    if(!outbuf)
    {
        printf("[SDFS] Can't allocate buffer! %08X\n", -1);
        return -1;
    }
    
    printf("[SDFS] Buffer pointer: %p\n", outbuf);
    
    *bufptr = outbuf;
    
    u32 bytesread;
    
    res = FSFILE_Read(filehand, &bytesread, 0, outbuf, *size);
    if(res)
    {
        printf("[SDFS] Can't read file: %08X\n", res);
        return res;
    }
    
    if(bytesread != *size)
    {
        printf("[SDFS] File sizes differ! %lu | %llu\n", bytesread, *size);
        return -10;
    }
    
    res = FSFILE_Close(filehand);
    if(res)
    {
        printf("[SDFS] Can't close the file handle: %08X\n", res);
        return res;
    }
    
    return 0;
}

Result sdfs_scandir(const char* path, char argv[][262], char extt[][16], u32* argc, char* ext)
{
    size_t len = strlen(root) + strlen(path);
    printf("Root: %s\nPath: %s\n", root, path);
    char concat[len + 1];
    memset(concat, 0, sizeof(concat));
    strcat(concat, root);
    strcat(concat, path);
    printf("[SDFS] Reading directory \"%s\"\n", concat);
    FS_Path pth = fsMakePath(PATH_ASCII, concat);
    
    Handle dirhand = 0;
    
    Result res = FSUSER_OpenDirectory(&dirhand, sdroot, pth);
    if(res)
    {
        printf("[SDFS] Can't open directory: %08X\n", res);
        return res;
    }
    
    FS_DirectoryEntry de;
    u32 read = 0;
    char namebuf[262];
    
    while(1)
    {
        res = FSDIR_Read(dirhand, &read, 1, &de);
        if(res)
        {
            printf("[SDFS] Can't scan directory: %08X\n", res);
            return res;
        }
        
        if(!read) break;
        
        memset(namebuf, 0, 262);
        utf16_to_utf8(namebuf, de.name, 262);
        
        printf("[SDFS] > %s\n", namebuf);
        
        if(ext != 0 && ext != -1 && ext != -2 && strcmp(namebuf + strlen(namebuf) - strlen(ext), ext)) continue;
        if(ext == -1) if(!(de.attributes & FS_ATTRIBUTE_DIRECTORY)) continue;
        if(ext == -2) if(de.attributes & FS_ATTRIBUTE_DIRECTORY) continue;
        
        memset(argv[*argc], 0, 262);
        if(de.attributes & FS_ATTRIBUTE_DIRECTORY) strcat(argv[*argc], "/");
        strcat(argv[*argc], namebuf);
        if(extt)
        {
            memset(extt[*argc], 0, 16);
            if(ext != -1 && ext != -2 && ext) strcat(extt[*argc], ext);
            else if(!(de.attributes & FS_ATTRIBUTE_DIRECTORY))
            {
                char* st = argv[*argc];
                st += strlen(st);
                while(*--st != '.' && st != argv[*argc]);
                if(*st == '.') strcat(extt[*argc], st);
            }
        }
        
        *argc = *argc + 1; // *argc++; // statement has no effect... wtf?!
    }
    
    res = FSDIR_Close(dirhand);
    if(res)
    {
        printf("[SDFS] Can't close directory: %08X\n", res);
        return res;
    }
    
    
    return 0;
}

Result sdfs_write2buf(const char* path, u64* size, char* bufptr)
{
    size_t len = strlen(root) + strlen(path);
    char concat[len + 1];
    memset(concat, 0, sizeof(concat));
    strcat(concat, root);
    strcat(concat, path);
    printf("[SDFS] Writing file \"%s\"\n", concat);
    FS_Path pth = fsMakePath(PATH_ASCII, concat);

    Handle filehand;

    Result res;
    res = FSUSER_OpenFile(&filehand, sdroot, pth, FS_OPEN_WRITE, FS_ATTRIBUTE_NONE);
    if(res)
    {
        printf("[SDFS] OpenFileDirectly error: %08X\n", res);
        return res;
    }

    printf("[SDFS] Buffer pointer: %p\n", bufptr);
    
    u32 read = 0;

    res = FSFILE_Write(filehand, &read, 0, bufptr, size, 0);
    if(res)
    {
        //printf("[SDFS] Can't write to file: %08X\n", res);
        return res;
    }
    
    if(read != size)
    {
        //printf("[SDFS] Size doesn't match! %d || %d\n", size, read);
        return -1;
    }
    
    puts("[SDFS] Flushing...");
    
    res = FSFILE_Flush(filehand);
    if(res)
    {
        //printf("[SDFS] Can't flush file contents: %08X\n", res);
        return res;
    }

    res = FSFILE_Close(filehand);
    if(res)
    {
        //printf("[SDFS] Can't close the file handle: %08X\n", res);
        return res;
    }
    
    return 0;
}