// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessBoard.generated.h"

class USceneComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class CHESS_API AChessBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY()
	TObjectPtr<USceneComponent> SceneComponent;

	//타일 매쉬
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMesh> SM;

	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> ISM;

	//보드 생성
	void SetBoard();
public:
	// 이동가능 표시를 위한 하이라이트 ISM
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> HighlightISM;

	//하이라이트 표시
	void VisibleHighlight(TArray<int32>& Arr);

	//하이라이트 표시
	void InvisibleHighlight();
};
