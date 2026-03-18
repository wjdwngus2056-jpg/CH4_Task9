#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "Task9ChatInput.generated.h"

class UEditableTextBox;
class UScrollBox;

UCLASS()
class TASK9_API UTask9ChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	void ShowChatMessage(const FString& InChatMessageString);
	
protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
public:
	UPROPERTY(meta=(BindWidgetoptional))
	TObjectPtr<UEditableTextBox> ChatInput;
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UScrollBox> ChatMessageScrollBox;
};
