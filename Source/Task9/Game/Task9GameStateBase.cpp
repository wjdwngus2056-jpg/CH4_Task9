#include "Task9GameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Task9PlayerController.h"

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
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				Task9PC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
