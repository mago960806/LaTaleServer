
#pragma once

#if defined(_LAYER_THREAD_LOAD)

DWORD WINAPI EquipInitThread(LPVOID lParam);
DWORD WINAPI EquipOnThread(LPVOID lParam);
DWORD WINAPI EquipOffThread(LPVOID lParam);
DWORD WINAPI WeaponChangeThread(LPVOID lParam);

#endif