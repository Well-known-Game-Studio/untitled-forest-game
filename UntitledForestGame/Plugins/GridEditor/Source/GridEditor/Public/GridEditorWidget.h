#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class AGrid;

class SGridEditorWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SGridEditorWidget) {}
    SLATE_ARGUMENT(TWeakObjectPtr<AGrid>, Grid)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    // Reference to the Grid
    TWeakObjectPtr<AGrid> Grid;

    // Callbacks for UI actions
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
