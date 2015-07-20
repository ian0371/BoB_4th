#include "stdafx.h"
#include "mmio.h"
#include "lib.h"
#include "FileIoHelper.h"
#include "FileIoHelperClass.h"

class StopWatch
{
private:
	LARGE_INTEGER	mFreq, mStart, mEnd;
	float			mTimeforDuration;
public:
	StopWatch() : mTimeforDuration(0)
	{
		mFreq.LowPart = mFreq.HighPart = 0;
		mStart = mFreq;
		mEnd = mFreq;
		QueryPerformanceFrequency(&mFreq);
	}
	~StopWatch()
	{
	}

public:
	void Start(){ QueryPerformanceCounter(&mStart); }
	void Stop()
	{
		QueryPerformanceCounter(&mEnd);
		mTimeforDuration = (mEnd.QuadPart - mStart.QuadPart) / (float)mFreq.QuadPart;
	}
	float GetDurationSecond() { return mTimeforDuration; }
	float GetDurationMilliSecond() { return mTimeforDuration * 1000.f; }

};

#define WHICH 2
#if WHICH==1
#define MYPATH L"C:\\Users\\Song\\Documents\\GitHub\\BoB_4th\\OS hw2\\Debug\\"
#elif WHICH==2
#define MYPATH L""
#elif WHICH==3
#define MYPATH L"E:\\test\\"
#endif

int main(int argc, char *argv[]){
	/* Doesn't work with large files
	_ASSERTE(create_very_big_file(L"huge.txt", 4000));
	StopWatch s;
	s.Start();
	file_copy_using_read_write(L"huge.txt",
		L"huge_RW.txt");
	s.Stop();
	printf("RW: %f\n", s.GetDurationMilliSecond());
	s.Start();
	file_copy_using_read_write(L"huge.txt",
		L"huge_mmio.txt");
	s.Stop();
	printf("mmio: %f\n", s.GetDurationMilliSecond());
	return 0;
	*/
	FileIoHelper f;
	LARGE_INTEGER fileSize;
	fileSize.QuadPart = (unsigned long long)5 * GB;

	f.FIOCreateFile(MYPATH L"huge.txt", fileSize);
	printf("File Creation Complete\n");
	f.~f();

	StopWatch s;
	s.Start();
	if (!file_copy_using_memory_map_extended(
		MYPATH L"huge.txt",
		MYPATH L"huge_mmio.txt")) {
		print("file_copy_using_memory_map_extended() failed.\n");
	}
	s.Stop();
	printf("[+] copying via mmio: %fs\n", s.GetDurationSecond());
	
	s.Start();
	if (!file_copy_using_read_write(
		MYPATH L"huge.txt",
		MYPATH L"huge_rw.txt")) {
		print("file_copy_using_read_write() failed.\n");
	}
	s.Stop();
	printf("[+] copying via rw: %fs\n", s.GetDurationSecond());
}
