
#include "RogueProjectileMagic.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"


ARogueProjectileMagic::ARogueProjectileMagic()
{
	// A esfera é o componente raiz: sua transformação posiciona o ator e ela fornece a geometria de colisão.
	// O perfil "Projectile" precisa existir nas configurações do projeto e define as respostas de colisão.
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;
	SphereComponent->SetSphereRadius(16.0f);
	SphereComponent->SetCollisionProfileName("Projectile");

	// Cria o componente que executará o efeito visual contínuo do projétil.
	// O sistema Niagara atribuído a ele determina o visual e se a emissão realmente fica em repetição.
	LoopedNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LoopedNiagaraComp"));
	// Configura a esfera como pai na hierarquia de transformações para o registro dos componentes.
	// Assim, o efeito acompanha o deslocamento do projétil; essa ligação não cria uma restrição física.
	LoopedNiagaraComponent->SetupAttachment(SphereComponent);

	// O componente de movimento atualiza o deslocamento do projétil. InitialSpeed define a velocidade
	// inicial, em unidades da Unreal por segundo (normalmente cm/s), e gravidade zero desativa a queda
	// causada pela gravidade calculada por esse componente.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	ProjectileMovementComponent->InitialSpeed = 2000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Após a inicialização dos componentes, mantém a implementação da classe base com Super e registra
// o método que receberá os impactos da esfera. Este é um método virtual sobrescrito, não um módulo.
void ARogueProjectileMagic::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// OnComponentHit é um delegado dinâmico multicast: pode notificar vários métodos associados.
	// AddDynamic registra este objeto e o ponteiro para o método, cuja declaração usa UFUNCTION.
	// Esse evento representa um impacto bloqueante conforme a configuração de colisão, não uma sobreposição (overlap).
	SphereComponent->OnComponentHit.AddDynamic(this, &ARogueProjectileMagic::OnActorHit);

	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

// Apesar do nome OnActorHit, este método está associado ao evento de componente OnComponentHit.
// HitComponent é o componente deste ator atingido; OtherActor e OtherComp identificam o outro lado.
// Hit contém os dados do impacto, recebidos por referência constante para evitar uma cópia.
// NormalImpulse representa o impulso de uma colisão física e pode ser zero em impactos por varredura (sweep).
void ARogueProjectileMagic::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// TSubclassOf<UDamageType> guarda uma referência de classe (UClass) restrita a UDamageType
	// ou a uma de suas subclasses. Não é uma instância de dano nem um componente.
	// StaticClass() obtém a classe UDamageType da reflexão da Unreal, sem criar um objeto desse tipo.
	TSubclassOf<UDamageType> DmgTypeClass = UDamageType::StaticClass();
	// Envia dano genérico ao outro ator: 1.0f é o dano base, o Controller do instigador identifica
	// o responsável, e this indica que o projétil é o causador direto. A reação e qualquer redução
	// de vida dependem da implementação do ator atingido; ApplyDamage não fornece um sistema de vida pronto.
	UGameplayStatics::ApplyDamage(OtherActor, 1.0f, GetInstigatorController(), this, DmgTypeClass);

	// Solicita uma instância do efeito na posição atual do ator, que pode diferir do ponto de impacto.
	// Aqui this fornece o contexto do mundo; o efeito não é anexado ao projétil por essa chamada.
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ExplosionEffect, GetActorLocation());

	// Solicita a destruição deste ator (o projétil), não da função nem do ator atingido.
	// A destruição de AActor é diferida; a liberação de sua memória cabe posteriormente ao coletor de lixo.
	// Destroy() não equivale a delete this e não interrompe automaticamente a execução do método.
	Destroy();
}