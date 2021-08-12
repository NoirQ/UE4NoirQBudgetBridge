#include "NoirQBudgetBridgePrivatePCH.h"
#include "NoirQBudgetBridgeMenuCommands.h"

void FNoirQBudgetBridgeCommands::RegisterCommands()
{
	//UI_COMMAND(TestButton, "TestButton", "This button does not do anything", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(LoadBridgeButton, "LoadBridgeFile", "Load Json File and add to scene", EUserInterfaceActionType::Button, FInputChord());
}

