// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"



UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(MoverActor))
	{
		return;
	}

	MoverComponentPtr = MoverActor->FindComponentByClass<UMover>();

	if (!IsValid(MoverComponentPtr))
	{
		return;
	}

	if (bIsPressurePlate)
	{
		OnComponentBeginOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UTriggerComponent::OnOverlapEnd);
	}

	//MoverComponentPtr->ShouldMove = true;

	//AActor* ActorPtr =  MoverComponentPtr->GetOwner();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		return;
	}

	ActorCount++;
	if (bIsTriggered) 
	{
		return;
	}
	
	TriggerComponent(true);
}

void UTriggerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("PressurePlateActivator"))
	{
		return;
	}

	ActorCount--;

	if (ActorCount > 0 || !bIsTriggered)
	{
		return;
	}

	TriggerComponent(false);
}

void UTriggerComponent::TriggerComponent(const bool& bTriggerValue)
{
	bIsTriggered = bTriggerValue;

	MoverComponentPtr->SetShouldMove(bTriggerValue);

	//MoverComponentPtr->ShouldMove = !MoverComponentPtr->ShouldMove;
}