// Copyright Epic Games, Inc. All Rights Reserved.

#include "GridEditorModule.h"
#include "GridEditorEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "GridEditorModule"

void FGridEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FGridEditorEditorModeCommands::Register();
}

void FGridEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FGridEditorEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGridEditorModule, GridEditorEditorMode)