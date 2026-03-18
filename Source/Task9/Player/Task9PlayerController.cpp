#include "Task9PlayerController.h"

#include "EngineUtils.h"
#include "Task9.h"
#include "Game/Task9GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Task9ChatInput.h"

void ATask9PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	
	FInputModeUIOnly FInputModeUIOnly;
	SetInputMode(FInputModeUIOnly);
	
	if (IsValid(ChatInputWidgetClass))
	{
		ChatInputWidgetInstance = CreateWidget<UTask9ChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance))
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
}

void ATask9PlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	
	if (IsLocalController())
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void ATask9PlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	if (IsValid(ChatInputWidgetInstance))
	{
		ChatInputWidgetInstance->ShowChatMessage(InChatMessageString);
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
