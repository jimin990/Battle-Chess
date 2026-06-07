// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PieceStatusWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class CHESS_API UPieceStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	// Piece Type
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChessPieceTypeText;

	// Piece Ã¼·Â
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ChessPieceHPText;

};
