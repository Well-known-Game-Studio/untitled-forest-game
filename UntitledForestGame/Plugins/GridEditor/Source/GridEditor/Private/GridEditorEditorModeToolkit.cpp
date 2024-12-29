// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridEditorEditorModeToolkit.h"
#include "GridEditorEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "GridEditorEditorModeToolkit"

FGridEditorEditorModeToolkit::FGridEditorEditorModeToolkit()
{
}

void FGridEditorEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FGridEditorEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FGridEditorEditorModeToolkit::GetToolkitFName() const
{
	return FName("GridEditorEditorMode");
}

FText FGridEditorEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "GridEditorEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
