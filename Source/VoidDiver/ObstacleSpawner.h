#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataAsset.h"
#include "ObstacleSpawner.generated.h"

// 패턴 하나 = 장애물들의 상대 위치 목록
USTRUCT(BlueprintType)
struct FObstaclePattern
{
	GENERATED_BODY()

	// 이 패턴에 속한 장애물들의 상대 위치 (스폰 기준점에서의 오프셋)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	TArray<FVector> ObstacleOffsets;

	// 난이도. 0=쉬움, 값이 클수록 어려움. BP에서 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
	float Difficulty = 1.0f;
};

UCLASS()
class VOIDDIVER_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()

public:
	AObstacleSpawner();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// 스폰할 장애물 클래스. BP에서 BP_Obstacle 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<class AObstacle> ObstacleClass;

	// 스폰 간격(초). BP에서 튜닝
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnInterval = 1.0f;

	// 스폰 높이(Z). 플레이어보다 위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnHeight = 2000.0f;

	// 좌우 랜덤 범위(X). -Range ~ +Range 사이에서 스폰
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnRangeX = 500.0f;

	// 풀 크기. 미리 만들어둘 장애물 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 PoolSize = 20;

	// 디자인해둔 패턴들. BP에서 편집
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<FObstaclePattern> Patterns;

	// 진행도가 이 시간(초)에 도달하면 최대 난이도. BP에서 튜닝
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
	float TimeToMaxDifficulty = 60.0f;

private:
	// 타이머 핸들
	FTimerHandle SpawnTimerHandle;

	// 실제 스폰 함수
	void SpawnPattern();

	// 장애물 풀(창고)
	UPROPERTY()
	TArray<class AObstacle*> ObstaclePool;

	// 풀 초기화 함수
	void InitializePool();

	// 풀에서 비활성 장애물 하나 반환. 없으면 nullptr
	AObstacle* GetPooledObstacle();

	// 게임 시작 후 흐른 시간
	float ElapsedTime = 0.0f;

	// 진행도(0~1) 계산. 0=시작, 1=최대 난이도
	float GetProgress() const;

	// 가중치 기반 패턴 선택
	int32 PickPatternIndex() const;
};