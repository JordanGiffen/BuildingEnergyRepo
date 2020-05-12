// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber"));
	
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("PrimarySelect", IE_Pressed, this, &UGrabber::PrimaryInteract);
		InputComponent->BindAction("PrimarySelect", IE_Released, this, &UGrabber::Release);
		InputComponent->BindAction("NextInventorySlot", IE_Pressed, this, &UGrabber::NextInventorySlot);
		InputComponent->BindAction("PreviousInventorySlot", IE_Pressed, this, &UGrabber::PreviousInventorySlot);
	}
}

void UGrabber::NextInventorySlot()
{
	CurrentInventorySlot++;
	UpdateInventorySlot();
}

void UGrabber::PreviousInventorySlot()
{
	CurrentInventorySlot--;
	UpdateInventorySlot();
}

void UGrabber::UpdateInventorySlot() // Keep this updated with all the possible ToolSelected options
{
	if (CurrentInventorySlot > InventorySize - 1)
	{
		CurrentInventorySlot = 0;
	}
	else if (CurrentInventorySlot < 0)
	{
		CurrentInventorySlot = InventorySize - 1;
	}
	
	switch (CurrentInventorySlot)
	{
	case 0:
		ToolSelected = EToolSelected::Grabber;
		break;
	
	case 1:
		ToolSelected = EToolSelected::Placer;
		break;
	
	default:
		UE_LOG(LogTemp, Warning, TEXT("This shouldn't happen"))
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("Current inventory selection: %i"), CurrentInventorySlot)
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the %s component attached but no PhysicsHandle component was found!"), *GetOwner()->GetName(), *GetName());
	}
}

void UGrabber::PrimaryInteract()
{
	switch (ToolSelected)
	{
	case EToolSelected::Grabber:
		PickUpObject();
		break;
	
	case EToolSelected::Placer:
		PlaceItem();
		break;
	
	default:
		UE_LOG(LogTemp, Warning, TEXT("This shouldn't happen"))
		break;
	}
}

void UGrabber::PickUpObject()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// Attach the HitResult component to the physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) {return;}
		PhysicsHandle->GrabComponentAtLocation
		(
		ComponentToGrab,
		NAME_None,
		GetPlayersReach()
		);
	}
}

void UGrabber::PlaceItem()
{
	UE_LOG(LogTemp, Warning, TEXT("Tried to place! :)"))
}

void UGrabber::Release()
{
	if (!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If the physics handle is attached
	if (!PhysicsHandle) {return;}
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
	
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;

	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	
	return Hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}