// Fill out your copyright notice in the Description page of Project Settings.


#include "ForcePullPush.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UForcePullPush::UForcePullPush()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UForcePullPush::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

AActor* UForcePullPush::TraceTarget(float range, float radius, ETraceTypeQuery traceChannel, UCameraComponent* cameraComp)
{
	if (UWorld* world = GetOwner()->GetWorld())
	{
		if (AActor* owner = GetOwner())
		{
			TArray<AActor*> actorsToIgnore;
			actorsToIgnore.Add(owner);
			FHitResult hitResult;
			UKismetSystemLibrary::SphereTraceSingle(world,
				owner->GetActorLocation(),
				owner->GetActorLocation() + cameraComp->GetForwardVector() * range,
				radius,
				traceChannel,
				false,
				actorsToIgnore,
				EDrawDebugTrace::ForDuration,
				hitResult,
				true
			);
			return hitResult.GetActor();
		}
	}
	return nullptr;
}


// Called every frame
void UForcePullPush::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	

	// ...
}

void UForcePullPush::FindTarget(float range, float radius, ETraceTypeQuery traceChannel, UCameraComponent* cameraComp)
{
	TargetActor = TraceTarget(range, radius, traceChannel, cameraComp);
	if (TargetActor)
	{
		TargetActor->GetComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(false);
	}
}

void UForcePullPush::PullTarget(float speed, float deltaTime)
{
	if (TargetActor == nullptr) return;

	if (FVector::Dist(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation()) > 0.1)
	{
		TargetActor->SetActorLocation(FMath::Lerp(TargetActor->GetActorLocation(), GetOwner()->GetActorLocation(), speed * deltaTime));
	}
	else
	{
		TargetActor->SetActorLocation(GetOwner()->GetActorLocation());
	}
}

void UForcePullPush::StopPulling()
{
	if (TargetActor)
		TargetActor->GetComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(true);
	TargetActor = nullptr;
}

void UForcePullPush::PushObject(float range, float radius, ETraceTypeQuery traceChannel, UCameraComponent* cameraComp, float pushForce)
{
	AActor* actorToPush = TraceTarget(range, radius, traceChannel, cameraComp);
	if (actorToPush)
	{
		if (actorToPush == TargetActor)
		{
			StopPulling();
		}
		actorToPush->GetComponentByClass<UStaticMeshComponent>()->AddImpulseAtLocation(cameraComp->GetForwardVector() * pushForce, actorToPush->GetActorLocation());
	}
}

