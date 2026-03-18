#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Task9GameModeBase.generated.h"

class ATask9PlayerController;
class ATask9GameStateBase;

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
	void IncreaseGuessCount(ATask9PlayerController* InChattingPlayerController);
	
	void ResetGame();
	void JudgeGame(ATask9PlayerController* InChattingPlayerController, int InStrikeCount);

public:
	void StartTurn(int32 InTurnIndex);
	void EndTurn();
	void AdvanceTurn();
	void HandleTurnTimerTick();
	void HandleDelayedReset();
	bool IsCurrentTurnPlayer(const ATask9PlayerController* InPlayerController) const;
	void BroadcastSystemMessage(const FString& InMessageString);
	void UpdateTurnUI() const;
	
protected:
	FString SecretNumberString;
	TArray<TObjectPtr<ATask9PlayerController>> AllPlayerControllers;
	int32 CurrentTurnIndex = INDEX_NONE;
	int32 RemainingTurnTime = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 TurnDuration = 10;

	UPROPERTY(EditDefaultsOnly)
	float ResetDelay = 4.f;

	FTimerHandle TurnTimerHandle;
	FTimerHandle ResetTimerHandle;
};
