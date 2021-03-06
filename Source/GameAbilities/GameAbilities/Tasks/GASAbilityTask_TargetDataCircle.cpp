// Fill out your copyright notice in the Description page of Project Settings.

#include "GameAbilities.h"
#include "../Targeting/GASAbilityTargetingObject.h"
#include "../GASAbilityBase.h"
#include "GASAbilityTask_TargetDataCircle.h"


UGASAbilityTask_TargetDataCircle* UGASAbilityTask_TargetDataCircle::TargetCircleDataTask(UObject* WorldContextObject, TSubclassOf<class UGASAbilityTargetingObject> Class, EGASConfirmType ConfirmTypeIn)
{
	auto MyObj = NewAbilityTask<UGASAbilityTask_TargetDataCircle>(WorldContextObject);

	if (MyObj)
	{
		MyObj->ConfirmType = ConfirmTypeIn;
	}
	return MyObj;
}

void UGASAbilityTask_TargetDataCircle::Activate()
{
	switch (ConfirmType)
	{
		case EGASConfirmType::Instant:
		{
			TargetObj2->GetTarget();
		}
		case EGASConfirmType::WaitForConfirm:
		{
			if (Ability.IsValid())
			{
				if (!Ability->OnConfirmDelegate.IsBoundToObject(this))
				{
					Ability->OnConfirmDelegate.AddUObject(this, &UGASAbilityTask_TargetDataCircle::OnConfirm);
				}
			}
		}
	}
	if (TargetObj2)
	{
		UE_LOG(GameAbilities, Log, TEXT("TArget object spawned"));
	}
	else
	{
		UE_LOG(GameAbilities, Log, TEXT("TArget object is null."));
	}
	//EndTask();
}

// ---------------------------------------------------------------------------------------

bool UGASAbilityTask_TargetDataCircle::BeginSpawningActor(UObject* WorldContextObject, TSubclassOf<UGASAbilityTargetingObject> Class, UGASAbilityTargetingObject*& SpawnedActor)
{
	SpawnedActor = Class.GetDefaultObject();//NewObject<UGASAbilityTargetingObject>(WorldContextObject, Class);

	if (SpawnedActor == nullptr)
	{
		//Failure.Broadcast(nullptr);
		return false;
	}
	else
	{
		TargetObj2 = SpawnedActor;
		SpawnedActor->AbilityOwner = Ability;
	}
	UE_LOG(GameAbilities, Log, TEXT("Begin Spawning Actor in GASAbilityTask_SpawnActor"));
	return true;
}

void UGASAbilityTask_TargetDataCircle::FinishSpawningActor(UObject* WorldContextObject, UGASAbilityTargetingObject* SpawnedActor)
{
	if (SpawnedActor)
	{
		//Success.Broadcast(SpawnedActor);
	}
	UE_LOG(GameAbilities, Log, TEXT("Finish Spawning Actor in GASAbilityTask_SpawnActor"));
	ReadyForActivation();
}

void UGASAbilityTask_TargetDataCircle::OnConfirm()
{
	TargetObj2->GetTarget();
	EndTask();
}