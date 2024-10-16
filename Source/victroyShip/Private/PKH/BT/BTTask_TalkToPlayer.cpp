// Fill out your copyright notice in the Description page of Project Settings.


#include "PKH/BT/BTTask_TalkToPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PKH/BT/BTNPCKey.h"
#include "PKH/Component/TalkComponent.h"
#include "PKH/NPC/NPCBase.h"

UBTTask_TalkToPlayer::UBTTask_TalkToPlayer()
{
	NodeName = TEXT("Talk2Player");
}

EBTNodeResult::Type UBTTask_TalkToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult =  Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(nullptr == OwnerPawn)
	{
		return EBTNodeResult::Failed;
	}

	ANPCBase* NPC = Cast <ANPCBase>(OwnerPawn);
	if(nullptr == NPC)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if(nullptr == BBComp)
	{
		return EBTNodeResult::Failed;
	}

	AActor* Player = Cast<AActor>(BBComp->GetValueAsObject(KEY_PLAYER));
	if(nullptr == Player)
	{
		return EBTNodeResult::Failed;
	}

	UTalkComponent* TalkComp = Cast<UTalkComponent>(Player->GetComponentByClass(UTalkComponent::StaticClass()));
	if(nullptr == TalkComp || TalkComp->IsInConversation())
	{
		return EBTNodeResult::Failed;
	}

	NPC->GreetingToPlayer();
	return EBTNodeResult::Succeeded;
}
