#include "ObstacleSpawner.h"
#include "Obstacle.h"

AObstacleSpawner::AObstacleSpawner()
{
	PrimaryActorTick.bCanEverTick = false; // 타이머로 스폰하니 Tick 불필요
}

void AObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();

	// SpawnInterval마다 SpawnObstacle 반복 호출
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AObstacleSpawner::SpawnObstacle,
		SpawnInterval,
		true // 반복
	);
}

void AObstacleSpawner::SpawnObstacle()
{
	if (ObstacleClass == nullptr)
	{
		return; // 클래스 지정 안 됐으면 스킵
	}

	// 스포너 위치 기준 + 좌우 랜덤 + 위쪽 높이에 스폰
	const float RandomX = FMath::FRandRange(-SpawnRangeX, SpawnRangeX);
	const FVector SpawnLocation = GetActorLocation() + FVector(RandomX, 0.0f, SpawnHeight);
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	GetWorld()->SpawnActor<AObstacle>(ObstacleClass, SpawnLocation, SpawnRotation);
}