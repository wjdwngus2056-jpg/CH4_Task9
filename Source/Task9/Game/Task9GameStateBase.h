#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Task9GameStateBase.generated.h"

UCLASS()
class TASK9_API ATask9GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("Player")));
};
