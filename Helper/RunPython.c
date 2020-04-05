#include "RunPython.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

void StartProcess(char* CmdLine)
{
#if defined(linux)

#endif
#if defined(_WINDOWS_) || defined(_WIN32)
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process.
    if( !CreateProcess( NULL,   // No module name (use command line)
                        CmdLine,        // Command line
                        NULL,           // Process handle not inheritable
                        NULL,           // Thread handle not inheritable
                        FALSE,          // Set handle inheritance to FALSE
                        0,              // No creation flags
                        NULL,           // Use parent's environment block
                        NULL,           // Use parent's starting directory
                        &si,            // Pointer to STARTUPINFO structure
                        &pi )           // Pointer to PROCESS_INFORMATION structure
            )
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
#endif

}

void RunPythonScript(char script_path[MAX_FILE_PATH], char *args){
    unsigned int length = 0;
    length += strlen(args);
    char *command = calloc(MAX_FILE_PATH + strlen(PYTHON_EXE_PATH)+length +10,1);
    unsigned int offset = 0;

    memcpy(command + offset,PYTHON_EXE_PATH,strlen(PYTHON_EXE_PATH));
    offset += strlen(PYTHON_EXE_PATH);
    command[offset] = ' ';
    offset++;

    memcpy(command + offset,script_path,strlen(script_path));
    offset += strlen(script_path);
    command[offset] = ' ';
    offset++;

    memcpy(command + offset,args,strlen(args));
    offset += strlen(PYTHON_EXE_PATH);

    StartProcess(command);
    free(command);
}
