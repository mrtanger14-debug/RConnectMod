#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <iostream>

bool FileExists(const wchar_t* path) {
    return GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES;
}

void ShowError(const wchar_t* message) {
    MessageBoxW(nullptr, message, L"Error", MB_ICONERROR | MB_OK);
}

DWORD FindProcessByName(const wchar_t* name) {
    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return 0;

    if (Process32FirstW(snap, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, name) == 0) {
                DWORD pid = pe.th32ProcessID;
                CloseHandle(snap);
                return pid;
            }
        } while (Process32NextW(snap, &pe));
    }

    CloseHandle(snap);
    return 0;
}

bool InjectDLL(DWORD pid, const std::wstring& dllPath) {
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProc) return false;

    SIZE_T size = (dllPath.size() + 1) * sizeof(wchar_t);
    LPVOID remoteMem = VirtualAllocEx(hProc, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem) {
        CloseHandle(hProc);
        return false;
    }

    WriteProcessMemory(hProc, remoteMem, dllPath.c_str(), size, nullptr);

    LPVOID loadLib = GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "LoadLibraryW");
    if (!loadLib) {
        VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    HANDLE th = CreateRemoteThread(
        hProc,
        nullptr,
        0,
        (LPTHREAD_START_ROUTINE)loadLib,
        remoteMem,
        0,
        nullptr
    );

    if (!th) {
        VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
        CloseHandle(hProc);
        return false;
    }

    WaitForSingleObject(th, INFINITE);
    CloseHandle(th);
    VirtualFreeEx(hProc, remoteMem, 0, MEM_RELEASE);
    CloseHandle(hProc);
    return true;
}

int wmain() {
    std::wstring launcher_path = L"nrsc_launcher.exe";
    std::wstring dll_path = L"SeamlessCoop\\RConnectMod.dll";

    if (!FileExists(launcher_path.c_str())) {
        ShowError(L"The file nrsc_launcher.exe was not found in the current directory.");
        return 1;
    }

    if (!FileExists(dll_path.c_str())) {
        ShowError(L"The file SeamlessCoop\\RConnectMod.dll was not found.");
        return 1;
    }

    STARTUPINFOW si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);

    std::vector<wchar_t> cmd(launcher_path.begin(), launcher_path.end());
    cmd.push_back(L'\0');

    if (!CreateProcessW(
        nullptr,
        cmd.data(),
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi))
    {
        ShowError(L"Failed to launch the process nrsc_launcher.exe.");
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    DWORD pid = 0;
    while (!(pid = FindProcessByName(L"nightreign.exe"))) {
        Sleep(200);
    }

    if (!InjectDLL(pid, dll_path)) {
        ShowError(L"Failed to inject the DLL into nightreign.exe.");
    }   
    return 0;
}