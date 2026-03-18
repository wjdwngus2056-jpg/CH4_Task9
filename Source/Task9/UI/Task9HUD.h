#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Task9HUD.generated.h"

class UProgressBar;
class UTextBlock;
class UTask9ChatInput;

UCLASS()
class TASK9_API UTask9HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void ShowChatMessage(const FString& InChatMessageString);

public:
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UProgressBar> TurnTimer;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> WinnerTitleText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> CurrentTurnPlayerNameText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> GuessCountText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> ResultMessageText;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UTask9ChatInput> ChatInputWidget;
};
