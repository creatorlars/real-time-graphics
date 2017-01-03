#include "pch.h"
#include "application.h"

int main()
{
	for (;;)
	{
		application app{};
		while (app.alive())
		{
			app.update();
		}

		if (!app.restart())
		{
			break;
		}
	}
}