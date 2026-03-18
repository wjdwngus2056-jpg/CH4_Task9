#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Task9GameModeBase.generated.h"

class ATask9PlayerController;

UCLASS()
class TASK9_API ATask9GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	
public:
	FString GenerateSecretNumber();
	bool IsGuessNumberString(const FString& InNumberString);
	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);
	
public:
	void PrintChatMessageString(ATask9PlayerController* InChattingPlayerController, const FString& InChatMessageString);
	
protected:
	FString SecretNumberString;
	TArray<TObjectPtr<ATask9PlayerController>> AllPlayerControllers;
	
};
