#ifndef HELPER_H
#define HELPER_H

int printToBinary(int val, char* buf)
{
        // won't overflow because max Integer length <= 10
	itoa(val, buf, 2);
	auto len = strlen(buf);
	char temp[37] = { 0 };

	char* p;
	size_t i;
	for (i = 0; i < 32 - len; i++)
		temp[i] = '0';

	p = buf;
	for (i = 32 - len; i < 32; i++)
	{
		temp[i] = *p;
		p++;
	}

	int dashCnt = 0;
	p = buf;
	char* q = temp;

	for (i = 0; i < 35; i++)
	{
		dashCnt++;
		if (dashCnt == 9)
		{
			*p++ = '-';
			dashCnt = 0;
		}
		else
			*p++ = *q++;
	}
	*p = 0;
	return len;
}

#endif