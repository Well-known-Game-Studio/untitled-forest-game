#include "GridEditorWidget.h"
#include "Grid.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"

void SGridEditorWidget::Construct(const FArguments& InArgs)
{
    Grids = InArgs._Grids;

    // Populate the names for the dropdown
    for (TWeakObjectPtr<AGrid> Manager : Grids)
    {
        if (Manager.IsValid())
        {
            GridNames.Add(MakeShareable(new FString(Manager->GetName())));
        }
    }

    ChildSlot
    [
        SNew(SVerticalBox)

        // Dropdown to select Grid
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(STextBlock).Text(FText::FromString("Select Grid"))
        ]
        + SVerticalBox::Slot()
        .AutoHeight()
        .Padding(5)
        [
            SNew(SComboBox<TSharedPtr<FString>>)
            .OptionsSource(&GridNames)
            .OnSelectionChanged(this, &SGridEditorWidget::OnGridSelected)
            .OnGenerateWidget_Lambda([](TSharedPtr<FString> InOption)
            {
                return SNew(STextBlock).Text(FText::FromString(*InOption));
            })
            [
                SNew(STextBlock).Text(FText::FromString("Select Grid"))
            ]
        ]
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

void SGridEditorWidget::OnGridSelected(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo)
{
    // Find the selected Grid
    for (TWeakObjectPtr<AGrid> Manager : Grids)
    {
        if (Manager.IsValid() && Manager->GetName() == *NewValue)
        {
            SelectedGrid = Manager;
            break;
        }
    }
}

FReply SGridEditorWidget::OnInitializeGridClicked()
{
    if (SelectedGrid.IsValid())
    {
        SelectedGrid->GridWidth = GridWidthInput;
        SelectedGrid->GridHeight = GridHeightInput;
        SelectedGrid->InitializeGrid();
    }
    return FReply::Handled();
}

FReply SGridEditorWidget::OnPaintCellClicked()
{
    if (SelectedGrid.IsValid())
    {
        FGridCell* Cell = SelectedGrid->GetGridCell(0, 0); // Example: Modify cell at (0, 0)
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
    if (SelectedGrid.IsValid())
    {
        for (int32 Y = 0; Y < SelectedGrid->GridHeight; ++Y)
        {
            for (int32 X = 0; X < SelectedGrid->GridWidth; ++X)
            {
                FGridCell* Cell = SelectedGrid->GetGridCell(X, Y);
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
