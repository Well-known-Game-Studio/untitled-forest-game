// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GridInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UGridItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *   Interface for Grid objects
 */
class GRIDMANAGER_API IGridItemInterface
{
    GENERATED_BODY()

public:

    // Implement this to be able to get the grid location of the object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grid")
    FVector2D GetGridLocation();

    // Implement this to be able to set the grid location of the object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grid")
    void SetGridLocation(FVector2D NewLocation);

    // Implement this to be able to get the grid size of the object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grid")
    FVector2D GetGridSize();

    // Implement this to be able to get the grid size of the object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grid")
    void SetGridSize(FVector2D NewSize);

    // Implement this to be able to get the grid size of the object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grid")
    void SetGridLocationAndSize(FVector2D NewLocation, FVector2D NewSize);

    // Implement this to be able to get the grid size of the object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grid")
    void GetGridLocationAndSize(FVector2D& OutLocation, FVector2D& OutSize);

    // Implement this to Get the rotation of the object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grid")
    float GetGridRotation();

    // Implement this to Set the rotation of the object
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grid")
    void SetGridRotation(float NewRotation);

}; // IGridInterface
