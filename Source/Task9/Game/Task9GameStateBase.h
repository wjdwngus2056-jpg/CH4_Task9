#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Task9GameStateBase.generated.h"

UCLASS()
class TASK9_API ATask9GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("Player")));

	UPROPERTY(Replicated, BlueprintReadOnly)
	FString CurrentTurnPlayerName;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 RemainingTurnTime = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 MaxTurnTime = 5;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 CurrentTurnGuessCount = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 MaxTurnGuessCount = 3;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FString ResultMessage;
};
