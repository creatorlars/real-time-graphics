#include "pch.h"
#include "application.h"

int main()
{
	for (;;)
	{
		auto app = application{};
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