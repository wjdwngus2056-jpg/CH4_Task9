#include "Task9HUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Game/Task9GameStateBase.h"
#include "UI/Task9ChatInput.h"

void UTask9HUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTask9HUD::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTask9HUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ATask9GameStateBase* Task9GameStateBase = GetWorld() ? GetWorld()->GetGameState<ATask9GameStateBase>() : nullptr;
	if (!IsValid(Task9GameStateBase))
	{
		return;
	}

	if (IsValid(TurnTimer))
	{
		const float Percent = Task9GameStateBase->MaxTurnTime > 0
			? static_cast<float>(Task9GameStateBase->RemainingTurnTime) / static_cast<float>(Task9GameStateBase->MaxTurnTime)
			: 0.f;
		TurnTimer->SetPercent(Percent);
	}

	if (IsValid(CurrentTurnPlayerNameText))
	{
		CurrentTurnPlayerNameText->SetText(FText::FromString(Task9GameStateBase->CurrentTurnPlayerName));
	}

	if (IsValid(GuessCountText))
	{
		const FString GuessCountString = FString::Printf(TEXT("%d / %d"),
			Task9GameStateBase->CurrentTurnGuessCount,
			Task9GameStateBase->MaxTurnGuessCount);
		GuessCountText->SetText(FText::FromString(GuessCountString));
	}

	const bool bHasWinnerMessage = !Task9GameStateBase->ResultMessage.IsEmpty();

	if (IsValid(WinnerTitleText))
	{
		WinnerTitleText->SetVisibility(bHasWinnerMessage ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (IsValid(ResultMessageText))
	{
		ResultMessageText->SetText(FText::FromString(Task9GameStateBase->ResultMessage));
		ResultMessageText->SetVisibility(bHasWinnerMessage ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UTask9HUD::ShowChatMessage(const FString& InChatMessageString)
{
	if (IsValid(ChatInputWidget))
	{
		ChatInputWidget->ShowChatMessage(InChatMessageString);
	}
}
