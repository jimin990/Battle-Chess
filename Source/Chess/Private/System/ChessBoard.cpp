// Fill out your copyright notice in the Description page of Project Settings.


#include "System/ChessBoard.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AChessBoard::AChessBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	ISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM"));
	ISM->NumCustomDataFloats = 3;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	ISM->SetupAttachment(RootComponent);

	// 이동가능 위치 ISM
	HighlightISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("HighlightISM"));
	HighlightISM->NumCustomDataFloats = 3;

	HighlightISM->SetCollisionEnabled(
		ECollisionEnabled::NoCollision
	);
}

// Called when the game starts or when spawned
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
	
	SetBoard();
}

// Called every frame
void AChessBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessBoard::SetBoard()
{
	if (!SM)
	{
		UE_LOG(LogTemp, Warning, TEXT("Static Mesh is Null!"));
		return;
	}

	ISM->SetStaticMesh(SM);
	HighlightISM->SetStaticMesh(SM);

	FBoxSphereBounds Bounds = SM->GetBounds();
	FVector MeshExtent = Bounds.BoxExtent;           // Mesh의 Half Size

	// 실제 타일 한 칸의 World Size (Scale 적용 후)
	FVector TileWorldSize = MeshExtent * 2.0f * FVector(0.25f, 0.25f, 0.25f);


	float TileSizeX = TileWorldSize.X;
	float TileSizeY = TileWorldSize.Y;
	float TileSizeZ = TileWorldSize.Z;

	FTransform TileTransform;
	//TileTransform.SetScale3D(FVector(0.25f, 0.25f, 0.25f)); // 기존
	TileTransform.SetScale3D(FVector(0.25f, 0.25f, 1.0f));
	TileTransform.SetRotation(FRotator(0, 0, 0).Quaternion());

	for (int32 i = 0; i < 8; i++)
	{
		for (int32 j = 0; j < 8; j++)
		{
			// 중앙 기준 위치 계산 (Actor Location 기준)
			FVector Location =
				FVector(
					(j - 3.5f) * TileSizeX,
					(i - 3.5f) * TileSizeY,
					TileSizeZ
				);

			TileTransform.SetLocation(Location);

			bool bIsLightSquare = ((i + j) % 2 == 0);
			float TileColor = bIsLightSquare ? 1.0f : 0.0f;   // Material Parameter

			int32 InstanceIndex = ISM->AddInstance(TileTransform);
			ISM->SetCustomDataValue(InstanceIndex, 0, TileColor, true);
			ISM->SetCustomDataValue(InstanceIndex, 1, TileColor, true);
			ISM->SetCustomDataValue(InstanceIndex, 2, TileColor, true);
		}
	}
}

void AChessBoard::VisibleHighlight(TArray<int32>& Arr)
{
	FTransform Transform;

	for (int32 i : Arr)
	{
		ISM->GetInstanceTransform(i, Transform, true);

		Transform.SetLocation(Transform.GetLocation() + FVector(0, 0, 20));

		int32 InstanceIndex = HighlightISM->AddInstance(Transform);

		HighlightISM->SetCustomDataValue(InstanceIndex, 0, 1, true);
		HighlightISM->SetCustomDataValue(InstanceIndex, 1, 0, true);
		HighlightISM->SetCustomDataValue(InstanceIndex, 2, 0, true);
	}
}

void AChessBoard::InvisibleHighlight()
{
	HighlightISM->ClearInstances();
}
