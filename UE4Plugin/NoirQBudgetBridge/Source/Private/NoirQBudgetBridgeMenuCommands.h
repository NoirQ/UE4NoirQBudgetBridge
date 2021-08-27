#pragma once


class FNoirQBudgetBridgeCommands : public TCommands<FNoirQBudgetBridgeCommands>
{

public:
	FNoirQBudgetBridgeCommands() : TCommands<FNoirQBudgetBridgeCommands>
		(
			"NoirQBudgetBridge",
			NSLOCTEXT("Contexts", "NoirQBudgetBridge", "NoirQBudgetBridge"),
			NAME_None,
			FNoirQBudgetBridgeCommands::GetStyleSetName() // Icon Style Set
			)
	{}

	/** Toggle play */
	TSharedPtr< FUICommandInfo > TestButton;

	// Load Bridge File
	TSharedPtr< FUICommandInfo > LoadBridgeButton;

	virtual void RegisterCommands() override;
};
