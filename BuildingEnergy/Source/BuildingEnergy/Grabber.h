// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UENUM()
enum class EToolSelected : uint8
{
	Grabber,
	Placer
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGENERGY_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere)
	float Reach = 300.f;

	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	void PrimaryInteract();
	void PlaceItem();

	void NextInventorySlot();
	void PreviousInventorySlot();
	int32 CurrentInventorySlot = 0;
	void UpdateInventorySlot();
	int32 InventorySize = 2; // Does NOT include 0 (e.g. InventorySize = 2 means 2 slots not 3)
	
	void Release();
	void FindPhysicsHandle();
	void SetupInputComponent();

	// Returns the first actor within rach with physics body
	FHitResult GetFirstPhysicsBodyInReach() const;

	// Return the line trace end location
	FVector GetPlayersReach() const;

	// Get players position in world
	FVector GetPlayersWorldPos() const;

	void PickUpObject();

	EToolSelected ToolSelected = EToolSelected::Grabber;
};
