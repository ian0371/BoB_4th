�� create_very_big_file�δ� ���� ũ�Ⱑ 4GB Ȥ�� �� �̻��� ������ ���� �� ������?
�� ������ �ٷ� �Լ��� �������� �ִµ�, �� �Լ��� 2��° ���ڴ� uint32_t���̴�. �̴� 0xffffffff(�� 4.2GB)������ ������ ������ �� �ִ�.
����
file_size.QuadPart = (1024 * 1024) * size_in_mb;
�� �κп��� 1024 * 1024 * 0xffffffff�� �ϰ� �Ǹ� ��� 32-bit���̹Ƿ� �����÷ο찡 �Ͼ��.<br/>

![pic1](pic1.png)<br/>

���� 64-bit������ casting�� �ʿ��ϴ�.

memory mapped I/O�� ���� ���� ū ������ ������� �ʴ� ������ Error â�� ���� �� �� �ִ�. <br/>

![pic2](pic2.png)<br/>

������ ���� �ڵ带 ���캸�� ������ ����.
_ASSERTE(fileSize.HighPart == 0);
LARGE_INTEGER�� HighPart�� 0���� �������༭ assertion error�� �ߴ� ���̴�.

�̸� ���ָ� ����� �Ǵ��� Ȯ���غ���.<br/>

![pic3](pic3.png)<br/>

MapViewOfFile���� ���������� �����ȴ�.
�� �Լ��� 4GB�� �Ѵ� Memory Map�� ������ �� ���� �����̴�.

���� ū ������ �Ѳ����� ��°�� �ű��� ����, ������� �ɰ��� ��ϸ��� �ű�� ����� ���ؾ��Ѵ�.<br/>

![pic5](pic5.png)<br/>

curOffset���� blockSize(������������ 512MB�� ����)��ŭ block�� �̵��ϸ鼭 MapViewOfFile�� ȣ���ؼ� copy�� �����ϰ�, ���� block�� copy�Ѵ�.
������ ����� ������ ����.<br/>

![time](time.png)<br/>

�����ϵ� �޸𸮺������� 5GB�ۿ� �׽�Ʈ���� ���Ͽ���.

Q. �� do { } while(false)�� ����ϴ���?