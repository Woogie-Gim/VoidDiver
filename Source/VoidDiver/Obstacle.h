#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class VOIDDIVER_API AObstacle : public AActor
{
	GENERATED_BODY()

public:
	AObstacle();

	// 풀에서 꺼내 활성화. 위치 지정 + 보이기 + Tick 켜기
	void Activate(const FVector& NewLocation);

	// 풀로 반납. 숨기기 + Tick 끄기
	void Deactivate();

	// 현재 활성 상태인지
	bool IsActive() const { return bIsActive; }

protected:
	virtual void Tick(float DeltaTime) override;

	// 장애물 외형. BP에서 메시 지정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle")
	class UStaticMeshComponent* MeshComp;

	// 아래로 흐르는 속도(cm/s). BP에서 튜닝
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float FallSpeed = 500.0f;

	// 이 높이(Z)보다 아래로 내려가면 스스로 제거
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle")
	float KillZ = -2000.0f;

	// 활성 상태 플래그
	bool bIsActive = false;

	// 오버랩 시작 시 호출. 시그니처는 언리얼이 정한 형식 고정
	UFUNCTION()
	void OnObstacleOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};