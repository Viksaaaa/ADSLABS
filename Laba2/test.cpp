#include "shannon_fano.h"

int main()
{
	setlocale(LC_ALL, "");
	shannon_fano device{};
	device.encode("Если человек не потерял способности ждать счастья — он счастлив. Это и есть счастье.");
	device.info();
	return 0;
}
