﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EomcriCharacter.h"
#include "EomcriPlayerController.h"
#include "EnhancedInputComponent.h"
// 카메라, 스프링 암 실제 구현이 필요한 경우라서 include
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEomcriCharacter::AEomcriCharacter()
{
    // Tick 함수는 우선 꺼둡니다.
    PrimaryActorTick.bCanEverTick = false;

    // (1) 스프링 암 생성
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    // 스프링 암을 루트 컴포넌트 (CapsuleComponent)에 부착
    SpringArmComp->SetupAttachment(RootComponent);
    // 캐릭터와 카메라 사이의 거리 기본값 300으로 설정
    SpringArmComp->TargetArmLength = 300.0f;
    // 컨트롤러 회전에 따라 스프링 암도 회전하도록 설정
    SpringArmComp->bUsePawnControlRotation = true;

    // (2) 카메라 컴포넌트 생성
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    // 스프링 암의 소켓 위치에 카메라를 부착
    CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
    // 카메라는 스프링 암의 회전을 따르므로 PawnControlRotation은 꺼둠
    CameraComp->bUsePawnControlRotation = false;

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    // 초기 체력 설정
    MaxHealth = 100.0f;
    Health = MaxHealth;
}

// Called to bind functionality to input
void AEomcriCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent로 캐스팅
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA를 가져오기 위해 현재 소유 중인 Controller를 ASpartaPlayerController로 캐스팅
        if (AEomcriPlayerController* PlayerController = Cast<AEomcriPlayerController>(GetController()))
        {
            if (PlayerController->MoveAction)
            {
                // IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
                EnhancedInput->BindAction(
                    PlayerController->MoveAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AEomcriCharacter::Move
                );
            }

            if (PlayerController->JumpAction)
            {
                // IA_Jump 액션 키를 "키를 누르고 있는 동안" StartJump() 호출
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AEomcriCharacter::StartJump
                );

                // IA_Jump 액션 키에서 "손을 뗀 순간" StopJump() 호출
                EnhancedInput->BindAction(
                    PlayerController->JumpAction,
                    ETriggerEvent::Completed,
                    this,
                    &AEomcriCharacter::StopJump
                );
            }

            if (PlayerController->LookAction)
            {
                // IA_Look 액션 마우스가 "움직일 때" Look() 호출
                EnhancedInput->BindAction(
                    PlayerController->LookAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AEomcriCharacter::Look
                );
            }

            if (PlayerController->SprintAction)
            {
                // IA_Sprint 액션 키를 "누르고 있는 동안" StartSprint() 호출
                EnhancedInput->BindAction(
                    PlayerController->SprintAction,
                    ETriggerEvent::Triggered,
                    this,
                    &AEomcriCharacter::StartSprint
                );
                // IA_Sprint 액션 키에서 "손을 뗀 순간" StopSprint() 호출
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
    // 컨트롤러가 있어야 방향 계산이 가능
    if (!Controller) return;

    // Value는 Axis2D로 설정된 IA_Move의 입력값 (WASD)을 담고 있음
// 예) (X=1, Y=0) → 전진 / (X=-1, Y=0) → 후진 / (X=0, Y=1) → 오른쪽 / (X=0, Y=-1) → 왼쪽
    const FVector2D MoveInput = value.Get<FVector2D>();

    if (!FMath::IsNearlyZero(MoveInput.X))
    {
        // 캐릭터가 바라보는 방향(정면)으로 X축 이동
        AddMovementInput(GetActorForwardVector(), MoveInput.X);
    }

    if (!FMath::IsNearlyZero(MoveInput.Y))
    {
        // 캐릭터의 오른쪽 방향으로 Y축 이동
        AddMovementInput(GetActorRightVector(), MoveInput.Y);
    }
}

void AEomcriCharacter::StartJump(const FInputActionValue & value)
{
    // Jump 함수는 Character가 기본 제공
    if (value.Get<bool>())
    {
        Jump();
    }
}

void AEomcriCharacter::StopJump(const FInputActionValue & value)
{
    // StopJumping 함수도 Character가 기본 제공
    if (!value.Get<bool>())
    {
        StopJumping();
    }
}

void AEomcriCharacter::Look(const FInputActionValue & value)
{
    // 마우스의 X, Y 움직임을 2D 축으로 가져옴
    FVector2D LookInput = value.Get<FVector2D>();

    // X는 좌우 회전 (Yaw), Y는 상하 회전 (Pitch)
    // 좌우 회전
    AddControllerYawInput(LookInput.X);
    // 상하 회전
    AddControllerPitchInput(LookInput.Y);
}

void AEomcriCharacter::StartSprint(const FInputActionValue & value)
{
    // Shift 키를 누른 순간 이 함수가 호출된다고 가정
    // 스프린트 속도를 적용
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void AEomcriCharacter::StopSprint(const FInputActionValue & value)
{
    // Shift 키를 뗀 순간 이 함수가 호출
    // 평상시 속도로 복귀
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

float AEomcriCharacter::GetHealth() const
{
	return Health;
}

// 체력 회복 함수
void AEomcriCharacter::AddHealth(float Amount)
{
    // 체력을 회복시킴. 최대 체력을 초과하지 않도록 제한함
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Log, TEXT("Health increased to: %f"), Health);
}

// 데미지 처리 함수
float AEomcriCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // 기본 데미지 처리 로직 호출 (필수는 아님)
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    // 체력을 데미지만큼 감소시키고, 0 이하로 떨어지지 않도록 Clamp
    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    UE_LOG(LogTemp, Warning, TEXT("Health decreased to: %f"), Health);

    // 체력이 0 이하가 되면 사망 처리
    if (Health <= 0.0f)
    {
        OnDeath();
    }

    // 실제 적용된 데미지를 반환
    return ActualDamage;
}

// 사망 처리 함수
void AEomcriCharacter::OnDeath()
{
    UE_LOG(LogTemp, Error, TEXT("Character is Dead!"));

    // 사망 후 로직
}

