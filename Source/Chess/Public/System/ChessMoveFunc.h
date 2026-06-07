// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChessMoveFunc.generated.h"

class AChessPieceBase;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FBoardCoord
{
	GENERATED_BODY()

	UPROPERTY()
	int32 X = 0;

	UPROPERTY()
	int32 Y = 0;

	FBoardCoord() {}

	FBoardCoord(int32 InX, int32 InY)
	{
		X = InX;
		Y = InY;
	}
};

UCLASS()
class CHESS_API UChessMoveFunc : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FBoardCoord IndexToCoord(int32 Index);

	static int32 CoordToIndex(int32 X, int32 Y);

	static bool IsValidCoord(int32 X, int32 Y);

	// 같은 팀인지 검사
	static bool IsSameTeam(
		const AChessPieceBase* A,
		const AChessPieceBase* B
	);

	//대각선 이동 공통 함수
	static TArray<int32> GetSlidingMoves(
		const AChessPieceBase* Piece,
		const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces,
		const TArray<FBoardCoord>& Directions
	);

public:
	// 선택한 말의 이동 가능 위치를 Index 배열로 반환하는 함수
	static TArray<int32> GetPieceMoves(
		const AChessPieceBase* Piece, 
		const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces
	);

	// 폰 이동
	static TArray<int32> GetPawnMoves(
		const AChessPieceBase* Piece, 
		const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces
	);

	// 비숍 이동
	static TArray<int32> GetBishopMoves(
		const AChessPieceBase* Piece, 
		const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces
	);

	// 룩 이동
	static TArray<int32> GetRookMoves(
		const AChessPieceBase* Piece,
		const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces
	);

	// 나이트 이동
	static TArray<int32> GetKnightMoves(
		const AChessPieceBase* Piece,
		const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces
	);

	// 퀸 이동
	static TArray<int32> GetQueenMoves(
		const AChessPieceBase* Piece,
		const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces
	);

	// 킹 이동
	static TArray<int32> GetKingMoves(
		const AChessPieceBase* Piece,
		const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces
	);
};
