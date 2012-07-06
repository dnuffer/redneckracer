#include <stdio.h>
#include "miniblocxx/Format.hpp"

int main(int argc, const char** argv)
{
	blocxx::Format fmt("foo %<1:10.4X!> %<2:6.3!> %<3:!> %<4:!> %<5:10.3> %6", 123, 'b', true, "cde", 1.8, (char*)0);
	printf("%s\n", fmt.c_str());
	return 0;
}
