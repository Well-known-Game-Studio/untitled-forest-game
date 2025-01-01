#include "GridEditorWidget.h"
#include "Grid.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"

void SGridEditorWidget::Construct(const FArguments& InArgs)
{
    Grid = InArgs._Grid;

    ChildSlot
    [
        SNew(SVerticalBox)

        // Grid Initialization
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(STextBlock).Text(FText::FromString("Grid Dimensions"))
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SEditableTextBox)
            .Text(FText::FromString("10")) // Default value
            .OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType)
            {
                GridWidthInput = FCString::Atoi(*Text.ToString());
            })
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SEditableTextBox)
            .Text(FText::FromString("10")) // Default value
            .OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType)
            {
                GridHeightInput = FCString::Atoi(*Text.ToString());
            })
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SButton)
            .Text(FText::FromString("Initialize Grid"))
            .OnClicked(this, &SGridEditorWidget::OnInitializeGridClicked)
        ]

        // Cell Attributes
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(STextBlock).Text(FText::FromString("Cell Attributes"))
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SEditableTextBox)
            .Text(FText::FromString("Soil")) // Default value
            .OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType)
            {
                CellTypeInput = Text.ToString();
            })
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SEditableTextBox)
            .Text(FText::FromString("1.0")) // Default soil quality
            .OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType)
            {
                SoilQualityInput = FCString::Atof(*Text.ToString());
            })
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SEditableTextBox)
            .Text(FText::FromString("0.0")) // Default water level
            .OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType)
            {
                WaterLevelInput = FCString::Atof(*Text.ToString());
            })
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SButton)
            .Text(FText::FromString("Paint Cell"))
            .OnClicked(this, &SGridEditorWidget::OnPaintCellClicked)
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SButton)
            .Text(FText::FromString("Fill Grid"))
            .OnClicked(this, &SGridEditorWidget::OnFillGridClicked)
        ]
    ];
}

FReply SGridEditorWidget::OnInitializeGridClicked()
{
    if (Grid.IsValid())
    {
        Grid->GridWidth = GridWidthInput;
        Grid->GridHeight = GridHeightInput;
        Grid->InitializeGrid();
    }
    return FReply::Handled();
}

FReply SGridEditorWidget::OnPaintCellClicked()
{
    if (Grid.IsValid())
    {
        FGridCell* Cell = Grid->GetGridCell(0, 0); // Example: Modify cell at (0, 0)
        if (Cell)
        {
            Cell->CellType = GetGridCellTypeFromString(CellTypeInput);
            Cell->Attributes.SoilQuality = SoilQualityInput;
            Cell->Attributes.WaterLevel = WaterLevelInput;
        }
    }
    return FReply::Handled();
}

FReply SGridEditorWidget::OnFillGridClicked()
{
    if (Grid.IsValid())
    {
        for (int32 Y = 0; Y < Grid->GridHeight; ++Y)
        {
            for (int32 X = 0; X < Grid->GridWidth; ++X)
            {
                FGridCell* Cell = Grid->GetGridCell(X, Y);
                if (Cell)
                {
                    Cell->CellType = GetGridCellTypeFromString(CellTypeInput);
                    Cell->Attributes.SoilQuality = SoilQualityInput;
                    Cell->Attributes.WaterLevel = WaterLevelInput;
                }
            }
        }
    }
    return FReply::Handled();
}
