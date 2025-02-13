#include "EomcriPlayerController.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System의 Local Player Subsystem을 사용하기 위해 포함
#include "EomcriGameInstance.h"
#include "EomcriGameState.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

AEomcriPlayerController::AEomcriPlayerController()
    : InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{
}

void AEomcriPlayerController::BeginPlay()
{
	Super::BeginPlay();

    // 현재 PlayerController에 연결된 Local Player 객체를 가져옴    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem을 통해 우리가 할당한 IMC를 활성화
                // 우선순위(Priority)는 0이 가장 높은 우선순위
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    FString CurrentMapName = GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MenuLevel"))
    {
        ShowMainMenu(false);
    }
}

UUserWidget* AEomcriPlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

void AEomcriPlayerController::ShowMainMenu(bool bIsRestart)
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }

        if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
        {
	        if (bIsRestart)
	        {
                ButtonText->SetText(FText::FromString(TEXT("Restart")));
	        }
            else
            {
                ButtonText->SetText(FText::FromString(TEXT("Start")));
            }
        }
    }
}

void AEomcriPlayerController::ShowGameHUD()
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (HUDWidgetInstance)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();

            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());
        }

        AEomcriGameState* EomcriGameState = GetWorld() ? GetWorld()->GetGameState<AEomcriGameState>() : nullptr;
        if (EomcriGameState)
        {
            EomcriGameState->UpdateHUD();
        }
    }
}

void AEomcriPlayerController::StartGame()
{
    if (UEomcriGameInstance* EomcriGameInstance = Cast<UEomcriGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        EomcriGameInstance->CurrentLevelIndex = 0;
        EomcriGameInstance->TotalScore = 0;
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
}


