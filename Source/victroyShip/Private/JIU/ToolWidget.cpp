// Fill out your copyright notice in the Description page of Project Settings.


#include "JIU/ToolWidget.h"

#include "Components/Button.h"
#include "JIU/FarmerComponent.h"

void UToolWidget::NativeConstruct()
{
	Super::NativeConstruct();

    button_0->OnClicked.AddDynamic(this, &UToolWidget::Onclick0);
    button_1->OnClicked.AddDynamic(this, &UToolWidget::Onclick1);
    button_2->OnClicked.AddDynamic(this, &UToolWidget::Onclick2);
    button_3->OnClicked.AddDynamic(this, &UToolWidget::Onclick3);
    button_4->OnClicked.AddDynamic(this, &UToolWidget::Onclick4);
    button_5->OnClicked.AddDynamic(this, &UToolWidget::Onclick5);

}

void UToolWidget::SetToolWidget(UFarmerComponent* fc)
{
    FarmerComponent = fc;
}

void UToolWidget::ClickButton(int index)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("equip %d"), index));

    if (FarmerComponent)
    {
        FarmerComponent->SwitchTool(index);
    }
}

void UToolWidget::Onclick0()
{
    ClickButton(0);
}

void UToolWidget::Onclick1()
{
    ClickButton(1);
}

void UToolWidget::Onclick2()
{
    ClickButton(2);
}

void UToolWidget::Onclick3()
{
    ClickButton(3);
}

void UToolWidget::Onclick4()
{
    ClickButton(4);
}

void UToolWidget::Onclick5()
{
    ClickButton(5);
}
