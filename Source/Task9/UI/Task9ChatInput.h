#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Task9ChatInput.generated.h"

class UEditableTextBox;

UCLASS()
class TASK9_API UTask9ChatInput : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEditableTextBox> ChatInput;
};
