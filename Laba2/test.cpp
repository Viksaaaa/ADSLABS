#include "shannon_fano.h"

int main()
{
	setlocale(LC_ALL, "");
	shannon_fano device{};
	device.encode("���� ������� �� ������� ����������� ����� ������� � �� ��������. ��� � ���� �������.");
	device.info();
	return 0;
}
