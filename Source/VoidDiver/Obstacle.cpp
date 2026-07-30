#include "Obstacle.h"
#include "Components/StaticMeshComponent.h"

AObstacle::AObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시를 루트로 생성
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);
}

void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 매 프레임 아래로 이동. DeltaTime 곱해 프레임 독립적으로
	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= FallSpeed * DeltaTime;
	SetActorLocation(NewLocation);

	// 너무 아래로 가면 제거
	if (NewLocation.Z < KillZ)
	{
		Deactivate();
	}
}

void AObstacle::Activate(const FVector& NewLocation)
{
	bIsActive = true;
	SetActorLocation(NewLocation);
	SetActorHiddenInGame(false);   // 화면에 보이게
	SetActorEnableCollision(true); // 충돌 켜기 (나중에 충돌 판정용)
	SetActorTickEnabled(true);     // Tick(하강) 켜기
}

void AObstacle::Deactivate()
{
	bIsActive = false;
	SetActorHiddenInGame(true);    // 화면에서 숨김
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);    // Tick 끄기 (성능 절약)
}