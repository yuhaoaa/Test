#include "findfile.h"

HANDLE hThread = NULL;
HANDLE hEvent = NULL;
volatile BOOL isFind = true;
volatile BOOL findAll = false;
FileInfoCallbackFun fileCallback;

struct INPARAM
{
    WCHAR filePath[1024];
};

void getFiles(LPVOID inparam);

void startFindFiles(WCHAR* filePath, FileInfoCallbackFun callback)
{
    fileCallback = callback;
    isFind = true;
    findAll = false;

    if (hEvent != NULL)
    {
        CloseHandle(hEvent);
    }

    hEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

    if (hThread != NULL)
    {
        CloseHandle(hThread);
    }

    INPARAM inparam;
    wcscpy(inparam.filePath, filePath);
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)getFiles, (LPVOID)&inparam, 0, NULL);
}

BOOL isFindAll()
{
    return findAll;
}

void pauseFindFiles(BOOL pause)
{
    if (pause)
    {
        ResetEvent(hEvent);
    }
    else
    {
        SetEvent(hEvent);
    }
}

void stopFindFiles()
{
    isFind = false;

    CloseHandle(hEvent);
    hEvent = NULL;
    CloseHandle(hThread);
    hThread = NULL;
}

void getFiles(LPVOID inparam)
{
    INPARAM* param = (INPARAM*)inparam;
    WIN32_FIND_DATA findFileData;

    HANDLE hFind = FindFirstFile(param->filePath, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        return;
    }

    if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0)
    {
        FILE_INFO fileInfo;
        wcscpy(fileInfo.fileName, findFileData.cFileName);
        fileInfo.fileSize = (findFileData.nFileSizeHigh << 32) + findFileData.nFileSizeLow;

        if (fileCallback)
        {
            fileCallback(&fileInfo);
        }
    }

    while (isFind)
    {
        WaitForSingleObject(hEvent, INFINITE);

        Sleep(10);
        BOOL ret = FindNextFile(hFind, &findFileData);

        if (ret)
        {
            if (findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
            {
                if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0)
                {
                    FILE_INFO fileInfo;
                    wcscpy(fileInfo.fileName, findFileData.cFileName);
                    fileInfo.fileSize = (findFileData.nFileSizeHigh << 32) + findFileData.nFileSizeLow;

                    if (fileCallback)
                    {
                        fileCallback(&fileInfo);
                    }
                }
            }
            else
            {
                FILE_INFO fileInfo;
                wcscpy(fileInfo.fileName, findFileData.cFileName);
                fileInfo.fileSize = (findFileData.nFileSizeHigh << 32) + findFileData.nFileSizeLow;

                if (fileCallback)
                {
                    fileCallback(&fileInfo);
                }
            }
        }
        else
        {
            findAll = true;
            ResetEvent(hEvent);
        }

        if (!isFind)
        {
            ResetEvent(hEvent);
        }
    }

    return;
}

