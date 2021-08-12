// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NoirQBudgetBridgeSettings.generated.h"

/**
 *  Use this class to add an item to the preferences
 */
UCLASS(Config = Editor, defaultconfig)
class NOIRQBUDGETBRIDGE_API UNoirQBudgetBridgeSettings : public UObject
{
	GENERATED_BODY()
public: 
	UNoirQBudgetBridgeSettings(const FObjectInitializer& ObjectInitalizer);

	/*UPROPERTY(EditAnywhere, config, Category = Custom)
	bool testBoolean;

	UPROPERTY(EditAnywhere, config, Category = Custom, meta = (EditCondition = testBoolean))
	FString testString;*/

	UPROPERTY(EditAnywhere, config, Category = Custom)
	FString LoadLayoutPath;

	
};
