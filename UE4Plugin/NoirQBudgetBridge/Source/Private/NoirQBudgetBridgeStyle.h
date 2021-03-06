// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

class FNoirQBudgetBridgeStyle
{
public:

	static void Initialize();
	static void Shutdown();

	static TSharedPtr< class ISlateStyle > Get();

private:

	static FString InContent(const FString& RelativePath, const ANSICHAR* Extension);

private:

	static TSharedPtr< class FSlateStyleSet > StyleSet;

	static FName GetStyleSetName();
};