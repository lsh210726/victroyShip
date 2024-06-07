// Fill out your copyright notice in the Description page of Project Settings.


#include "PKH/NPC/NPC_Artist.h"

#include "PKH/Animation/NPCAnimInstance.h"
#include "PKH/NPC/NPCController.h"

#define HOUR_GO_HILL 11
#define HOUR_GO_PARK 14
#define HOUR_BACK_HOME 17

ANPC_Artist::ANPC_Artist()
{
	NPCType = ENPCType::Artist;

	HomeLoc = FVector(-6606, -795, -148);
	HillLoc = FVector(-2326, -485, 628);
	ParkLoc = FVector(853, 2724, 628);

	WorkRotation = FRotator(0, 270, 0);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage_WorkRef(TEXT("/Script/Engine.AnimMontage'/Game/PKH/Anim/AM_ArtistWork.AM_ArtistWork'"));
	if (Montage_WorkRef.Object)
	{
		Montage_Work = Montage_WorkRef.Object;
	}
}

void ANPC_Artist::BeginPlay()
{
	Super::BeginPlay();


}

void ANPC_Artist::DoJob()
{
	Super::DoJob();

	
}

void ANPC_Artist::OnHourUpdated(int32 NewHour)
{
	if(NewHour == HOUR_GO_HILL)
	{
		NPCController->MoveToTargetLoc(HillLoc);
		return;
	}

	if (NewHour == HOUR_GO_PARK)
	{
		NPCController->MoveToTargetLoc(ParkLoc);
		NPCController->SetIsWorking(true);
		return;
	}

	if(NewHour == HOUR_BACK_HOME)
	{
		NPCController->MoveToHome();
		NPCController->SetIsWorking(false);
		AnimInstance->StopSpecificMontage(Montage_Work);
	}
}
