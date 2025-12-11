// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "iostream"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	FString OwnerName = Owner->GetActorNameOrLabel();
	
	StartLocation = Owner->GetActorLocation();

	// Oyun baþlarken TargetLocation StartLocation set olsun diye SetShouldMove burada çaðýrýldý.
	UMover::SetShouldMove(false);
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Karakter actor baktýðýnda actor haraket etmesi için tasarlanan metot MoveComponent metodu plate tarzý TriggerComponent baðlý bir actor ile kapýnýn açýlmasýný saðlayan bir metot
	//IsLookingAtDoor(DeltaTime);

	MoveComponent(DeltaTime);
}

void UMover::MoveComponent(const float& DeltaTime)
{
	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	const FVector CurrentLocaiton = Owner->GetActorLocation();

	if (CurrentLocaiton.Equals(TargetLocation, KINDA_SMALL_NUMBER))
	{
		return;
	}

	const float Speed = MoveOffSet.Length() / MoveTime;

	const FVector NewLocation = FMath::VInterpConstantTo(CurrentLocaiton, TargetLocation, DeltaTime, Speed);

	Owner->SetActorLocation(NewLocation);
}

void UMover::IsLookingAtDoor(const float& DeltaTime)
{
	AActor* Owner = GetOwner();

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	// Üstteki kodun farklý bir yöntemi
	// APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!IsValid(Owner) || !IsValid(PlayerPawn))
	{
		return;
	}

	const FVector OwnerLocation = Owner->GetActorLocation();

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();

	const FVector PlayerLookDirection = PlayerPawn->GetActorForwardVector();

	const auto DistanceSq = FVector::DistSquaredXY(OwnerLocation, PlayerLocation);

	const FVector ToObjectDirection = (OwnerLocation - PlayerLocation).GetSafeNormal();

	const auto LookDot = FVector::DotProduct(PlayerLookDirection, ToObjectDirection);
	// Deneme amacýyla yazýldýðý için statik deðer verildi.
	constexpr auto MaxDistance = 300.0f;

	const auto MaxDistanceSq = MaxDistance * MaxDistance;
	// Deneme amacýyla yazýldýðý için statik deðer verildi.
	constexpr auto LookingDotThreshold = 0.2f;
	
	const bool bInRange = DistanceSq <= MaxDistanceSq;
	const bool bIsLooking = LookDot >= LookingDotThreshold;
	ShouldMove = bInRange && bIsLooking;

	// Karakter actor baktýðýnda actor haraket etmesi için tasarlanan kod yapýsýnda TargetLocation burada hesaplanýyor
	TargetLocation = ShouldMove ? (StartLocation + MoveOffSet) : StartLocation;

	if (!OwnerLocation.Equals(TargetLocation, KINDA_SMALL_NUMBER))
	{
		MoveComponent(DeltaTime);
	}
}

bool UMover::GetShouldMove() const
{
	return ShouldMove;
}

void UMover::SetShouldMove(const bool& bIsTrigerred)
{
	ShouldMove = bIsTrigerred;

	TargetLocation = ShouldMove ? StartLocation + MoveOffSet : StartLocation;
}
