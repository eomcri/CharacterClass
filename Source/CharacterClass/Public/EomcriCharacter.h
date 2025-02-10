// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EomcriCharacter.generated.h"

class USpringArmComponent; // ������ �� ���� Ŭ���� ���
class UCameraComponent; // ī�޶� ���� Ŭ���� ���� ����

UCLASS()
class CHARACTERCLASS_API AEomcriCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEomcriCharacter();

protected:

	// ������ �� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	// ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
