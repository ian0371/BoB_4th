�� create_very_big_file�δ� ���� ũ�Ⱑ 4GB Ȥ�� �� �̻��� ������ ���� �� ������?
�� ������ �ٷ� �Լ��� �������� �ִµ�, �� �Լ��� 2��° ���ڴ� uint32_t���̴�. �̴� 0xffffffff(�� 4.2GB)������ ������ ������ �� �ִ�.
����
file_size.QuadPart = (1024 * 1024) * size_in_mb;
�� �κп��� 1024 * 1024 * 0xffffffff�� �ϰ� �Ǹ� ��� 32-bit���̹Ƿ� �����÷ο찡 �Ͼ��.
![pic1](pic1.png)
���� 64-bit������ casting�� �ʿ��ϴ�.

memory mapped I/O�� ���� ���� ū ������ ������� �ʴ� ������ Error â�� ���� �� �� �ִ�. 
![pic2](pic2.png)
������ ���� �ڵ带 ���캸�� ������ ����.
_ASSERTE(fileSize.HighPart == 0);
LARGE_INTEGER�� HighPart�� 0���� �������༭ assertion error�� �ߴ� ���̴�.

�̸� ���ָ� ����� �Ǵ��� Ȯ���غ���.
![pic3](pic3.png)
MapViewOfFile���� ���������� �����ȴ�.
�� �Լ��� 4GB�� �Ѵ� Memory Map�� ������ �� ���� �����̴�.

���� ū ������ �Ѳ����� ��°�� �ű��� ����, ������� �ɰ��� ��ϸ��� �ű�� ����� ���ؾ��Ѵ�.
![pic5](pic5.png)
curOffset���� blockSize(������������ 512MB�� ����)��ŭ block�� �̵��ϸ鼭 MapViewOfFile�� ȣ���ؼ� copy�� �����ϰ�, ���� block�� copy�Ѵ�.
������ ����� ������ ����.
![time](time.png)
�����ϵ� �޸𸮺������� 5GB�ۿ� �׽�Ʈ���� ���Ͽ���.

Q. �� do { } while(false)�� ����ϴ���?