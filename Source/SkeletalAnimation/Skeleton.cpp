#include <PGN/Math/Math.h>
#include <PGN/Utilities/ResourceManager/ResourceHandle.h>
#include <string.h>
#include "Anim.h"
#include "Animation.h"
#include "Skeleton.h"
#include "SkeletonTemplate.h"

Skeleton::Skeleton(pgn::Pool* pool)
{
	this->pool = pool;
	numAnimLayers = 0;
}

void Skeleton::dispose()
{

}

void Skeleton::playAnimation(pgn::Animation* anim, int transitionTime, pgn::TransitionFunc transitionFunc, float rate, int startTime)
{
	assert(numAnimLayers < maxNumAnimLayers);
	AnimLayer* layer = &animLayers[numAnimLayers++];
	layer->anim = ((Animation*)anim)->anim;
	layer->transitionTime = transitionTime;
	layer->transitionFunc = transitionFunc;
	layer->rate = rate;
	layer->startTime = startTime;

	layer->time = 0;
}

void Skeleton::updatePose(int dt, pgn::SkeletonTemplate* _templ, pgn::Float4x3* boneMats)
{
	if (dt)
	{
		for (int i = 0; i < numAnimLayers; i++)
			animLayers[i].time += dt;
	}

	if (boneMats)
	{
		const int maxNumBones = 128;

		pgn::Float4 quat[maxNumBones];
		pgn::Float4 pos[maxNumBones];
		unsigned short rotAffectedBy[maxNumBones];
		unsigned short posAffectedBy[maxNumBones];

		SkeletonTemplate* templ = (SkeletonTemplate*)_templ;
		size_t numBones = templ->bones.size();

		memset(rotAffectedBy, 0, sizeof(rotAffectedBy[0]) * numBones);
		memset(posAffectedBy, 0, sizeof(posAffectedBy[0]) * numBones);

		for (int i = 0; i < numAnimLayers; i++)
		{
			AnimLayer* layer = &animLayers[i];
			Anim* anim = (Anim*)layer->anim->core();

			if (!anim)
			{
				rotAffectedBy[0] |= 1 << i; // 防止在动画还没加载好的时候被认为是不活跃的层。
				continue;
			}

			int dt = (int)(layer->time * layer->rate);
			int time = layer->startTime + dt;
			int duration = anim->duration;
			time = (time % duration + duration) % duration;

			float weight;

			switch (layer->transitionFunc)
			{
			case pgn::linear:
				weight = dt >= layer->transitionTime ? 1.0f : dt / (float)layer->transitionTime;
				break;
			case pgn::easeIn:
			case pgn::easeOut:
			case pgn::easeInOut:
                weight = 0.0f;
				;
			}

			auto itBoneAnim = anim->boneAnims.begin();
			for (size_t j = 0; j < numBones; j++)
			{
				BoneAnim* boneAnim = &*itBoneAnim;

				auto& rotKeyMap = boneAnim->rotKeyMap;
				if (!rotKeyMap.empty())
				{
					auto it = rotKeyMap.upper_bound(time);
					int t1 = it->first;
					pgn::Float4* b = &it->second;

					it--;
					int t0 = it->first;
					pgn::Float4* a = &it->second;

					pgn::Float4 r;
					pgn::slerp(&r, a, b, (time - t0) / (float)(t1 - t0));

					if (rotAffectedBy[j] && weight != 1.0f)
					{
						pgn::slerp(&quat[j], &quat[j], &r, weight);
						rotAffectedBy[j] |= 1 << i;
					}
					else
					{
						quat[j] = r;
						rotAffectedBy[j] = 1 << i;
					}
				}

				auto& posKeyMap = boneAnim->posKeyMap;
				if (!posKeyMap.empty())
				{
					auto it = posKeyMap.upper_bound(time);
					int t1 = it->first;
					pgn::Float4* b = &it->second;

					it--;
					int t0 = it->first;
					pgn::Float4* a = &it->second;

					pgn::Float4 r;
					pgn::lerp(&r, a, b, (time - t0) / (float)(t1 - t0));

					if (posAffectedBy[j] && weight != 1.0f)
					{
						pgn::lerp(&pos[j], &pos[j], &r, weight);
						posAffectedBy[j] |= 1 << i;
					}
					else
					{
						pos[j] = r;
						posAffectedBy[j] = 1 << i;
					}
				}

				itBoneAnim++;
			}
		}

		static pgn::Float4x3 I =
		{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
		};

		pgn::Float4x3 combinedMats[maxNumBones];
		combinedMats[0] = I;

		auto itBone = templ->bones.begin();
		itBone++;
		for (size_t i = 1; i < numBones; i++)
		{
			Bone* bone = &*itBone;

			if (!rotAffectedBy[i])
				quat[i] = bone->defaultRot;

			if (!posAffectedBy[i])
				pos[i] = bone->defaultPos;

			pgn::Float4x4 localMat;

			pgn::quatToMat(&localMat.float4x3, &quat[i]);

			localMat[0][3] = pos[i][0];
			localMat[1][3] = pos[i][1];
			localMat[2][3] = pos[i][2];

			localMat[3][0] = 0.0f;
			localMat[3][1] = 0.0f;
			localMat[3][2] = 0.0f;
			localMat[3][3] = 1.0f;

			pgn::mul(&combinedMats[i], &localMat, &combinedMats[bone->parentIndex]);
			pgn::mul(&boneMats[i], &bone->offsetMat, &combinedMats[i]);

			itBone++;
		}

		unsigned short activeLayerMask = 0;
		for (size_t i = 0; i < numBones; i++)
		{
			activeLayerMask |= rotAffectedBy[i];
			activeLayerMask |= posAffectedBy[i];
		}

		int numActiveLayers = 0;
		for (int i = 0; i < numAnimLayers; i++)
		{
			if (activeLayerMask & (1 << i))
			{
				if (numActiveLayers != i)
					animLayers[numActiveLayers] = animLayers[i];

				numActiveLayers++;
			}
		}
		numAnimLayers = numActiveLayers;
	}
}
