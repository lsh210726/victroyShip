// Fill out your copyright notice in the Description page of Project Settings.


#include "JIU/PlantActor.h"

#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/Engine.h"
#include "JIU/GroundActor.h"

// Sets default values
APlantActor::APlantActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent); // Set Component on Root Component

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent); // Attach to Root Component
}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

void APlantActor::Destroyed()
{
	Super::Destroyed();

	if (Ground)
	{
		if (Ground->Plant)
		{
			Ground->Plant = nullptr;
		}
	}
}

void APlantActor::SetPlant(int id, AGroundActor* _ground)
{
	level = 0;
	this->Ground = _ground;

	switch (id)
	{
	case 0:
		MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *GrapePath0));
		PlantType = EPlantType::Grape;
		level = 4;
		break;
	case 1:
		PlantType = EPlantType::Sunflower;
		level = 3;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Mesh not set."));
	}
}

void APlantActor::GrowPlant()
{
	// if (Ground->WaterFigure < 20.f || Ground->FertilizerFigure < 20.f)	return;

	switch (PlantType)
	{
	case EPlantType::Grape:
		{
			if (level == 4)
			{
				level = 3;
				StartScaling();
				NewMesh = LoadObject<UStaticMesh>(nullptr, *GrapePath1);
				// MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *GrapePath1));
			}
			else if (level == 3)
			{
				level = 2;
				StartScaling();
				NewMesh = LoadObject<UStaticMesh>(nullptr, *GrapePath2);
				// MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *GrapePath2));
			}
			else if (level == 2)
			{
				level = 1;
				StartScaling();
				NewMesh = LoadObject<UStaticMesh>(nullptr, *GrapePath3);
				// MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *GrapePath3));
			}
			else if (level == 1)
			{
				level = 0;
				StartScaling();
				NewMesh = LoadObject<UStaticMesh>(nullptr, *GrapePath4);
				// MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *GrapePath4));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Havest Grape");
			}
			break;
		}
	case EPlantType::Sunflower:
		{
			if (level == 3)
			{
				level = 2;
				MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *SunflowerPath1));
			}
			else if (level == 2)
			{
				level = 1;
				MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *SunflowerPath2));
			}
			else if (level == 1)
			{
				level = 0;
				MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *SunflowerPath3));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Havest Sunflower");
			}
			break;
		}
	default:
		UE_LOG(LogTemp, Warning, TEXT("Plant type error."));
	}
}

void APlantActor::StartScaling()
{
	MeshComponent->SetWorldScale3D(InitialScale);

	if (FloatCurve)
	{
		SetupTimeline();
		MyTimeline.PlayFromStart();
	}
}

void APlantActor::HandleProgress(float Value)
{
	// 일시적인 스케일 변화 설정
	FVector TempMaxScale = (MaxScale == InitialScale) ? InitialScale + FVector(0.3f, 0.3f, 0.3f) : MaxScale;

	FVector NewScale = FMath::Lerp(InitialScale, TempMaxScale, Value);

	if (Value >= 0.85f && isChanged)
	{
		if (NewMesh)
		{
			MeshComponent->SetStaticMesh(NewMesh);
			isChanged = false;
		}
	}

	MeshComponent->SetWorldScale3D(NewScale);
}

void APlantActor::OnTimelineFinished()
{
	NewMesh = nullptr;
	isChanged = true;
}

void APlantActor::SetupTimeline()
{
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("HandleProgress"));

	FOnTimelineEvent TimelineFinished;
	TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
	MyTimeline.SetTimelineFinishedFunc(TimelineFinished);

	MyTimeline.AddInterpFloat(FloatCurve, ProgressFunction);
	MyTimeline.SetLooping(false);
	MyTimeline.SetTimelineLengthMode(TL_TimelineLength);
	MyTimeline.SetTimelineLength(0.5f);
}

void APlantActor::HavestPlant()
{
	if (level == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Havest Success");

		switch (PlantType)
		{
		case EPlantType::Grape:
		{
			level = 1;
			MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, *GrapePath3));
		}
		case EPlantType::Sunflower:
		{
			Destroyed();
		}
		default:
			UE_LOG(LogTemp, Warning, TEXT("Plant type error."));
		}
	}
}
