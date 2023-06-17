#include "get_next_line.h"

int main()
{
	int fd;
	char *ok;

	fd = open("test.txt", O_RDONLY);
	ok = get_next_line(fd);
	printf("%s", ok);
	while (ok != NULL)
	{
		free(ok);
		ok = get_next_line(fd);
		if (ok != NULL)
			printf("%s", ok);
	}
	close(fd);
	return 0;
}
