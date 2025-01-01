#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridCell.h"
#include "GridItem.generated.h"

class AActor;
class AGrid;

// Struct for multi-tile items
UCLASS(Blueprintable, BlueprintType)
class UGridItem : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D OriginCell; // Grid coordinates (e.g., bottom-left corner)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Width = 1; // Number of cells wide

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Height = 1; // Number of cells tall

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Rotation; // Rotation angle in degrees (0, 90, 180, 270)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> OccupiedCells; // Indices of occupied grid cells

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* Actor = nullptr; // Reference to the spawned actor

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AGrid* Grid = nullptr; // Reference to the grid

    // Initialization
    void Initialize(AGrid* NewGrid, const FString& Name, const FVector2D &Origin, int32 InWidth, int32 InHeight);

    // Update the Position / Rotation of the actor and the OccupiedCells array
    void Update(const FVector2D &NewOrigin, float NewRotation);

protected:
    void UpdateOccupiedCells();
};
