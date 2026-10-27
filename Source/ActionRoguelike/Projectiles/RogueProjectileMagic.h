#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RogueProjectileMagic.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueProjectileMagic : public AActor
{
	GENERATED_BODY()
	
protected:

	// Asset Niagara usado no impacto. UNiagaraSystem descreve o efeito; UNiagaraComponent é o
	// componente que executa um sistema no mundo. A referência ao asset não o reproduz automaticamente.
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UNiagaraComponent> LoopedNiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	// Permite que o sistema de reflexão da Unreal encontre este método para a associação com AddDynamic.
	// Isso não o torna automaticamente chamável em Blueprints; essa exposição requer especificadores próprios.
	UFUNCTION()
	void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	// Sobrescreve uma etapa do ciclo de vida de AActor executada após a inicialização dos componentes.
	// override faz o compilador verificar se a assinatura corresponde à função virtual da classe base.
	virtual void PostInitializeComponents() override;

	ARogueProjectileMagic();
};
