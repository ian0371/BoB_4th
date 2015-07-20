#include "stdafx.h"
#include "lib.h"

/**
* @brief
* @param
* @see
* @remarks
* @code
* @endcode
* @return
**/
void print(_In_ const char* fmt, _In_ ...)
{
	char log_buffer[2048];
	va_list args;

	va_start(args, fmt);
	HRESULT hRes = StringCbVPrintfA(log_buffer, sizeof(log_buffer), fmt, args);
	if (S_OK != hRes)
	{
		fprintf(
			stderr,
			"%s, StringCbVPrintfA() failed. res = 0x%08x",
			__FUNCTION__,
			hRes
			);
		return;
	}

	OutputDebugStringA(log_buffer);
	fprintf(stdout, "%s \n", log_buffer);
}

bool is_file_existsW(_In_ const wchar_t* file_path)
{
	_ASSERTE(NULL != file_path);
	_ASSERTE(TRUE != IsBadStringPtrW(file_path, MAX_PATH));
	if ((NULL == file_path) || (TRUE == IsBadStringPtrW(file_path, MAX_PATH))) return false;

	WIN32_FILE_ATTRIBUTE_DATA info = { 0 };

	if (GetFileAttributesExW(file_path, GetFileExInfoStandard, &info) == 0)
		return false;
	else
		return true;
}

char* WcsToMbsUTF8(_In_ const wchar_t* wcs)
{
	_ASSERTE(NULL != wcs);
	if (NULL == wcs) return NULL;

	int outLen = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, NULL, 0, NULL, NULL);
	if (0 == outLen) return NULL;

	char* outChar = (char*)malloc(outLen * sizeof(char));
	if (NULL == outChar) return NULL;
	RtlZeroMemory(outChar, outLen);

	if (0 == WideCharToMultiByte(CP_UTF8, 0, wcs, -1, outChar, outLen, NULL, NULL))
	{
		print("WideCharToMultiByte() failed, errcode=0x%08x", GetLastError());

		free(outChar);
		return NULL;
	}

	return outChar;
}

wchar_t* Utf8MbsToWcs(_In_ const char* utf8)
{
	_ASSERTE(NULL != utf8);
	if (NULL == utf8) return NULL;

	int outLen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8, -1, NULL, 0);
	if (0 == outLen) return NULL;

	wchar_t* outWchar = (wchar_t*)malloc(outLen * (sizeof(wchar_t)));  // outLen contains NULL char 
	if (NULL == outWchar) return NULL;
	RtlZeroMemory(outWchar, outLen);

	if (0 == MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, utf8, -1, outWchar, outLen))
	{
		print("MultiByteToWideChar() failed, errcode=0x%08x", GetLastError());
		free(outWchar);
		return NULL;
	}

	return outWchar;
}

wchar_t * getCurrentDir() {
	// current directory 를 구한다.
	wchar_t *buf = NULL;
	uint32_t buflen = 0;
	buflen = GetCurrentDirectoryW(buflen, buf); // 디렉토리 문자열의 길이를 모르니 0이나 -1을 넣으면 return해줌
	if (0 == buflen)
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		return false;
	}

	buf = (PWSTR)malloc(sizeof(WCHAR) * buflen);
	if (0 == GetCurrentDirectoryW(buflen, buf))
	{
		print("err, GetCurrentDirectoryW() failed. gle = 0x%08x", GetLastError());
		free(buf);
		return false;
	}
	return buf;
}

bool create_bob_txt()
{
	wchar_t *buf = getCurrentDir();

	// current dir \\ bob.txt 파일명 생성
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW( // safe하도록 모든 boundary check를 다 해주는 API
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, cannot create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	if (true == is_file_existsW(file_name))
	{
		::DeleteFileW(file_name);
	}

	// 파일 생성
	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		print("err, CreateFile(path=%ws), gle=0x%08x", file_name, GetLastError());
		return false;
	}

	// 파일에 데이터 쓰기
	DWORD cbWritten = 0;
	BYTE ByteOrderMark[] = { 0xEF, 0xBB, 0xBF };
	if (TRUE != WriteFile(file_handle, ByteOrderMark, sizeof(ByteOrderMark), &cbWritten, NULL)) {
		print("err, BOM writing failed, gle=0x%08x", GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	wchar_t file_content[] = L"안녕하십니까. 이 테이프는 2015학년도 대학수학능력시험 언어 영역 듣기 평가 안내 녹음테이프입니다. 지금부터 2015학년도 대학수학능력시험 1교시 언어 영역 듣기 방송을 시작하겠습니다.";
	char *utf8_file_content = WcsToMbsUTF8(file_content);

	WriteFile(file_handle, utf8_file_content, strlen(utf8_file_content), &cbWritten, NULL);

	wchar_t file_content2[] = L"'Cause baby now we got bad blood. You know it used to be mad love. So take a look what you've done. 'Cause baby now we got bad blood.";
	utf8_file_content = WcsToMbsUTF8(file_content2);
	WriteFile(file_handle, utf8_file_content, strlen(utf8_file_content), &cbWritten, NULL);
	free(utf8_file_content);
	// 파일 닫기
	CloseHandle(file_handle);
	return true;
}

bool copy_bob_txt() {
	wchar_t *buf = getCurrentDir();
	wchar_t src_filename[260], des_filename[260];
	if (!SUCCEEDED(StringCbPrintfW(
		src_filename,
		sizeof(src_filename),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, cannot create src file name");
		free(buf);
		return false;
	}
	if (!SUCCEEDED(StringCbPrintfW(
		des_filename,
		sizeof(des_filename),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, cannot create des file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;
	if (!CopyFile(src_filename, des_filename, false)) {
		print("err, copy failed");
		return false;
	}
	return true;
}

bool read_bob2_txt() {
	wchar_t *buf = getCurrentDir();
	wchar_t file_name[260];
	if (!SUCCEEDED(StringCbPrintfW( // safe하도록 모든 boundary check를 다 해주는 API
		file_name,
		sizeof(file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, cannot create file name");
		free(buf);
		return false;
	}
	free(buf); buf = NULL;

	HANDLE file_handle = CreateFileW(
		file_name,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	char contents[1024]; DWORD nBytesRead;
	memset(contents, NULL, sizeof(contents) / sizeof(char));
	ReadFile(file_handle, contents, sizeof(contents) / sizeof(char), &nBytesRead, NULL);
	/* [1] Using ReadFile() */
	// console은 ANSI에 맞춰져있어서 UTF8 -> ANSI 필요
	wchar_t *ucs2_contents = Utf8MbsToWcs(contents + 3); // do not convert BOM
	int outLen = WideCharToMultiByte(CP_ACP, 0, ucs2_contents, -1, NULL, 0, NULL, NULL);
	memset(contents, NULL, sizeof(contents) / sizeof(char));
	if (0 == WideCharToMultiByte(CP_ACP, 0, ucs2_contents, -1, contents, outLen, NULL, NULL)) {
		print("err, UCS2 to ANSI failed.");
		free(ucs2_contents);
		CloseHandle(file_handle);
	}
	printf("[1] %s\n", contents);
	free(ucs2_contents);

	/* [2] Using memory-mapped I/O */
	HANDLE file_map = CreateFileMapping(
		file_handle,
		NULL,
		PAGE_READONLY,
		0,
		0,
		NULL
		);
	if (NULL == file_map)
	{
		print("err, CreateFileMapping(%ws) failed, gle = %u", file_name, GetLastError());
		CloseHandle(file_handle);
		return false;
	}

	PCHAR file_view = (PCHAR)MapViewOfFile(
		file_map,
		FILE_MAP_READ,
		0,
		0,
		0
		);
	if (NULL == file_view)
	{
		print("err, MapViewOfFile(%ws) failed, gle = %u", file_name, GetLastError());

		CloseHandle(file_map);
		CloseHandle(file_handle);
		return false;
	}

	ucs2_contents = Utf8MbsToWcs(file_view + 3);
	outLen = WideCharToMultiByte(CP_ACP, 0, ucs2_contents, -1, NULL, 0, NULL, NULL);
	memset(contents, NULL, sizeof(contents) / sizeof(char));
	if (0 == WideCharToMultiByte(CP_ACP, 0, ucs2_contents, -1, contents, outLen, NULL, NULL)) {
		print("err, UCS2 to ANSI failed.");
		free(ucs2_contents);
		CloseHandle(file_handle);
	}
	printf("[2] %s\n", contents);
	free(ucs2_contents);

	CloseHandle(file_handle);
	return true;
}

bool remove_bob_txt()
{
	wchar_t *buf = getCurrentDir();
	wchar_t file_name[260];

	//wcscat_s(buf, buflen+16, L"\\bob.txt");
	if (!SUCCEEDED(StringCbPrintfW( // safe하도록 모든 boundary check를 다 해주는 API
		file_name,
		sizeof(file_name),
		L"%ws\\bob.txt",
		buf)))
	{
		print("err, cannot delete bob.txt");
		free(buf);
		return false;
	}
	DeleteFile(file_name);

	if (!SUCCEEDED(StringCbPrintfW( // safe하도록 모든 boundary check를 다 해주는 API
		file_name,
		sizeof(file_name),
		L"%ws\\bob2.txt",
		buf)))
	{
		print("err, cannot delete bob2.txt");
		free(buf);
		return false;
	}
	DeleteFile(file_name);

	free(buf);
	return true;
}
