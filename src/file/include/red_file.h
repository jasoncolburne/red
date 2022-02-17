/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_file.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_FILE_H__
#define __RED_FILE_H__

#include "red_context.h"

#ifdef __cplusplus
extern "C" {
#endif


struct _RedFile;
typedef struct _RedFile* RedFile;

/* file open flags (OR these) */
#define RED_FILE_OPEN_RDONLY   0x0001
#define RED_FILE_OPEN_WRONLY   0x0002
#define RED_FILE_OPEN_RDWR     0x0004
#define RED_FILE_OPEN_NONBLOCK 0x0008
#define RED_FILE_OPEN_APPEND   0x0010
#define RED_FILE_OPEN_CREAT    0x0020
#define RED_FILE_OPEN_TRUNC    0x0040
#define RED_FILE_OPEN_EXCL     0x0080
#define RED_FILE_OPEN_SHLOCK   0x0100
#define RED_FILE_OPEN_EXLOCK   0x0200
#define RED_FILE_OPEN_NOFOLLOW 0x0400
#define RED_FILE_OPEN_SYMLINK  0x0800
#define RED_FILE_OPEN_EVTONLY  0x1000

/* TODO: should redefine these to hide os specific stuff */
#define RED_FILE_POSITION_CURRENT 0x01
#define RED_FILE_POSITION_START   0x02
#define RED_FILE_POSITION_END     0x03

extern
int
redFileOpen(
    RedFile*    file,
    red_u32     flags,
    const char* path,
    RedContext  rCtx
    );

extern
int
redFileClose(
    RedFile* file
    );

extern
int
redFileWrite(
    RedFile     file,
    const void* data,
    size_t      bytes
    );

extern
int
redFileRead(
    RedFile file,
    void*   data,
    size_t* bytes
    );

extern
int
redFileReadAlloc(
    RedFile file,
    void**  data,
    size_t* bytes
    );

extern
int
redFileReadAllAlloc(
    RedFile file,
    void**  data,
    size_t* bytes
    );

/* relativeTo must be one of:
 * RED_FILE_POSITION_CURRENT
 * RED_FILE_POSITION_START
 * RED_FILE_POSITION_END
 */
extern
int
redFilePositionSet(
    RedFile file,
    red_i64 position,
    red_u32 relativeTo
    );

extern
int
redFilePositionGet(
    RedFile  file,
    red_i64* position
    );

extern
int
redFileLength(
    RedFile  file,
    red_i64* length
    );

extern
int
redFileOpenStdout(
    RedFile*   file,
    RedContext rCtx
    );

extern
int
redFileOpenStderr(
    RedFile*   file,
    RedContext rCtx
    );

extern
int
redFileOpenStdin(
    RedFile*   file,
    RedContext rCtx
    );


#ifdef __cplusplus
}
#endif


#endif /* __RED_FILE_H__ */
