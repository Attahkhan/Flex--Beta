#include "DLLMain.h"
#include "Utilities.h"
#include "INJ/ReflectiveLoader.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"
#include "Dumping.h"
#include "KnifeChanger.h"
#include "Xor.h"
#include "AntiLeak.h"

HINSTANCE HThisModule;
bool DoUnload;

int InitialThread()
{
	Offsets::Initialise(); 
	Interfaces::Initialise();
	NetVar.RetrieveClasses(); 
	NetvarManager::Instance()->CreateDatabase();
	Render::Initialise();
	Hacks::SetupHacks();
	Menu::SetupMenu();
	Hooks::Initialise();
	ApplyKnifeHooks();

	while (DoUnload == false)
	{
		Sleep(1000);
	}

	Hooks::UndoHooks();
	Sleep(2000); 
	FreeLibraryAndExitThread(HThisModule, 0);
	return 0;
}

bool _stdcall DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID lpReserved)
{

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:

		if (GetModuleHandleA("csgo.exe"))
		{
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);
		}

		else if (!GetModuleHandleA("csgo.exe"))
		{
			ExitProcess(NULL);
			return TRUE;
		}

	}
	return TRUE;
}