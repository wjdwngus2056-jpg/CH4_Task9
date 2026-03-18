#include "Task9ChatInput.h"

#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Player/Task9PlayerController.h"

void UTask9ChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UTask9ChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	
	if (ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UTask9ChatInput::ShowChatMessage(const FString& InChatMessageString)
{
	if (!IsValid(ChatMessageScrollBox))
	{
		return;
	}

	UTextBlock* ChatMessageTextBlock = NewObject<UTextBlock>(this);
	if (IsValid(ChatMessageTextBlock))
	{
		ChatMessageTextBlock->SetText(FText::FromString(InChatMessageString));
		ChatMessageTextBlock->SetAutoWrapText(true);
		ChatMessageScrollBox->AddChild(ChatMessageTextBlock);
		ChatMessageScrollBox->ScrollToEnd();
	}
}

void UTask9ChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ATask9PlayerController* OwningCXPlayerController = Cast<ATask9PlayerController>(OwningPlayerController);
			if (IsValid(OwningCXPlayerController) == true)
			{
				OwningCXPlayerController->SetChatMessageString(Text.ToString());

				ChatInput->SetText(FText());
			}
		}
	}
}
