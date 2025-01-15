// Copyright Epic Games, Inc. All Rights Reserved.

#include "week2PickUpComponent.h"
#include "week2GameMode.h"
#include <Kismet/GameplayStatics.h>

Uweek2PickUpComponent::Uweek2PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void Uweek2PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &Uweek2PickUpComponent::OnSphereBeginOverlap);
}

void Uweek2PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	Aweek2Character* Character = Cast<Aweek2Character>(OtherActor);

	if(Character != nullptr)
	{

		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		Aweek2GameMode* GameMode = Cast<Aweek2GameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			// Call the StartGame function
			GameMode->HandlePickupEvent();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode is not of type AMyGameMode!"));
		}

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
