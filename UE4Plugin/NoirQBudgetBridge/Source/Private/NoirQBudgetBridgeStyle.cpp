#include "NoirQBudgetBridgePrivatePCH.h"
#include "NoirQBudgetBridgeStyle.h"
#include "SlateStyle.h"
#include "IPluginManager.h"


#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( FNoirQBudgetBridgeStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )


FString FNoirQBudgetBridgeStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("NoirQBudgetBridge"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr< FSlateStyleSet > FNoirQBudgetBridgeStyle::StyleSet = NULL;
TSharedPtr< class ISlateStyle > FNoirQBudgetBridgeStyle::Get() { return StyleSet; }

void FNoirQBudgetBridgeStyle::Initialize()
{
	const FVector2D Icon40x40(40.0f, 40.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}

	StyleSet = MakeShareable(new FSlateStyleSet("NoirQBudgetBridgeStyle"));
	StyleSet->SetContentRoot(FPaths::GetProjectFilePath() / TEXT("Content"));
	StyleSet->SetCoreContentRoot(FPaths::GetProjectFilePath() / TEXT("Content"));

	// Plugins Manager
	{
		StyleSet->Set("NoirQBudgetBridgeStyle.Icon", new IMAGE_BRUSH("Icon40", Icon40x40));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}

void FNoirQBudgetBridgeStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}