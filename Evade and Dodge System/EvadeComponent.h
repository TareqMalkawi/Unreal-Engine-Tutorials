// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EvadeComponent.generated.h"

UENUM(BlueprintType)
enum class EEvadeDirection : uint8
{
	None     UMETA(DisplayName = "None"),
	Evade_Forward        UMETA(DisplayName = "Evade_Forward"),
	Evade_Left     UMETA(DisplayName = "Evade_Left"),
	Evade_Right     UMETA(DisplayName = "Evade_Right"),
	Evade_Backward     UMETA(DisplayName = "Evade_Backward")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LOCOMOTIONSERIES_API UEvadeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEvadeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetEvadeDirection(FVector2D MovementVector);

private:
	
	class AMainCharacter* MainCharacterRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	bool bIsEvading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	EEvadeDirection EvadeDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EvadeMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	FName EvadeSectionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* TransparentMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* WeaponMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Evade/Dodge", meta = (AllowPrivateAccess = "true"))
	USoundBase* EvadeSound;

	FOnMontageBlendingOutStarted OnMontageBlendingOutStarted;
	FOnMontageBlendedInEnded OnMontageBlendingInEnded;

	void OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	void OnEvadeMontageBlendedInEnded(UAnimMontage* Montage);
	
	FTimerHandle ResetEvadeDirectionTimerHandle;

	float ResetEvadeDirectionTimeRate;

	void ResetEvadeDirection();

public:
	void SendMovementVector(FVector2D MovementVector);
	void Evade(AMainCharacter* MainCharacter);
	
	FORCEINLINE bool GetIsEvading() const { return bIsEvading; }
};
