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
		&AObstacleSpawner::SpawnPattern, // 변경: 패턴 단위 스폰
		SpawnInterval, true);
}

void AObstacleSpawner::SpawnPattern()
{
	// 디자인된 패턴이 없으면 스킵
	if (Patterns.Num() == 0) return;

	// 패턴 하나를 랜덤 선택
	const int32 Index = FMath::RandRange(0, Patterns.Num() - 1);
	const FObstaclePattern& Chosen = Patterns[Index];

	// 스폰 기준점 (스포너 위치 + 위쪽 높이)
	const FVector Origin = GetActorLocation() + FVector(0.0f, 0.0f, SpawnHeight);

	// 패턴에 속한 각 오프셋마다 풀에서 장애물 꺼내 배치
	for (const FVector& Offset : Chosen.ObstacleOffsets)
	{
		AObstacle* Obstacle = GetPooledObstacle();
		if (Obstacle == nullptr)
		{
			// 풀이 부족하면 이 패턴의 나머지는 포기 (PoolSize 늘려 해결)
			break;
		}
		Obstacle->Activate(Origin + Offset);
	}
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