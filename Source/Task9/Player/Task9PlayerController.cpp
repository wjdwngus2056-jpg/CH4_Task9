#include "Task9PlayerController.h"

#include "Task9.h"
#include "Task9PlayerState.h"
#include "Game/Task9GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Task9HUD.h"

void ATask9PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	
	FInputModeUIOnly FInputModeUIOnly;
	SetInputMode(FInputModeUIOnly);
	
	if (IsValid(HUDWidgetClass))
	{
		HUDWidgetInstance = CreateWidget<UTask9HUD>(this, HUDWidgetClass);
		if (IsValid(HUDWidgetInstance))
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
}

void ATask9PlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	if (IsLocalController())
	{
		ATask9PlayerState* Taks9PS = GetPlayerState<ATask9PlayerState>();
		if (IsValid(Taks9PS) == true)
		{
			FString CombinedMessageString = Taks9PS->GetPlayerInfoString() + TEXT(" : ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ATask9PlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	if (IsValid(HUDWidgetInstance))
	{
		HUDWidgetInstance->ShowChatMessage(InChatMessageString);
		return;
	}

	Task9FunctionLibrary::PrintDebugLog(this, InChatMessageString, 10.f);
}

void ATask9PlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ATask9PlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM))
	{
		ATask9GameModeBase* Task9GM = Cast<ATask9GameModeBase>(GM);
		if (IsValid(Task9GM))
		{
			Task9GM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}
