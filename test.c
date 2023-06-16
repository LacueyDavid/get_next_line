#include "get_next_line.h"

int main()
{
	int fd;
	char *ok;

	fd = open("test.txt", O_RDONLY);

	while (ok != NULL)
	{
		ok = get_next_line(fd);
		if (ok != NULL)
			printf("the string:%s", ok);
		free(ok);
	}

	close(fd);
	return 0;
}
