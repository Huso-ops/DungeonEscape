// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */

// Forward Declartation

class UPrimitiveComponent;
class USphereComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DUNGEONESCAPE_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
	
	public:
		// Sets default values for this component's properties
		UTriggerComponent();
	protected:
		// Called when the game starts
		virtual void BeginPlay() override;

	public:
		// Called every frame
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		void TriggerComponent(const bool& bTriggerValue);

		UPROPERTY(EditAnywhere)
		TObjectPtr<AActor> MoverActor;

		UPROPERTY()
		TObjectPtr<UMover> MoverComponentPtr;

		UPROPERTY(EditAnywhere)
		bool bIsPressurePlate = false;

		UPROPERTY(VisibleAnywhere)
		bool bIsTriggered = false;

		UPROPERTY(VisibleAnywhere)
		int32 ActorCount;

		UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		/** called when something leaves the sphere component */
		UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
