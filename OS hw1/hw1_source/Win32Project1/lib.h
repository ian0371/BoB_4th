#pragma once

#include "stdafx.h"

void print(_In_ const char* fmt, _In_ ...);
bool is_file_existsW(_In_ const wchar_t* file_path);

char* WcsToMbsUTF8(_In_ const wchar_t* wcs);
wchar_t* Utf8MbsToWcs(_In_ const char* utf8);
wchar_t * getCurrentDir();
bool create_bob_txt();
bool copy_bob_txt();
bool read_bob2_txt();
bool remove_bob_txt();