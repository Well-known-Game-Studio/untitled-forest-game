#pragma once

#include "CoreMinimal.h"
#include "GridCell.generated.h"

class UGridItem;

// Enum for grid cell types
UENUM(BlueprintType)
enum class EGridCellType : uint8
{
    Empty,
    Ground,
    Unusable,
};

GRIDMANAGER_API EGridCellType GetGridCellTypeFromString(const FString& CellTypeString);

// Enum for ground types
UENUM(BlueprintType)
enum class EGroundType : uint8
{
  Empty,
    Grass,
    Dirt,
    Sand,
    Stone,
    Water,
    Snow,
    Ice,
    Mud,
    Swamp,
    Count
};

GRIDMANAGER_API EGroundType GetGroundTypeFromString(const FString& GroundTypeString);

// Struct for grid cell atributes
USTRUCT(BlueprintType)
struct FGridCellAttributes
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGroundType GroundType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SoilQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WaterLevel;
};

// Struct for grid cell
USTRUCT(BlueprintType)
struct FGridCell
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D GridPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector WorldPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGridCellType CellType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGridCellAttributes Attributes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOccupied;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UGridItem* OccupyingItem;
};
