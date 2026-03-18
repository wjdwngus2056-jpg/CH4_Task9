#include "Task9PlayerState.h"

#include "Net/UnrealNetwork.h"

ATask9PlayerState::ATask9PlayerState()
	: PlayerNameString(TEXT("None")),
	  CurrentGuessCount(0),
	  MaxGuessCount(3)
{
	bReplicates = true;
}

void ATask9PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}

FString ATask9PlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString;

	return PlayerInfoString;
}
