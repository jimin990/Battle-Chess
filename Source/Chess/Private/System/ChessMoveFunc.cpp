// Fill out your copyright notice in the Description page of Project Settings.


#include "System/ChessMoveFunc.h"
#include "Character/ChessPiece/ChessPieceBase.h"

FBoardCoord UChessMoveFunc::IndexToCoord(int32 Index)
{
    return FBoardCoord(Index % 8, Index / 8);
}

int32 UChessMoveFunc::CoordToIndex(int32 X, int32 Y)
{
    return Y * 8 + X;
}

bool UChessMoveFunc::IsValidCoord(int32 X, int32 Y)
{
    return X >= 0 && X < 8 && Y >= 0 && Y < 8;
}

bool UChessMoveFunc::IsSameTeam(const AChessPieceBase* A, const AChessPieceBase* B)
{
    if (!A || !B)
    {
        return false;
    }

    bool bIsSame = A->Team == B->Team;

    if (bIsSame)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Immediately another team piece is placed."));
    }

    return bIsSame;
}

TArray<int32> UChessMoveFunc::GetSlidingMoves(const AChessPieceBase* Piece, const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces, const TArray<FBoardCoord>& Directions)
{
    TArray<int32> Result;

    if (!Piece)
    {
        return Result;
    }

    FBoardCoord Coord =
        IndexToCoord(Piece->OwnIndex);

    for (const FBoardCoord& Dir : Directions)
    {
        int32 X = Coord.X;
        int32 Y = Coord.Y;

        while (true)
        {
            X += Dir.X;
            Y += Dir.Y;

            if (!IsValidCoord(X, Y))
            {
                break;
            }

            int32 MoveIndex =
                CoordToIndex(X, Y);

            AChessPieceBase* OtherPiece =
                ChessPieces[MoveIndex];

            // 빈 칸
            if (OtherPiece == nullptr)
            {
                Result.Add(MoveIndex);
                continue;
            }

            // 적 말
            if (!IsSameTeam(Piece, OtherPiece))
            {
                Result.Add(MoveIndex);
            }

            // 말 만나면 종료
            break;
        }
    }

    return Result;
}

TArray<int32> UChessMoveFunc::GetPieceMoves(
    const AChessPieceBase* Piece, 
    const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces)
{
    TArray<int32> Result;

    if (Piece->PieceType == EChessPieceType::Pawn)
    {
        Result= GetPawnMoves(Piece, ChessPieces);
    }
    else if (Piece->PieceType == EChessPieceType::Bishop)
    {
        Result = GetBishopMoves(Piece, ChessPieces);
    }
    else if (Piece->PieceType == EChessPieceType::Rook)
    {
        Result = GetRookMoves(Piece, ChessPieces);
    }
    else if (Piece->PieceType == EChessPieceType::Knight)
    {
        Result = GetKnightMoves(Piece, ChessPieces);
    }
    else if (Piece->PieceType == EChessPieceType::Queen)
    {
        Result = GetQueenMoves(Piece, ChessPieces);
    }
    else if (Piece->PieceType == EChessPieceType::King)
    {
        Result = GetKingMoves(Piece, ChessPieces);
    }

    return Result;
}

TArray<int32> UChessMoveFunc::GetPawnMoves(
    const AChessPieceBase* Piece, 
    const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces)
{
    TArray<int32> Result;

    FBoardCoord Coord =
        IndexToCoord(Piece->OwnIndex);

    int32 Direction =
        Piece->Team ==
        EChessTeam::White
        ? 1 : -1;

    int32 ForwardY =
        Coord.Y + Direction;

    // 전진
    if (IsValidCoord(Coord.X, ForwardY))
    {
        int32 ForwardIndex =
            CoordToIndex(
                Coord.X,
                ForwardY
            );

        // 앞이 비었을 때만
        if (ChessPieces[ForwardIndex]
            == nullptr)
        {
            Result.Add(ForwardIndex);
        }
    }

    // 왼쪽 공격
    if (IsValidCoord(
        Coord.X - 1,
        ForwardY))
    {
        int32 AttackIndex =
            CoordToIndex(
                Coord.X - 1,
                ForwardY
            );

        AChessPieceBase* OtherPiece =
            ChessPieces[AttackIndex];

        if (OtherPiece &&
            !IsSameTeam(
                Piece,
                OtherPiece))
        {
            Result.Add(AttackIndex);
        }
    }

    // 오른쪽 공격
    if (IsValidCoord(
        Coord.X + 1,
        ForwardY))
    {
        int32 AttackIndex =
            CoordToIndex(
                Coord.X + 1,
                ForwardY
            );

        AChessPieceBase* OtherPiece =
            ChessPieces[AttackIndex];

        if (OtherPiece &&
            !IsSameTeam(
                Piece,
                OtherPiece))
        {
            Result.Add(AttackIndex);
        }
    }

    return Result;
}

TArray<int32> UChessMoveFunc::GetBishopMoves(
    const AChessPieceBase* Piece, 
    const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces)
{
    TArray<FBoardCoord> Directions =
    {
        FBoardCoord(1, 1),
        FBoardCoord(-1, 1),
        FBoardCoord(1, -1),
        FBoardCoord(-1, -1)
    };

    return GetSlidingMoves(
        Piece,
        ChessPieces,
        Directions
    );
}

TArray<int32> UChessMoveFunc::GetRookMoves(
    const AChessPieceBase* Piece,
    const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces)
{
    TArray<FBoardCoord> Directions =
    {
        FBoardCoord(0, 1),
        FBoardCoord(0, -1),
        FBoardCoord(-1, 0),
        FBoardCoord(1, 0)
    };

    return GetSlidingMoves(
        Piece,
        ChessPieces,
        Directions
    );
}

TArray<int32> UChessMoveFunc::GetKnightMoves(
    const AChessPieceBase* Piece,
    const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces)
{
    TArray<int32> Result;

    FBoardCoord Coord =
        IndexToCoord(Piece->OwnIndex);

    const int32 DX[8] =
    {
        -2, -1, 1, 2,
        2, 1, -1, -2
    };

    const int32 DY[8] =
    {
        1, 2, 2, 1,
        -1, -2, -2, -1
    };

    for (int32 i = 0; i < 8; i++)
    {
        int32 X = Coord.X + DX[i];
        int32 Y = Coord.Y + DY[i];

        if (!IsValidCoord(X, Y))
        {
            continue;
        }

        int32 MoveIndex =
            CoordToIndex(X, Y);

        AChessPieceBase* OtherPiece =
            ChessPieces[MoveIndex];

        // 빈 칸
        if (OtherPiece == nullptr)
        {
            Result.Add(MoveIndex);
            continue;
        }

        // 적 말
        if (!IsSameTeam(Piece, OtherPiece))
        {
            Result.Add(MoveIndex);
        }
    }

    return Result;
}

TArray<int32> UChessMoveFunc::GetQueenMoves(
    const AChessPieceBase* Piece,
    const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces)
{
    TArray<FBoardCoord> Directions =
    {
        FBoardCoord(1, 1),
        FBoardCoord(-1, 1),
        FBoardCoord(1, -1),
        FBoardCoord(-1, -1),

        FBoardCoord(0, 1),
        FBoardCoord(0, -1),
        FBoardCoord(-1, 0),
        FBoardCoord(1, 0)
    };

    return GetSlidingMoves(
        Piece,
        ChessPieces,
        Directions
    );
}

TArray<int32> UChessMoveFunc::GetKingMoves(
    const AChessPieceBase* Piece,
    const TArray<TObjectPtr<AChessPieceBase>>& ChessPieces)
{
    TArray<int32> Result;

    FBoardCoord Coord =
        IndexToCoord(Piece->OwnIndex);

    for (int32 XOffset = -1;
        XOffset <= 1;
        XOffset++)
    {
        for (int32 YOffset = -1;
            YOffset <= 1;
            YOffset++)
        {
            if (XOffset == 0 &&
                YOffset == 0)
            {
                continue;
            }

            int32 X = Coord.X + XOffset;
            int32 Y = Coord.Y + YOffset;

            if (!IsValidCoord(X, Y))
            {
                continue;
            }

            int32 MoveIndex =
                CoordToIndex(X, Y);

            AChessPieceBase* OtherPiece =
                ChessPieces[MoveIndex];

            if (OtherPiece == nullptr)
            {
                Result.Add(MoveIndex);
                continue;
            }

            if (!IsSameTeam(Piece, OtherPiece))
            {
                Result.Add(MoveIndex);
            }
        }
    }

    return Result;
}