#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Task9PlayerState.generated.h"

UCLASS()
class TASK9_API ATask9PlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATask9PlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;
};
