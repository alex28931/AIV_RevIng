// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Containers/Deque.h"
#include "TimeRewinder.generated.h"

struct FSnapShot
{
	double TimeSTamp;
	FVector Position;
	FRotator Rotation;

	FSnapShot(double InTimeStamp, FVector InPosition, FRotator InRotation) : TimeSTamp(InTimeStamp), Position(InPosition), Rotation(InRotation)
	{}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIV_REVING_API UTimeRewinder : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeRewinder();
	UFUNCTION(BlueprintCallable)
	void RegisterSnapshot();
	UFUNCTION(BlueprintCallable)
	void Rewind(float DeltaTime, float DeltaTimeSnap);
	UFUNCTION(BlueprintCallable)
	void BeginRewind();
	UFUNCTION(BlueprintCallable)
	bool CanRewind();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxSnaphots;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RewindSpeed;
	FVector StartRewindPosition;
	FRotator StartRewindRotation;
	double StartRewindTimeStamp;
	float RewindAccumulator;

private:
	TDeque<FSnapShot> SnapShots;

	void BeginRewindPosition();
	void RewindPosition();
	void BeginRewindRotation();
	void RewindRotation();


};
