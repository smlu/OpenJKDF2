#include "wuRegistry.h"

#include "jk.h"

#ifdef WIN32_BLOBS

LSTATUS wuRegistry_Startup(HKEY hKey, LPCSTR lpSubKey, BYTE *lpData)
{
    LSTATUS result; // eax
    LSTATUS v6; // esi
    BOOL v7; // edi
    HKEY v8; // [esp+4Ch] [ebp-94h] BYREF
    HKEY phkResult; // [esp+50h] [ebp-90h] BYREF
    DWORD cbData; // [esp+54h] [ebp-8Ch] BYREF
    DWORD dwDisposition; // [esp+58h] [ebp-88h] BYREF
    DWORD Type; // [esp+5Ch] [ebp-84h] BYREF
    BYTE Data[128]; // [esp+60h] [ebp-80h] BYREF

    wuRegistry_bInitted = 1;
    wuRegistry_lpSubKey = lpSubKey;
    wuRegistry_hKey = hKey;
    result = jk_RegCreateKeyExA(hKey, lpSubKey, 0, wuRegistry_lpClass, 0, 0xF003Fu, 0, &phkResult, &dwDisposition);
    v6 = result;
    if ( !result )
    {
        jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &v8);
        cbData = 128;
        v7 = jk_RegQueryValueExA(v8, "Version", 0, &Type, Data, &cbData) == 0;
        result = jk_RegCloseKey(v8);
        if ( v7 )
        {
            result = _strcmpi((const char *)Data, (const char *)lpData);
            if ( result )
            {
                jk_RegDeleteKeyA(wuRegistry_hKey, wuRegistry_lpSubKey);
                result = jk_RegCreateKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, wuRegistry_byte_855EB4, 0, 0xF003F, 0, &phkResult, &dwDisposition);
                v6 = result;
            }
        }
    }
    if ( !v6 )
    {
        jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &v8);
        jk_RegSetValueExA(v8, "Version", 0, 1u, lpData, _strlen((const char *)lpData));
        jk_RegCloseKey(v8);
        result = jk_RegCloseKey(phkResult);
    }
    return result;
}

void wuRegistry_Shutdown()
{
    wuRegistry_bInitted = 0;
}

int wuRegistry_SaveInt(LPCSTR lpValueName, int val)
{
    HKEY phkResult; // [esp+0h] [ebp-4h] BYREF

    if ( jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult) )
        return 0;
    jk_RegSetValueExA(phkResult, lpValueName, 0, 3u, (const BYTE *)&val, 4u);
    jk_RegCloseKey(phkResult);
    return 1;
}

int wuRegistry_SaveFloat(LPCSTR lpValueName, float val)
{
    HKEY v2; // ecx
    HKEY phkResult; // [esp+0h] [ebp-4h] BYREF

    phkResult = v2;
    if ( jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult) )
        return 0;
    jk_RegSetValueExA(phkResult, lpValueName, 0, 3u, (const BYTE *)&val, 4u);
    jk_RegCloseKey(phkResult);
    return 1;
}

int wuRegistry_GetInt(LPCSTR lpValueName, int a2)
{
    HKEY phkResult; // [esp+0h] [ebp-Ch] BYREF
    DWORD cbData; // [esp+4h] [ebp-8h] BYREF
    BYTE Data[4]; // [esp+8h] [ebp-4h] BYREF

    if ( !jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult) )
    {
        cbData = 4;
        if ( !jk_RegQueryValueExA(phkResult, lpValueName, 0, (LPDWORD)&lpValueName, Data, &cbData) )
        {
            jk_RegCloseKey(phkResult);
            return *(int*)Data;
        }
        jk_RegCloseKey(phkResult);
    }
    return a2;
}

float wuRegistry_GetFloat(LPCSTR lpValueName, float v5)
{
    HKEY phkResult; // [esp+0h] [ebp-Ch] BYREF
    DWORD cbData; // [esp+4h] [ebp-8h] BYREF
    BYTE Data[4]; // [esp+8h] [ebp-4h] BYREF

    if ( !jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult) )
    {
        cbData = 4;
        if ( !jk_RegQueryValueExA(phkResult, lpValueName, 0, (LPDWORD)&lpValueName, Data, &cbData) )
        {
            jk_RegCloseKey(phkResult);
            return *(float *)Data;
        }
        jk_RegCloseKey(phkResult);
    }
    return v5;
}

int wuRegistry_SaveBool(LPCSTR lpValueName, HKEY phkResult)
{
    int Data; // [esp+0h] [ebp-4h] BYREF

    Data = (int)phkResult;
    if ( jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult) )
        return 0;
    jk_RegSetValueExA(phkResult, lpValueName, 0, REG_BINARY, &Data, 4u);
    jk_RegCloseKey(phkResult);
    return 1;
}

int wuRegistry_GetBool(LPCSTR lpValueName, int a2)
{
    HKEY phkResult; // [esp+0h] [ebp-Ch] BYREF
    DWORD cbData; // [esp+4h] [ebp-8h] BYREF
    int Data; // [esp+8h] [ebp-4h] BYREF

    if ( !jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult) )
    {
        cbData = 4;
        if ( !jk_RegQueryValueExA(phkResult, lpValueName, 0, (LPDWORD)&lpValueName, &Data, &cbData) )
        {
            jk_RegCloseKey(phkResult);
            return Data;
        }
        jk_RegCloseKey(phkResult);
    }
    return a2;
}

int wuRegistry_SaveBytes(LPCSTR lpValueName, BYTE *lpData, DWORD cbData)
{
    HKEY phkResult; // [esp+0h] [ebp-4h] BYREF

    if ( jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult) )
        return 0;
    jk_RegSetValueExA(phkResult, lpValueName, 0, REG_BINARY, lpData, cbData);
    jk_RegCloseKey(phkResult);
    return 1;
}

int wuRegistry_GetBytes(LPCSTR lpValueName, DWORD Type, DWORD cbData)
{
    HKEY phkResult; // [esp+0h] [ebp-4h] BYREF

    if ( !jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult) )
    {
        if ( !jk_RegQueryValueExA(phkResult, lpValueName, 0, &Type, (LPBYTE)Type, &cbData) )
        {
            jk_RegCloseKey(phkResult);
            return 1;
        }
        jk_RegCloseKey(phkResult);
    }
    return 0;
}

LSTATUS wuRegistry_SetString(LPCSTR lpValueName, BYTE *lpData)
{
    HKEY phkResult; // [esp+0h] [ebp-4h] BYREF

    jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult);
    jk_RegSetValueExA(phkResult, lpValueName, 0, REG_SZ, lpData, _strlen((const char *)lpData));
    return jk_RegCloseKey(phkResult);
}

int wuRegistry_GetString(LPCSTR lpValueName, LPBYTE lpData, int outSize, char *out)
{
    int result; // eax
    HKEY phkResult; // [esp+8h] [ebp-Ch] BYREF
    DWORD cbData; // [esp+Ch] [ebp-8h] BYREF
    DWORD Type; // [esp+10h] [ebp-4h] BYREF

    jk_RegOpenKeyExA(wuRegistry_hKey, wuRegistry_lpSubKey, 0, 0xF003Fu, &phkResult);
    cbData = outSize;
    if (jk_RegQueryValueExA(phkResult, lpValueName, 0, &Type, lpData, &cbData))
    {
        if (out)
        {
            _strncpy((char *)lpData, out, outSize - 1);
            lpData[outSize - 1] = 0;
        }
        jk_RegCloseKey(phkResult);
        result = 0;
    }
    else
    {
        jk_RegCloseKey(phkResult);
        result = 1;
    }
    return result;
}
#endif
