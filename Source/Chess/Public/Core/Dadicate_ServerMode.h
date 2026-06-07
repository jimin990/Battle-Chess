// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Dadicate_ServerMode.generated.h"

/**
 * 
 */
UCLASS()
class CHESS_API ADadicate_ServerMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADadicate_ServerMode();
	
public:
	// 접속시 가장 먼저 실행
	virtual void PreLogin(
		const FString& Options,
		const FString& Address,
		const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage
	) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SeamTraval();
};
