// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ForcePullPush.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIV_REVING_API UForcePullPush : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UForcePullPush();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	AActor* TargetActor;
	AActor* TraceTarget(float range, float radius, ETraceTypeQuery traceChannel, UCameraComponent* cameraComp);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void FindTarget(float range, float radius, ETraceTypeQuery traceChannel, UCameraComponent* cameraComp);
	UFUNCTION(BlueprintCallable)
		void PullTarget(float speed, float deltaTime);
	UFUNCTION(BlueprintCallable)
		void StopPulling();
	UFUNCTION(BlueprintCallable)
		void PushObject(float range, float radius, ETraceTypeQuery traceChannel, UCameraComponent* cameraComp, float pushForce);
};
