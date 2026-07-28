
#include "DiverGameMode.h"
#include "DiverCharacter.h"

ADiverGameMode::ADiverGameMode()
{
	// 기본 스폰 폰을 DiverCharacter로 지정
	DefaultPawnClass = ADiverCharacter::StaticClass();
}
