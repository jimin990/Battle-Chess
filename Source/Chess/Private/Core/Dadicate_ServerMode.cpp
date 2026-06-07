// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Dadicate_ServerMode.h"

ADadicate_ServerMode::ADadicate_ServerMode()
{
	bUseSeamlessTravel = true;
}

void ADadicate_ServerMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("Dadicate - PreLogin!"));
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void ADadicate_ServerMode::PostLogin(APlayerController* NewPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Dadicate - PostLogin! %s"), *NewPlayer->GetName());
	Super::PostLogin(NewPlayer);

	//플레이어 2명 접속시 본 게임으로 넘어감
	if (GetNumPlayers() == 2)
	{
		SeamTraval();
	}
}

void ADadicate_ServerMode::SeamTraval()
{
	GetWorld()->ServerTravel("/Game/Chess/ChessGame/Chess2");
}
