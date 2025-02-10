// Fill out your copyright notice in the Description page of Project Settings.


#include "EomcriCharacter.h"
#include "EomcriPlayerController.h"
#include "EnhancedInputComponent.h"
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

    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA�� �������� ���� ���� ���� ���� Controller�� ASpartaPlayerController�� ĳ����
        if (AEomcriPlayerController* PlayerController = Cast<AEomcriPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AEomcriCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                // IA_Jump �׼� Ű�� "Ű�� ������ �ִ� ����" StartJump() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AEomcriCharacter::StartJump
                );

                // IA_Jump �׼� Ű���� "���� �� ����" StopJump() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AEomcriCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                // IA_Look �׼� ���콺�� "������ ��" Look() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AEomcriCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                // IA_Sprint �׼� Ű�� "������ �ִ� ����" StartSprint() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AEomcriCharacter::StartSprint
                );
                // IA_Sprint �׼� Ű���� "���� �� ����" StopSprint() ȣ��
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Completed,
                    this,
                    &AEomcriCharacter::StopSprint
                );
            }
        }
    }
}

void AEomcriCharacter::Move(const FInputActionValue & value)
{
    // ��Ʈ�ѷ��� �־�� ���� ����� ����
    if (!Controller) return;

    // Value�� Axis2D�� ������ IA_Move�� �Է°� (WASD)�� ��� ����
// ��) (X=1, Y=0) �� ���� / (X=-1, Y=0) �� ���� / (X=0, Y=1) �� ������ / (X=0, Y=-1) �� ����
    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // ĳ���Ͱ� �ٶ󺸴� ����(����)���� X�� �̵�
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        // ĳ������ ������ �������� Y�� �̵�
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void AEomcriCharacter::StartJump(const FInputActionValue & value)
{
}

void AEomcriCharacter::StopJump(const FInputActionValue & value)
{
}

void AEomcriCharacter::Look(const FInputActionValue & value)
{
}

void AEomcriCharacter::StartSprint(const FInputActionValue & value)
{
}

void AEomcriCharacter::StopSprint(const FInputActionValue & value)
{
}

