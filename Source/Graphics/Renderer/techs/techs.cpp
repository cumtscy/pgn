#include "LightIndexingTechDesc.h"
#include "PostProcessingTechDesc.h"
#include "SkinnedMeshTechDesc.h"
#include "StaticMeshTechDesc.h"
#include "techs.h"

TechDesc* techDescs[numTechs] =
{
	  lightIndexingTechDesc
	, staticMeshTechDesc
	, skinnedMeshTechDesc
	, postProcessingTechDesc
};
