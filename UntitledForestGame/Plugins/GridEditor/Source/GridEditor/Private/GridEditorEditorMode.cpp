// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridEditorEditorMode.h"
#include "GridEditorEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "GridEditorEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/GridEditorSimpleTool.h"
#include "Tools/GridEditorInteractiveTool.h"

// step 2: register a ToolBuilder in FGridEditorEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "GridEditorEditorMode"

const FEditorModeID UGridEditorEditorMode::EM_GridEditorEditorModeId = TEXT("EM_GridEditorEditorMode");

FString UGridEditorEditorMode::SimpleToolName = TEXT("GridEditor_ActorInfoTool");
FString UGridEditorEditorMode::InteractiveToolName = TEXT("GridEditor_MeasureDistanceTool");


UGridEditorEditorMode::UGridEditorEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UGridEditorEditorMode::EM_GridEditorEditorModeId,
		LOCTEXT("ModeName", "GridEditor"),
		FSlateIcon(),
		true);
}


UGridEditorEditorMode::~UGridEditorEditorMode()
{
}


void UGridEditorEditorMode::ActorSelectionChangeNotify()
{
}

void UGridEditorEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FGridEditorEditorModeCommands& SampleToolCommands = FGridEditorEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UGridEditorSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UGridEditorInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UGridEditorEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FGridEditorEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UGridEditorEditorMode::GetModeCommands() const
{
	return FGridEditorEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
