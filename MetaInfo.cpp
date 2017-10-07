/*
Tim's FlexBeta Framework 2017
*/

#include "MetaInfo.h"
#include "Utilities.h"

void PrintMetaHeader()
{
	printf("                                 Flex");
	Utilities::SetConsoleColor(FOREGROUND_INTENSE_GREEN);
	printf("Ware\n");
	Utilities::SetConsoleColor(FOREGROUND_WHITE);
	Utilities::Log("Build %s", __DATE__);
	Utilities::Log("Setting Up FlexBeta for %s", FlexBeta_META_GAME);
}