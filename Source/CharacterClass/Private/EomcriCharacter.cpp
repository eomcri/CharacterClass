// Fill out your copyright notice in the Description page of Project Settings.


#include "EomcriCharacter.h"
// ī�޶�, ������ �� ���� ������ �ʿ��� ���� include
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AEomcriCharacter::AEomcriCharacter()
{
    // Tick �Լ��� �켱 ���Ӵϴ�.
    PrimaryActorTick.bCanEverTick = false;

    // (1) ������ �� ����
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // ������ ���� ��Ʈ ������Ʈ (CapsuleComponent)�� ����
    SpringArmComp->SetupAttachment(RootComponent);
    // ĳ���Ϳ� ī�޶� ������ �Ÿ� �⺻�� 300���� ����
    SpringArmComp->TargetArmLength = 300.0f;
    // ��Ʈ�ѷ� ȸ���� ���� ������ �ϵ� ȸ���ϵ��� ����
    SpringArmComp->bUsePawnControlRotation = true;

    // (2) ī�޶� ������Ʈ ����
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    // ������ ���� ���� ��ġ�� ī�޶� ����
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    // ī�޶�� ������ ���� ȸ���� �����Ƿ� PawnControlRotation�� ����
    CameraComp->bUsePawnControlRotation = false;
}

// Called to bind functionality to input
void AEomcriCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

