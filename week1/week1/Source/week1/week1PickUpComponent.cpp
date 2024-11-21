// Copyright Epic Games, Inc. All Rights Reserved.

#include "week1PickUpComponent.h"

Uweek1PickUpComponent::Uweek1PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void Uweek1PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &Uweek1PickUpComponent::OnSphereBeginOverlap);
}

void Uweek1PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	Aweek1Character* Character = Cast<Aweek1Character>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
