#include "io_utils.h"

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/stat.h>
#endif

bool io_utils::is_folder_exists(const char* path)
{
#ifdef _WIN32
    DWORD result = GetFileAttributesA(path);
    if (result == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    return result == FILE_ATTRIBUTE_DIRECTORY;
#else
    // TODO: test on Linux
    struct stat info_system = { 0 };
    if (stat(path, &info_system)) {
        return false
    }
    return S_ISDIR(info_system.st_mode);
#endif
}

void io_utils::create_folder(const char* path)
{
#ifdef _WIN32
    _mkdir(path);
#else
    mkdir(path, 0755);
#endif
}
