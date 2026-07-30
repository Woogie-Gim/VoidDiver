#include "ObstacleSpawner.h"
#include "Obstacle.h"

AObstacleSpawner::AObstacleSpawner()
{
	PrimaryActorTick.bCanEverTick = false; // 타이머로 스폰하니 Tick 불필요
}

void AObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();

	InitializePool(); // 타이머 시작 전에 풀부터 채움

	// SpawnInterval마다 SpawnObstacle 반복 호출
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle, this,
		&AObstacleSpawner::SpawnObstacle, SpawnInterval, true);
}

void AObstacleSpawner::SpawnObstacle()
{
	// 풀에서 놀고 있는 장애물 하나 꺼냄
	AObstacle* Obstacle = GetPooledObstacle();
	if (Obstacle == nullptr)
	{
		return; // 풀이 꽉 찼으면 이번엔 스킵 (PoolSize 늘리면 해결)
	}

	// 좌우 랜덤 + 위쪽 높이 위치 계산
	const float RandomX = FMath::FRandRange(-SpawnRangeX, SpawnRangeX);
	const FVector SpawnLocation =
		GetActorLocation() + FVector(RandomX, 0.0f, SpawnHeight);

	Obstacle->Activate(SpawnLocation); // 꺼내서 그 위치에 활성화
}

void AObstacleSpawner::InitializePool()
{
	if (ObstacleClass == nullptr) return;

	for (int32 i = 0; i < PoolSize; i++)
	{
		// 멀리 숨겨둔 채로 미리 생성
		AObstacle* NewObstacle = GetWorld()->SpawnActor<AObstacle>(
			ObstacleClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (NewObstacle)
		{
			NewObstacle->Deactivate();      // 비활성 상태로 창고에
			ObstaclePool.Add(NewObstacle);  // 풀에 등록
		}
	}
}

AObstacle* AObstacleSpawner::GetPooledObstacle()
{
	// 풀을 돌며 비활성 장애물 찾기
	for (AObstacle* Obstacle : ObstaclePool)
	{
		if (Obstacle && !Obstacle->IsActive())
		{
			return Obstacle; // 놀고 있는 놈 반환
		}
	}
	return nullptr; // 다 쓰는 중이면 nullptr
}