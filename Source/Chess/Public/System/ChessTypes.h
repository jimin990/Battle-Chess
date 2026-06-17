#pragma once

#include "CoreMinimal.h"
#include "ChessTypes.generated.h"

UENUM(BlueprintType)
enum class EChessTeam : uint8
{
    None,
    White,
    Black
};

UENUM(BlueprintType)
enum class EChessPieceType : uint8
{
	None UMETA(DisplayName = "None"),

	Pawn UMETA(DisplayName = "Pawn"),

	Rook UMETA(DisplayName = "Rook"),

	Knight UMETA(DisplayName = "Knight"),

	Bishop UMETA(DisplayName = "Bishop"),

	Queen UMETA(DisplayName = "Queen"),

	King UMETA(DisplayName = "King")
};

USTRUCT(BlueprintType)
struct FChessBattleStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange = 150.f;
};