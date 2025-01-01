#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class AGrid;

class SGridEditorWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SGridEditorWidget) {}
    SLATE_ARGUMENT(TArray<TWeakObjectPtr<AGrid>>, Grids)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    // List of Grid actors in the level
    TArray<TWeakObjectPtr<AGrid>> Grids;

    // Currently selected Grid
    TWeakObjectPtr<AGrid> SelectedGrid;

    // Callbacks
    void OnGridSelected(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo);

    // UI Components
    TArray<TSharedPtr<FString>> GridNames;
    TSharedPtr<SComboBox<TSharedPtr<FString>>> GridDropdown;

    // Other callbacks for UI actions
    FReply OnInitializeGridClicked();
    FReply OnPaintCellClicked();
    FReply OnFillGridClicked();

    // Input values for grid cell attributes
    FString CellTypeInput;
    float SoilQualityInput;
    float WaterLevelInput;

    // Input values for grid dimensions
    int32 GridWidthInput;
    int32 GridHeightInput;
};
