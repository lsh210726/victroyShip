// Fill out your copyright notice in the Description page of Project Settings.


#include "JIU/FarmerComponent.h"

#include "Blueprint/UserWidget.h"
#include "JIU/GroundActor.h"
#include "JIU/PlantActor.h"
#include "JIU/PlantInfoWidget.h"
#include "JIU/PlantWidget.h"
#include "JIU/ToolWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFarmerComponent::UFarmerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFarmerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	PlantWidget = CreateWidget<UPlantWidget>(GetWorld(), plantFactory);
	PlantWidget->AddToViewport();
	PlantWidget->SetVisibility(ESlateVisibility::Hidden);

	PlantInfoWidget = CreateWidget<UPlantInfoWidget>(GetWorld(), plantInfoFactory);
	PlantInfoWidget->AddToViewport();
	PlantInfoWidget->SetVisibility(ESlateVisibility::Hidden);

	ToolWidget = CreateWidget<UToolWidget>(GetWorld(), toolFactory);
	ToolWidget->AddToViewport();
	ToolWidget->SetVisibility(ESlateVisibility::Hidden);
	ToolWidget->SetToolWidget(this);

	// GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}


// Called every frame
void UFarmerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFarmerComponent::OpenPlantWidget()
{
	if (PlantWidget && Ground)
	{
		if (PlantWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			PlantWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			if (Ground->GroundState != EGroundState::Default)
			{

				PlantWidget->SetVisibility(ESlateVisibility::Visible);
				PlantWidget->SetPlantWidget(Ground);
			}
		}
	}
}

void UFarmerComponent::OpenInfoWidget()
{
	if (Ground)
	{
		if (PlantInfoWidget && Ground->Plant)
		{
			if (PlantInfoWidget->GetVisibility() == ESlateVisibility::Hidden)
			{
				PlantInfoWidget->SetVisibility(ESlateVisibility::Visible);
				PlantInfoWidget->SetPlantInfo(Ground->Plant);
				// PlantInfoWidget->UpdatePlantState();

				PlayerController->SetInputMode(FInputModeGameAndUI());
				PlayerController->bShowMouseCursor = true;
			}
		}
	}
}

void UFarmerComponent::CloseInfoWidget()
{
	if (PlantInfoWidget)
	{
		if (PlantInfoWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			// Ground->MoveCamera(false);
			PlantInfoWidget->SetVisibility(ESlateVisibility::Hidden);
			PlantInfoWidget->ground = nullptr;

			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->bShowMouseCursor = false;
		}
	}
}

void UFarmerComponent::OpenToolWidget()
{
	if (ToolWidget)
	{
		if (ToolWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			ToolWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			ToolWidget->SetVisibility(ESlateVisibility::Visible);

			PlayerController->SetInputMode(FInputModeGameAndUI());
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void UFarmerComponent::FarmingInteraction()
{
	if (!Ground)	return;

	switch (CurrentTool)
	{
	case 0:
		if (Ground->Plant)
		{
			Ground->Plant->NegativeReaction();
		}
		else
		{
			OpenPlantWidget();
		}
		break;
	case 1:
		Ground->ProwGround();
		break;
	case 2:
		Ground->RemoveWeed();
		break;
	case 3:
		Ground->WaterPlant();
		break;
	case 4:
		Ground->FertilizePlant();
		break;
	case 5:
		Ground->RemovePlant();
		break;
	default: ;
	}
}

void UFarmerComponent::SwitchTool(int index)
{
	CurrentTool = index;
}

int UFarmerComponent::HarvestPlant()
{
	int id = -1;

	if (Ground)
	{
		if (Ground->Plant)
		{
			id = Ground->Plant->plantID;
			HarvestAmount = Ground->Plant->PlantInfo.HarvestAmount;

			Ground->Plant->HavestPlant();

			PlantInfoWidget->SetPlantInfo(Ground->Plant);
		}
	}

	return id;
}

void UFarmerComponent::RemovePlant()
{
	if (Ground)
	{
		if (Ground->Plant)
		{
			Ground->RemovePlant();

			PlantInfoWidget->SetVisibility(ESlateVisibility::Hidden);
			PlantInfoWidget->ground = nullptr;
		}
	}
}
