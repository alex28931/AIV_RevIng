// Fill out your copyright notice in the Description page of Project Settings.


#include "LightDetector.h"

// Sets default values for this component's properties
ULightDetector::ULightDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULightDetector::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void ULightDetector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float ULightDetector::ComputeCurrentBrightness()
{
	// Check if given UTextureRenderTarget2D* is valid.
	if (!RenderTexture)
	{
		return 0.f;
	}
	
	/*  Get what is currently streamed on given UTextureRenderTarget2D. Then save it in a FRenderTarget* var inside this class.
	This variable is used to cycle its pixels. Beware this is an heavy operation, 
	so make sure to not use it often or at least using a small UTextureRenderTarget2D.
	Check if RenderTarget is valid before using it. */

	RenderTarget = RenderTexture->GameThread_GetRenderTargetResource();
	if (!RenderTarget)
	{
		return 0.f;
	}

	// Reset class float variable. It is used to store the value of the highest brightness of all pixels for current call.
	MaxBrightness = -1.f;

	// Read pixels from FRenderTarget.
	RenderTarget->ReadPixels(pixels);

	// Cycle all pixels found.
	for (int i = 0; i < pixels.Num(); ++i)
	{
		// Store current pixel channels (Alpha is not necessary).
		PixelChannelR = pixels[i].R;
		PixelChannelG = pixels[i].G;
		PixelChannelB = pixels[i].B;

		// Compute current pixel brightness using a formula.
		// www.stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color
		float CurrentBrightness = ((0.299 * PixelChannelR) + (0.587 * PixelChannelG) + (0.114 * PixelChannelB));

		// If computed brightenss is greater to what MaxBrightness has stored, update it.
		if (CurrentBrightness >= MaxBrightness)
		{
			MaxBrightness = CurrentBrightness;
		}
	}

	// Return the max brightness (in the moment this method was called) among all pixels of given UTextureRenderTarget2D.
	return MaxBrightness;
}

