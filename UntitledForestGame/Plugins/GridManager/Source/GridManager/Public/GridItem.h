#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridCell.h"
#include "GridItem.generated.h"

class AActor;
class AGrid;

// Struct for multi-tile items
UCLASS(Blueprintable, BlueprintType)
class GRIDMANAGER_API UGridItem : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite) // , meta = (ExposeOnSpawn = true))
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D OriginCell; // Grid coordinates (e.g., bottom-left corner)

    UPROPERTY(EditAnywhere, BlueprintReadWrite) // , meta = (ExposeOnSpawn = true))
    int32 Width = 1; // Number of cells wide

    UPROPERTY(EditAnywhere, BlueprintReadWrite) // , meta = (ExposeOnSpawn = true))
    int32 Height = 1; // Number of cells tall

    UPROPERTY(EditAnywhere, BlueprintReadWrite) // , meta = (ExposeOnSpawn = true))
    float Rotation; // Rotation angle in degrees (0, 90, 180, 270)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> OccupiedCells; // Indices of occupied grid cells

    UPROPERTY(EditAnywhere, BlueprintReadWrite) // , meta = (ExposeOnSpawn = true))
    AActor* Actor = nullptr; // Reference to the spawned actor

    UPROPERTY(EditAnywhere, BlueprintReadWrite) // , meta = (ExposeOnSpawn = true))
    AGrid* Grid = nullptr; // Reference to the grid

    // Initialization
    UFUNCTION(BlueprintCallable)
    void Initialize(AGrid* NewGrid, const FString& Name, const FVector2D &Origin, int32 InWidth, int32 InHeight);

    // Spawn the actor
    UFUNCTION(BlueprintCallable)
    void SpawnActor(TSubclassOf<AActor> ActorClass);

    // Set the Actor pointer
    UFUNCTION(BlueprintCallable)
    void SetActor(AActor* NewActor);

    // Set the Grid pointer
    UFUNCTION(BlueprintCallable)
    void SetGrid(AGrid* NewGrid);

    // Get the actor's transform
    UFUNCTION(BlueprintCallable)
    FTransform GetActorTransform() const;

    // Get the actor's location
    UFUNCTION(BlueprintCallable)
    FVector GetActorLocation() const;

    // Get the actor's rotation
    UFUNCTION(BlueprintCallable)
    FRotator GetActorRotation() const;

    // Set the actor's location
    UFUNCTION(BlueprintCallable)
    void SetActorLocation(const FVector &NewLocation);

    // Set the actor's rotation
    UFUNCTION(BlueprintCallable)
    void SetActorRotation(const FRotator &NewRotation);

    // Update the Position / Rotation of the actor and the OccupiedCells array
    UFUNCTION(BlueprintCallable)
    void Update(const FVector2D &NewOrigin, float NewRotation);

protected:
    void UpdateOccupiedCells();
};
