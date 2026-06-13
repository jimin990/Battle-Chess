// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/LobbyGameMode.h"
#include "Character/Players/MyChessPlayerController.h"

ALobbyGameMode::ALobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void ALobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("Lobby - PreLogin!"));

	if (GetNumPlayers() >= 2)
	{
		ErrorMessage = TEXT("Game is full");
	}
}

APlayerController* ALobbyGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("Lobby - Login!"));
	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Lobby - PostLogin! %s"), *NewPlayer->GetName());
	Super::PostLogin(NewPlayer);

	//플레이어 2명 접속시 본 게임으로 넘어감
	if (GetNumPlayers() == 2)
	{
		//UE_LOG(LogTemp, Log, TEXT("Next Level!"));
		NextLevel();
	}
}

void ALobbyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Lobby - HandleStarting! %s"), *NewPlayer->GetName());
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ALobbyGameMode::NextLevel()
{
	GetWorld()->ServerTravel("/Game/Chess/ChessGame/Chess2");
}
