// GameplayAbilitySystem Project Aura


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::HighLightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->CustomDepthStencilValue = CUSTOM_DEPTH_RED;
	Weapon->SetRenderCustomDepth(true);
	Weapon->CustomDepthStencilValue = CUSTOM_DEPTH_RED;
}

void AAuraEnemy::UnHighLightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);

}
