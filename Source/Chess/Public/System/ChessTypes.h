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