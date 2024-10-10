// Fill out your copyright notice in the Description page of Project Settings.


#include "EvadeComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEvadeComponent::UEvadeComponent() :
	EvadeSectionName(FName(TEXT(""))),
	bIsEvading(false),
	EvadeDirection(EEvadeDirection::None),
	ResetEvadeDirectionTimeRate(0.1f)
{

}


// Called when the game starts
void UEvadeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEvadeComponent::SendMovementVector(FVector2D MovementVector)
{
	SetEvadeDirection(MovementVector);

	GetWorld()->GetTimerManager().SetTimer(ResetEvadeDirectionTimerHandle, this , 
		&UEvadeComponent::ResetEvadeDirection, ResetEvadeDirectionTimeRate);	
}

void UEvadeComponent::SetEvadeDirection(FVector2D MovementVector)
{
	if (MovementVector.Y > 0)
	{
		EvadeDirection = EEvadeDirection::Evade_Forward;
	}
	else if (MovementVector.Y < 0)
	{
		EvadeDirection = EEvadeDirection::Evade_Backward;
	}
	else if (MovementVector.X < 0)
	{
		EvadeDirection = EEvadeDirection::Evade_Left;
	}
	else if (MovementVector.X > 0)
	{
		EvadeDirection = EEvadeDirection::Evade_Right;
	}
	else
	{
		EvadeDirection = EEvadeDirection::None;
	}
}

void UEvadeComponent::Evade(AMainCharacter* MainCharacter)
{
	if (!MainCharacter) return;

	if (EvadeDirection == EEvadeDirection::None) return;

	bIsEvading = true;

	MainCharacterRef = MainCharacter;

	// Hide weapon.
	if (TransparentMaterial)
	{
		if (MainCharacter)
		{
			MainCharacter->GetMesh()->SetMaterial(3, TransparentMaterial);
		}
	}

	if (MainCharacter)
	{
		// Fetch related animation.
		UAnimInstance* AnimInstance = MainCharacter->GetMesh()->GetAnimInstance();

		switch (EvadeDirection)
		{
		case EEvadeDirection::None:
			EvadeSectionName = "";
			break;
		case EEvadeDirection::Evade_Forward:
			EvadeSectionName = "Evade_Forward";
			break;
		case EEvadeDirection::Evade_Left:
			EvadeSectionName = "Evade_Left";
			break;
		case EEvadeDirection::Evade_Right:
			EvadeSectionName = "Evade_Right";
			break;
		case EEvadeDirection::Evade_Backward:
			EvadeSectionName = "Evade_Backward";
			break;
		}

		if (AnimInstance)
		{
			if (EvadeMontage)
			{
				AnimInstance->Montage_Play(EvadeMontage);

				AnimInstance->Montage_JumpToSection(EvadeSectionName);

				OnMontageBlendingInEnded.BindUObject(this, &UEvadeComponent::OnEvadeMontageBlendedInEnded);
				OnMontageBlendingOutStarted.BindUObject(this, &UEvadeComponent::OnEvadeMontageBlendingOut);

				// Subscribe to the blending in delegate.
				AnimInstance->Montage_SetBlendedInDelegate(OnMontageBlendingInEnded, EvadeMontage);

				// Subscribe to the blending out delegate.
				AnimInstance->Montage_SetBlendingOutDelegate(OnMontageBlendingOutStarted, EvadeMontage);
			}
		}
	}
}

void UEvadeComponent::OnEvadeMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (!MainCharacterRef) return;

	// Return control to the player.
	bIsEvading = false;

	EvadeDirection = EEvadeDirection::None;

	// Show weapon.
	if (WeaponMaterial)
	{
		if (MainCharacterRef)
		{
			MainCharacterRef->GetMesh()->SetMaterial(3, WeaponMaterial);
		}
	}
}

void UEvadeComponent::OnEvadeMontageBlendedInEnded(UAnimMontage* Montage)
{
	if (EvadeSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), EvadeSound);
	}
}

void UEvadeComponent::ResetEvadeDirection()
{
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
				FString::Printf(TEXT("Evade Direction: %d"), EvadeDirection));
	}*/

	EvadeDirection = EEvadeDirection::None;

	// EvadeSectionName(FName(TEXT("")))
}
