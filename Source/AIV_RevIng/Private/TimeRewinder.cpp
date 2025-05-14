// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeRewinder.h"

// Sets default values for this component's properties
UTimeRewinder::UTimeRewinder()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxSnaphots = 50;
	RewindSpeed = 3.0f;
}

void UTimeRewinder::RegisterSnapshot()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	FVector CurrentPosition = Owner->GetActorLocation();
	FRotator CurrentRotation = Owner->GetActorRotation();
	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		return;
	}
	double TimeStamp = CurrentWorld->GetTimeSeconds();
	SnapShots.EmplaceLast(TimeStamp, CurrentPosition, CurrentRotation);
	if (SnapShots.Num() >= MaxSnaphots) 
	{
		SnapShots.PopFirst();
	}
	UE_LOG(LogTemp, Warning, TEXT("Time: %f NumSnapShot: %d"), TimeStamp, SnapShots.Num());
	DrawDebugSphere(CurrentWorld, SnapShots.Last().Position, 5.0f, 32, FColor::Red, false, 2.0f);
}

void UTimeRewinder::Rewind(float DeltaTime, float DeltaTimeSnap)
{
	RewindAccumulator += RewindSpeed/ DeltaTimeSnap * DeltaTime;
	RewindPosition();
	RewindRotation();
	if (RewindAccumulator >= 1) 
	{
		SnapShots.PopLast();
		RewindAccumulator = 0.0f;
	}
}

void UTimeRewinder::BeginRewind()
{
	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		return;
	}
	StartRewindTimeStamp= CurrentWorld->GetTimeSeconds();
	BeginRewindPosition();
	BeginRewindRotation();
}

bool UTimeRewinder::CanRewind()
{
	return !SnapShots.IsEmpty();
}


// Called when the game starts
void UTimeRewinder::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTimeRewinder::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTimeRewinder::BeginRewindPosition()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	StartRewindPosition = Owner->GetActorLocation();
}

void UTimeRewinder::RewindPosition()
{
	FVector PrevPosition = SnapShots.Last().Position;
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	Owner->SetActorLocation(FMath::Lerp(StartRewindPosition, PrevPosition, RewindAccumulator));
	if (RewindAccumulator >= 1)
	{
		Owner->SetActorLocation(PrevPosition);
		StartRewindPosition = PrevPosition;
	}
}

void UTimeRewinder::BeginRewindRotation()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	StartRewindRotation = Owner->GetActorRotation();
}

void UTimeRewinder::RewindRotation()
{
	FRotator PrevRotation = SnapShots.Last().Rotation;
	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		return;
	}
	APlayerController* PlayerController = CurrentWorld->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}
	FRotator Rotator = FQuat::Slerp(StartRewindRotation.Quaternion(), PrevRotation.Quaternion(), RewindAccumulator).Rotator();
	PlayerController->SetControlRotation(Rotator);
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	Owner->SetActorRotation(Rotator);
	if (RewindAccumulator >= 1)
	{
		PlayerController->SetControlRotation(PrevRotation);
		Owner->SetActorRotation(PrevRotation);
		StartRewindRotation = PrevRotation;
	}
}

