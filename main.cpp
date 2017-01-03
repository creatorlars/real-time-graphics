#include "pch.h"
#include "application.h"

int main()
{
	for (;;)
	{
		application application{};
		while (application.alive())
		{
			application.update();
		}

		if (!application.restart())
		{
			break;
		}
	}
}