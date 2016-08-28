/*
 * This is not my code.  The following code
 * is from MarcusD's Mario Maker Project.
 * All credit goes for this file goes to him.
 */

#pragma once

#include <3ds.h>

#define FS_ATTRIBUTE_NONE 0

#ifdef __cplusplus
extern "C"
{
#endif

Result sdfs_init(char* r);
void sdfs_free();
Result sdfs_read2buf(const char* path, u64* size, char** bufptr);
Result sdfs_scandir(const char* path, char argv[][262], char extt[][16], u32* argc, char* ext);
Result sdfs_write2buf(const char* path, u64* size, char* bufptr);

#ifdef __cplusplus
}
#endif
