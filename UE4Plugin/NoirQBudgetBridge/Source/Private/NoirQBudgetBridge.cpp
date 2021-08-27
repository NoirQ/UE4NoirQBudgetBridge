// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "NoirQBudgetBridgePrivatePCH.h"
#include "NoirQBudgetBridge.h"
#include "NoirQBudgetBridgeMenuCommands.h"

#include "NoirQBudgetBridgeSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "NoirQBudgetBridgeStyle.h"
#include "Editor.h"
#include "AssetRegistryModule.h"
#include "AssetData.h"
#include "AssetSelection.h"
#include "Paths.h"


IMPLEMENT_MODULE(FNoirQBudgetBridge, NoirQBudgetBridge)


void FNoirQBudgetBridge::StartupModule()
{
#if WITH_EDITOR
	FNoirQBudgetBridgeStyle::Initialize();
	FNoirQBudgetBridgeCommands::Register();
	BindCommands();
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	TSharedPtr<FExtender> MyExtender = MakeShareable(new FExtender);
	MyExtender->AddToolBarExtension("Content", EExtensionHook::After, NULL, FToolBarExtensionDelegate::CreateRaw(this, &FNoirQBudgetBridge::AddMyToolbarExtension));

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(MyExtender);

	RegisterCustomSettings();

	// don;'t currently use the in view menus
	/*FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors LevelEditorMenuExtenderDelegate;
	LevelEditorMenuExtenderDelegate = FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors::CreateStatic(&FNoirQBudgetBridge::OnExtendLevelEditorMenu);
	auto& MenuExtenders = LevelEditorModule.GetAllLevelViewportContextMenuExtenders();
	MenuExtenders.Add(LevelEditorMenuExtenderDelegate);*/
#endif
}

void FNoirQBudgetBridge::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FNoirQBudgetBridgeCommands::Unregister();
	FNoirQBudgetBridgeStyle::Shutdown();
	if (UObjectInitialized())
	{
		UnregisterCustomSettings();
	}
}

bool FNoirQBudgetBridge::SupportsDynamicReloading()
{
	return true;
}

void FNoirQBudgetBridge::ExtendMenu(class FMenuBuilder& MenuBuilder)
{
	FUIAction Action_Test(FExecuteAction::CreateStatic(&FNoirQBudgetBridge::ExtendTestButton));
	MenuBuilder.BeginSection("Test", LOCTEXT("Actor Menu", "Test"));
	{
		MenuBuilder.AddMenuEntry
		(
			LOCTEXT(STUBLOC, "Actor Button"),
			LOCTEXT(STUBLOCTOOLTIP, "Test Button on an actor"),
			FSlateIcon(),
			Action_Test,
			NAME_None,
			EUserInterfaceActionType::Button
		);
	}
	MenuBuilder.EndSection();
}

TSharedRef<FExtender> FNoirQBudgetBridge::OnExtendLevelEditorMenu(const TSharedRef<FUICommandList> CommandList, TArray<AActor*> SelectedActors)
{
	TSharedRef<FExtender> Extender(new FExtender());
	Extender->AddMenuExtension(
		"ActorSelectVisibilityLevels",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateStatic(&FNoirQBudgetBridge::ExtendMenu)
	);
	return Extender;
}

void FNoirQBudgetBridge::AddMyToolbarExtension(FToolBarBuilder& ToolbarBuilder)
{
	FSlateIcon MyIcon = FSlateIcon(FNoirQBudgetBridgeStyle::Get()->GetStyleSetName(), "NoirQBudgetBridgeStyle.Icon");
	ToolbarBuilder.BeginSection(TEXT(STUBLOC));
	{
		ToolbarBuilder.AddComboButton(
			FUIAction(),
			FOnGetContent::CreateRaw(this,&FNoirQBudgetBridge::GenerateToolbarMenuWidget),
			LOCTEXT(STUBLOC, NOIRQ_BUTTONNAME),
			LOCTEXT(STUBLOCTOOLTIP, STUBLOC),
			FSlateIcon(FNoirQBudgetBridgeStyle::Get()->GetStyleSetName(), "NoirQBudgetBridgeStyle.Icon"),
			false);
			
	}
	ToolbarBuilder.EndSection();

}

TSharedRef<SWidget> FNoirQBudgetBridge::GenerateToolbarMenuWidget()
{
	FMenuBuilder MenuBuilder(true, MyCommandBindings);

	MenuBuilder.BeginSection(STUBLOC, LOCTEXT("Stub", STUBLOC));
	{
		MenuBuilder.AddMenuEntry(FNoirQBudgetBridgeCommands::Get().LoadBridgeButton);
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}



void FNoirQBudgetBridge::BindCommands()
{
	MyCommandBindings = MakeShareable(new FUICommandList);
		
	FUICommandList& ActionList = *MyCommandBindings;
	const FNoirQBudgetBridgeCommands& Commands = FNoirQBudgetBridgeCommands::Get();

	/*MyCommandBindings->MapAction(
		Commands.TestButton,
		FExecuteAction::CreateRaw(this, &FNoirQBudgetBridge::TestButton),
		EUIActionRepeatMode::RepeatEnabled);*/

	MyCommandBindings->MapAction(
		Commands.LoadBridgeButton,
		FExecuteAction::CreateRaw(this, &FNoirQBudgetBridge::LoadLayoutFile),
		EUIActionRepeatMode::RepeatEnabled);
}


void FNoirQBudgetBridge::RegisterCustomSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", STUBLOC, LOCTEXT("RuntimeGeneralSettingsName", "Budget Bridge"), LOCTEXT("RuntimeGeneralSettingsDescription", "My config"), GetMutableDefault<UNoirQBudgetBridgeSettings>());

		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FNoirQBudgetBridge::HandleSaveSettings);
		}
	}
}

void FNoirQBudgetBridge::UnregisterCustomSettings()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", STUBLOC);
	}
}

bool FNoirQBudgetBridge::HandleSaveSettings()
{
	UNoirQBudgetBridgeSettings* Settings = GetMutableDefault<UNoirQBudgetBridgeSettings>();

	//Todo: Validation
	Settings->SaveConfig();

	return true;
}


// the bits that actuall do stuff:

void FNoirQBudgetBridge::TestButton()
{
	UE_LOG(LogTemp, Warning, TEXT("This button works!"));
}

void FNoirQBudgetBridge::ExtendTestButton()
{
	UE_LOG(LogTemp, Warning, TEXT("This static button works!"));
}

void FNoirQBudgetBridge::LoadLayoutFile()
{
	FString LayoutFilePath, LayoutFileName;
	UNoirQBudgetBridgeSettings* Settings = GetMutableDefault<UNoirQBudgetBridgeSettings>();

	LayoutFileName = Settings->LoadLayoutName;
	FString filename = "BridgeFile.json";
	if (LayoutFileName.IsEmpty())
	{
		LayoutFileName = filename;
	}
	
	FString projectContentDir = FPaths::ProjectDir();
	FString fullDir = projectContentDir + "/" + LayoutFileName;
	LayoutFilePath = fullDir;
	
	// https://stackoverflow.com/questions/52516942/how-to-parse-array-of-array-in-a-json-file-in-c-using-unreal
	FString jsonAsString;
	FFileHelper::LoadFileToString(jsonAsString, *LayoutFilePath);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(jsonAsString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		const TArray<TSharedPtr<FJsonValue>>* Items;
		if (JsonObject->TryGetArrayField(TEXT("items"), Items))
		{
			for (TSharedPtr<FJsonValue> ItemValue : *Items)
			{
				FMatrix ueMatrix;
				const TSharedPtr<FJsonObject> Item = ItemValue->AsObject();
				FString itemName;
				Item->TryGetStringField(TEXT("item"), itemName);

				bool bAlreadyExists = false;
				AActor* editedActor = NULL;
				// See if the actor is already in the level. If so, we can just set the matrix.
				ULevel* desiredLevel = GWorld->GetCurrentLevel();
				for (TActorIterator<AActor> ActorItr(desiredLevel->OwningWorld); ActorItr; ++ActorItr)
				{
					AActor* thisActor = *ActorItr;
					FString actorName;
					thisActor->GetName(actorName);
					FName fName = thisActor->GetFName();
					actorName = fName.ToString();

					FString humanReadableName = thisActor->GetHumanReadableName();

					FString actorLabel = thisActor->GetActorLabel();

					//if (thisActor->ActorHasTag(FName(*itemName)))
					{
						if (actorLabel.Compare(itemName) == 0)
						{
							bAlreadyExists = true;
							editedActor = thisActor;
							break;
						}
					}
				}


				const TArray<TSharedPtr<FJsonValue>>* RootMatrix;
				if (Item->TryGetArrayField("matrix", RootMatrix))
				{
					int rowId = 0;
					for (TSharedPtr<FJsonValue> RootMatrixValue : *RootMatrix)
					{
						//						const TArray<TSharedPtr<FJsonValue>>* CurrentRow;


						const TArray<TSharedPtr<FJsonValue>> asArray = RootMatrixValue->AsArray();
						for (int i = 0; i < asArray.Num(); i++)
						{
							float currentValue = -1.f;
							asArray[i]->TryGetNumber(currentValue);
							ueMatrix.M[rowId][i] = currentValue;
						}
						rowId++;

					}
				}


				FTransform newTransform(ueMatrix);
				if (bAlreadyExists)
				{
					editedActor->SetActorTransform(newTransform);
				}
				else
				{

					FString redirectValue = "";
					Item->TryGetStringField(TEXT("redirect"), redirectValue);
					FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
					FAssetData Asset = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*redirectValue));

					if (!redirectValue.IsEmpty())
					{
						editedActor = FActorFactoryAssetProxy::AddActorForAsset(Asset.GetAsset());
					}

					if (editedActor != NULL)
					{
						editedActor->SetActorTransform(newTransform);

						editedActor->SetActorLabel(itemName);
					}
				}


			}
		}
	}
}
