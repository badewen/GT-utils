#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "./include/enet/enet.h"
#include <Windows.h>
#include "main.h"
#include <ctime>
#include <exception>
#include <ios>
#include <sstream>
#include <string.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string_view>
#include "./include/MinHook/MinHook.h"
#include <Psapi.h>
#include <random>
#include <mutex>
#include <iphlpapi.h>

#include "Hooks.h"
#include "Utils.h"


HINSTANCE dllh = 0;

#define LOGI std::cout 

void mainhack();

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved )  // reserved
{
    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:    
            DisableThreadLibraryCalls(hinstDLL);
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mainhack , nullptr, 0, 0);
            dllh = hinstDLL;
            break;

        case DLL_THREAD_ATTACH:

        case DLL_THREAD_DETACH:

        case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

void mainhack() {
    AllocConsole();
    
    HMODULE loaded_mod[1024];
    DWORD loaded_mod_count = 0;
    FILE* stdoutf = 0;
    HANDLE gt_proc = 0;

    uint8_t some_packet[] = {0x4, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    freopen_s(&stdoutf, "packet_logs.txt", "w", stdout);
    std::cout << "HAHAHAHAHAH";

    gt_proc = GetCurrentProcess();

    Hooks::Init();

    std::cout << '\n' << Utils::random_number_uint(8) << '\n' << Utils::random_number_hex(8) << '\n' << Utils::random_mac() << '\n'; 

    if (!EnumProcessModulesEx(gt_proc, loaded_mod, sizeof(loaded_mod), &loaded_mod_count, LIST_MODULES_64BIT)) {
        std::cout << "EnumProcessModules Failed\n";
        std::cout << GetLastError() << '\n';
    }

    // 1023 + null terminator
    WCHAR mod_name[1024];

    std::cout << "LOADED MODULES : \n";

    for (int i = 0; i < loaded_mod_count/ sizeof(HMODULE); i++) {
        if (!GetModuleBaseNameW(gt_proc, loaded_mod[i], mod_name, sizeof(mod_name) - 1)) {
            std::cout << "GetModuleBaseName Failed";
            std::cout << GetLastError() << '\n';
        }
        std::cout << mod_name << '\n';
        memset(mod_name, 0, sizeof(mod_name));
    }


    while(1) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x0001) {
            break;
        }
    }

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    FreeConsole();
    fclose(stdoutf);
    FreeLibraryAndExitThread(dllh, 0);
}