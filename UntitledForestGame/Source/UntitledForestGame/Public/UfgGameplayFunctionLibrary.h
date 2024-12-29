// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridCell.h"
#include "UfgGameplayFunctionLibrary.generated.h"

class AGrid;

UCLASS()
class UNTITLEDFORESTGAME_API UUfgGameplayFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category = "Grid")
    static FGridCellAttributes RandomizeGridCellAttributes(EGroundType GroundType);

    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetGridCellIndex(int32 X, int32 Y, const AGrid* Grid);

    UFUNCTION(BlueprintCallable, Category = "Grid")
    static int32 GetGridCellIndexForGridWidth(int32 X, int32 Y, int32 GridWidth);

    UFUNCTION(BlueprintCallable, Category = "Debug")
    static void DrawHitPointAndBounds(AActor* HitActor, const FHitResult& Hit);

    static FString GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero=true);
    static FText GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero=true);

    static FString GetIntAsString(int TheNumber);
    static FText GetIntAsText(int TheNumber);

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    static void GetAllCppSubclasses(UClass* BaseClass, TArray<UClass*>& ClassArray);

    UFUNCTION(BlueprintCallable, Category = "Gameplay")
    static void GetAllBlueprintSubclasses(UClass* BaseClass, TArray<UClass*>& ClassArray);

};
