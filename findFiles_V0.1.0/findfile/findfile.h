#ifndef FINDFILE_H
#define FINDFILE_H

#include <Windows.h>

#ifdef WIN32
#ifdef FINDFILE_LIBRARY
#define FINDFILE_EXPORT __declspec(dllexport)
#else
#define FINDFILE_EXPORT
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_LEN 256

struct FILE_INFO
{
    WCHAR fileName[MAX_LEN];
    __int64 fileSize;
};

typedef void (*FileInfoCallbackFun) (FILE_INFO* fileInfo);
FINDFILE_EXPORT void startFindFiles(WCHAR* filePath, FileInfoCallbackFun callback = nullptr);
FINDFILE_EXPORT BOOL isFindAll();
FINDFILE_EXPORT void pauseFindFiles(BOOL pause = true);
FINDFILE_EXPORT void stopFindFiles();

#ifdef __cplusplus
}

#endif
#endif // FINDFILE_H
