#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Task9PlayerController.generated.h"

class UTask9HUD;

UCLASS()
class TASK9_API ATask9PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public: 
	virtual void BeginPlay() override;
	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString);
	
public:
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);
	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTask9HUD> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UTask9HUD> HUDWidgetInstance;
	
	FString ChatMessageString;
};
