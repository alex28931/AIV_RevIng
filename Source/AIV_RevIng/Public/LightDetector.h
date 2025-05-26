// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "UnrealClient.h"
#include "LightDetector.generated.h"

/* 
	This ActorComponent is used to get, using ComputeCurrentBrightness(),
	the highest brightness among all pixels inside a given RenderTexture2D exposed as UPROPERTY.
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIV_REVING_API ULightDetector : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	float MaxBrightness;

	TArray<FColor> pixels;
	float PixelChannelR{ 0 };
	float PixelChannelG{ 0 };
	float PixelChannelB{ 0 };
	FRenderTarget* RenderTarget;

public:	
	ULightDetector();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float ComputeCurrentBrightness();

	UPROPERTY(EditAnywhere)
	UTextureRenderTarget2D* RenderTexture;
		
};
