#include <iostream>
#include "Server.h"

int main()
{
	Server server;
	while (1)
	{
		server.ClientConnent();
	}
	system("pause");
	return 0;
}