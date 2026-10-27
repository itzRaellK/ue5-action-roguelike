// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionRoguelike : ModuleRules
{
	public ActionRoguelike(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		// Estes são módulos reais da Unreal: agrupam código e participam da compilação e da ligação.
		// Dependências públicas também ficam disponíveis aos módulos que dependem de ActionRoguelike.
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		// Dependências privadas ficam disponíveis para compilar este módulo, sem propagação pública.
		// EnhancedInput fornece o sistema de ações de entrada; Niagara fornece os efeitos visuais usados no projétil.
		PrivateDependencyModuleNames.AddRange(new string[] { "EnhancedInput", "Niagara" });
	}
}
