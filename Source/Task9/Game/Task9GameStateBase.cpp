#include "Task9GameStateBase.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Task9PlayerController.h"

void ATask9GameStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentTurnPlayerName);
	DOREPLIFETIME(ThisClass, RemainingTurnTime);
	DOREPLIFETIME(ThisClass, MaxTurnTime);
	DOREPLIFETIME(ThisClass, CurrentTurnGuessCount);
	DOREPLIFETIME(ThisClass, MaxTurnGuessCount);
	DOREPLIFETIME(ThisClass, ResultMessage);
}

void ATask9GameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (!HasAuthority())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC))
		{
			ATask9PlayerController* Task9PC = Cast<ATask9PlayerController>(PC);
			if (IsValid(Task9PC))
			{
				FString NotificationString = InNameString + TEXT("이 게임에 참가했습니다.");
				Task9PC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
