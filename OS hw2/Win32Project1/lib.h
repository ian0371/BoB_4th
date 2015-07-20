#pragma once
#include "stdafx.h"

#define MB ((1024)*(1024))
#define GB (MB*(1024))

void print(_In_ const char* fmt, _In_ ...);
bool is_file_existsW(_In_ const wchar_t* file_path);
wchar_t * getCurrentDir();