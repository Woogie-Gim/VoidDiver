#include "ObstacleSpawner.h"
#include "Obstacle.h"

AObstacleSpawner::AObstacleSpawner()
{
	PrimaryActorTick.bCanEverTick = true; // 시간 누적 위해 켬
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

void AObstacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ElapsedTime += DeltaTime; // 진행도 누적
}


void AObstacleSpawner::SpawnPattern()
{
	// 디자인된 패턴이 없으면 스킵
	if (Patterns.Num() == 0) return;

	// 패턴 하나를 랜덤 선택
	const int32 Index = PickPatternIndex(); // 가중치 기반 선택
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

// 진행도 0~1 반환
float AObstacleSpawner::GetProgress() const
{
	// Clamp로 0~1 범위 보장. TimeToMaxDifficulty초 지나면 1로 고정
	return FMath::Clamp(ElapsedTime / TimeToMaxDifficulty, 0.0f, 1.0f);
}

int32 AObstacleSpawner::PickPatternIndex() const
{
	const float Progress = GetProgress(); // 0~1

	// 각 패턴의 가중치 계산
	TArray<float> Weights;
	float TotalWeight = 0.0f;

	for (const FObstaclePattern& Pattern : Patterns)
	{
		// 진행도에 따라 무게 결정:
		// - 초반(Progress=0): 쉬운 패턴(Difficulty 낮음)에 큰 무게
		// - 후반(Progress=1): 어려운 패턴(Difficulty 높음)에 큰 무게
		// Lerp로 두 극단을 진행도만큼 섞음
		const float EasyWeight = 1.0f / Pattern.Difficulty;  // 쉬울수록 큰 값
		const float HardWeight = Pattern.Difficulty;         // 어려울수록 큰 값
		const float Weight = FMath::Lerp(EasyWeight, HardWeight, Progress);

		Weights.Add(Weight);
		TotalWeight += Weight;
	}

	// 0~TotalWeight 범위에서 랜덤값 하나 뽑기
	float Roll = FMath::FRandRange(0.0f, TotalWeight);

	// 룰렛: 무게를 누적하며 Roll이 걸리는 구간 찾기
	for (int32 i = 0; i < Weights.Num(); i++)
	{
		Roll -= Weights[i];
		if (Roll <= 0.0f)
		{
			return i; // 이 패턴 당첨
		}
	}

	return Patterns.Num() - 1; // 안전장치
}