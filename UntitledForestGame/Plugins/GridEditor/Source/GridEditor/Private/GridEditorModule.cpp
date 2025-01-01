// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridEditorModule.h"
#include "GridEditorEditorModeCommands.h"

#include "Grid.h"
#include "GridEditorWidget.h"
// #include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "ToolMenus.h"
#include "EngineUtils.h"

#define LOCTEXT_NAMESPACE "GridEditorModule"

void FGridEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FGridEditorEditorModeCommands::Register();

    // Register a new tab spawner
    FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
        FName("GridEditorTab"), // FName instead of a raw string
        FOnSpawnTab::CreateRaw(this, &FGridEditorModule::OnSpawnGridEditorTab)
    )
    .SetDisplayName(LOCTEXT("GridEditorTabTitle", "Grid Editor"))
    .SetMenuType(ETabSpawnerMenuType::Hidden);

    // Add toolbar button
    UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateLambda([]()
    {
        UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");

        FToolMenuSection& Section = Menu->FindOrAddSection("GridEditor");
        Section.AddEntry(FToolMenuEntry::InitToolBarButton(
            "OpenGridEditor",
            FUIAction(FExecuteAction::CreateLambda([]()
            {
                FGlobalTabmanager::Get()->TryInvokeTab(FName("GridEditorTab")); // Correctly use FName
            })),
            LOCTEXT("OpenGridEditor", "Grid Editor"),
            LOCTEXT("OpenGridEditorTooltip", "Open the Grid Editor"),
            FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.GameSettings")
        ));
    }));
}

void FGridEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FGridEditorEditorModeCommands::Unregister();

    // Unregister the tab spawner
    FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("GridEditorTab");
    UToolMenus::UnregisterOwner(this);
}


TSharedRef<SDockTab> FGridEditorModule::OnSpawnGridEditorTab(const FSpawnTabArgs& SpawnTabArgs)
{
    TArray<TWeakObjectPtr<AGrid>> Grids;

    // Find all Grid actors in the level
    for (TActorIterator<AGrid> It(GEditor->GetEditorWorldContext().World()); It; ++It)
    {
        Grids.Add(*It);
    }

    return SNew(SDockTab)
        .TabRole(ETabRole::NomadTab)
        [
            SNew(SGridEditorWidget)
            .Grids(Grids)
        ];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGridEditorModule, GridEditor)
