#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <learnopengl/humanoid_const.h>

const float EPSILON = 1e-6f;
const float PI_HALF = (float)1.57079632679489661923f;

struct SkeletonLimit {
	glm::vec3 mMin;
	glm::vec3 mMax;
	glm::vec3 mValue;
	float mLength;
};

struct Limit
{
	glm::vec3 mMin;
	glm::vec3 mMax;
};

struct HumanBone {
	std::string mBoneName;
	std::string mHumanName;
	Limit mLimit;
};

class HumanDescription
{
public:
	HumanDescription() {};
	~HumanDescription() {};

	std::vector<HumanBone> mHuman;
private:
};


struct Axes {
	glm::fquat mPreQ;
	glm::fquat mPostQ;
	glm::vec3 mSgn;
	Limit mLimit;
	float mLength;
	//ZYRoll
};

static inline glm::vec3 Quat2Qtan(const glm::fquat& q)
{
	float w = q.w;
	w = w == 0.f ? EPSILON : w;
	return glm::vec3(q.x / w, q.y / w, q.z / w);
}

static inline glm::vec3 Quat2ZYRoll(const glm::fquat& q)
{
	const glm::vec3 qtan = Quat2Qtan(q);
	const float qtanx = qtan.x;
	const float x2p1 = 1.f + qtanx * qtanx;
	return glm::vec3(qtanx, (qtan.y - qtan.x * qtan.z) / x2p1, (qtan.z + qtanx * qtan.y) / x2p1);
}

static inline glm::fquat ZYRoll2Quat(const glm::vec3 zyroll)
{
	return glm::normalize(glm::fquat(1.0f, zyroll.x, zyroll.y + zyroll.x * zyroll.z, zyroll.z - zyroll.x * zyroll.y));
}

static inline glm::vec3 LimitProject(const Limit& l, const glm::vec3& v)
{
	glm::vec3 res;
	for (int i = 0; i < 3; i++)
	{
		res[i] = v[i] < 0 ? (l.mMin[i] < 0 ? -v[i] / l.mMin[i] : (l.mMin[i] > 0 ? v[i] : 0))
			: (l.mMax[i] > 0 ? v[i] / l.mMax[i] : (l.mMax[i] < 0 ? v[i] : 0));
	}
	return res;
}

//v:[0,1]
static inline glm::vec3 LimitUnproject(const Limit& l, const glm::vec3 v)
{
	glm::vec3 res;
	for (int i = 0; i < 3; i++)
	{
		res[i] = v[i] < 0 ? (l.mMin[i] < 0 ? -v[i] * l.mMin[i] : (l.mMin[i] > 0 ? v[i] : 0))
			: (l.mMax[i] > 0 ? v[i] * l.mMax[i] : (l.mMax[i] < 0 ? v[i] : 0));
	}
	return res;
}

static inline glm::fquat AxesProject(const Axes& a, const glm::fquat& q)
{
	return glm::normalize(glm::conjugate(a.mPreQ) * (q * a.mPostQ));
}

//q: bone muscle axes rotation   ->  localTransform
static inline glm::fquat AxesUnproject(const Axes& a, const glm::fquat& q)
{
	return glm::normalize(a.mPreQ * (q * glm::conjugate(a.mPostQ)));
}

static inline glm::vec3 ToAxes(const Axes& a, const glm::fquat& q)
{
	const glm::fquat qp = AxesProject(a, q);
	glm::vec3 xyz = LimitProject(a.mLimit, 2.f * glm::atan(Quat2ZYRoll(qp) * glm::sign(a.mSgn)));
	return xyz;
}

static inline glm::fquat FromAxes(const Axes& a, const glm::vec3& uvw)
{   
	glm::vec3 halfTanIn = LimitUnproject(a.mLimit, uvw);
	glm::vec3 halfTanRes = glm::tan(glm::clamp(0.5f * halfTanIn, -PI_HALF + EPSILON, PI_HALF - EPSILON));
	glm::fquat q= ZYRoll2Quat(halfTanRes * glm::sign(a.mSgn));
	return AxesUnproject(a, q);
}

struct HumanNodeData
{
	AssimpNodeData* mNode;
	int mParentId = -1;//parentId in vector
	int mBoneIndex = -1;//boneId in bone vector
	int mHumanIndex = -1;
};

class Human
{
public:
	Human()
	{
		memset(mHumanBoneIndex, -1, sizeof(int) * Bones::LastBone);
		for (int i = 0; i < Bones::LastBone; i++)
		{
			mHumanBoneMass[i] = HumanBoneDefaultMass[i];
		}
		memset(mLeftHand.mHandBoneIndex, -1, sizeof(int) * hBoneCount);
		memset(mRightHand.mHandBoneIndex, -1, sizeof(int) * hBoneCount);
	}

	~Human() = default;

	int mHumanBoneIndex[Bones::LastBone];//bone id in mBones
	float mHumanBoneMass[Bones::LastBone];//Adjust bone mass while missing bone



	Hand mLeftHand;
	Hand mRightHand;

	std::vector<HumanNodeData*> mHumanSkeleton;//valid node; //hand+body?
	std::vector<glm::mat4> pose;
	std::vector<glm::mat4> poseWS;
	std::vector<Axes> mAxesArray;
};


class HumanoidHelp
{
public:
	HumanoidHelp(const std::vector<Bone>& bones, const std::vector<AssimpNodeData*>& nodeDatas, AssimpNodeData* rootNode)
	{
		mBones = bones;
		mRootNode = rootNode;
		mNodeDatas = nodeDatas;
		mHumanBoneIds.resize(bones.size(), -1);

		//body+hand
		for (int i = 0; i < nodeDatas.size(); i++) {
			HumanNodeData temp;
			temp.mNode = nodeDatas[i];
			temp.mParentId = -1;
			bool find = false;
			for (int j = 0; j < nodeDatas.size(); j++)
			{
				for (auto& child : nodeDatas[j]->children)
				{
					if (child.name == temp.mNode->name)
					{
						temp.mParentId = j;
						find = true;
						break;
					}
				}
				if (find)
					break;
			}
			auto iter = std::find_if(bones.begin(), bones.end(), [&](const Bone& bone) {return bone.GetBoneName() == temp.mNode->name; });
			if (iter != bones.end())
				temp.mBoneIndex = iter - bones.begin();
			mHumanoidSkeleton.push_back(std::move(temp));
		}

		//Initialize humandescription
		Intialize();
		//Build Human
		BuildHuman();
		//Simulate
		ApplyZeroStateWithMuscle();

	};
	~HumanoidHelp() {
	};
	
	void SetHumanDescription(HumanDescription&& hd)
	{
		mHumanDescription = std::move(hd);
	}

	void SetSkeletonBones(const std::vector<Bone>& bones)
	{
		mBones = bones;
	}

	//Init HD
	void Intialize() 
	{
		//body
		mHumanDescription.mHuman.push_back(HumanBone{ "Hips","Hips",Limit{glm::vec3{-40, -40, -40}, glm::vec3{40, 40, 40},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "Spine","Spine",Limit{glm::vec3{-40, -40, -40}, glm::vec3{40, 40, 40},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "Spine1","Chest",Limit{glm::vec3{-40, -40, -40}, glm::vec3{40, 40, 40},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "Spine2","UpperChest",Limit{glm::vec3{-20, -20, -20}, glm::vec3{20, 20, 20},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "Neck","Neck",Limit{glm::vec3{-40, -40, -40}, glm::vec3{40, 40, 40},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "Head","Head",Limit{glm::vec3{-40, -40, -40}, glm::vec3{40, 40, 40},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftUpLeg","LeftUpperLeg",Limit{glm::vec3{-60, -60, -90}, glm::vec3{60, 60, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightUpLeg","RightUpperLeg",Limit{glm::vec3{-60, -60, -90}, glm::vec3{60, 60, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftLeg","LeftLowerLeg",Limit{glm::vec3{-90, 0, -80}, glm::vec3{90, 0, 80},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightLeg","RightLowerLeg",Limit{glm::vec3{-90, 0, -80}, glm::vec3{90, 0, 80},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftFoot","LeftFoot",Limit{glm::vec3{0, -30, -50}, glm::vec3{0, 30, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightFoot","RightFoot",Limit{glm::vec3{0, -30, -50}, glm::vec3{0, 30, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftShoulder","LeftShoulder",Limit{glm::vec3{0, -15, -15}, glm::vec3{0, 15, 30},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightShoulder","RightShoulder",Limit{glm::vec3{0, -15, -15}, glm::vec3{0, 15, 30},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftArm","LeftUpperArm",Limit{glm::vec3{-90, -100, -60}, glm::vec3{90, 100, 100},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightArm","RightUpperArm",Limit{glm::vec3{-90, -100, -60}, glm::vec3{90, 100, 100},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftForeArm","LeftLowerArm",Limit{glm::vec3{-90, 0, -80}, glm::vec3{90, 0, 80},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightForeArm","RightLowerArm",Limit{glm::vec3{-90, 0, -80}, glm::vec3{90, 0, 80},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHand","LeftHand",Limit{glm::vec3{0, -40, -80}, glm::vec3{0, 40, 80},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHand","RightHand",Limit{glm::vec3{0, -40, -80}, glm::vec3{0, 40, 80},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftToeBase","LeftToes",Limit{glm::vec3{0, 0, -50}, glm::vec3{0, 0, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightToeBase","RightToes",Limit{glm::vec3{0, 0, -50}, glm::vec3{0, 0, 50},} });

		//LeftHand
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandThumb1","LeftThumbProximal",Limit{glm::vec3{0, -25, -20}, glm::vec3{0, 25, 20},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandThumb2","LeftThumbIntermediate",Limit{glm::vec3{0, 0, -40}, glm::vec3{0, 0, 35},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandThumb3","LeftThumbDistal",Limit{glm::vec3{0, 0, -40}, glm::vec3{0, 0, 35},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandIndex1","LeftIndexProximal",Limit{glm::vec3{0, -20, -50}, glm::vec3{0, 20, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandIndex2","LeftIndexIntermediate",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandIndex3","LeftIndexDistal",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandMiddle1","LeftMiddleProximal",Limit{glm::vec3{0, -7.5f, -50}, glm::vec3{0, 7.5f, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandMiddle2","LeftMiddleIntermediate",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandMiddle3","LeftMiddleDistal",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandRing1","LeftRingProximal",Limit{glm::vec3{0, -7.5f, -50}, glm::vec3{0, 7.5f, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandRing2","LeftRingIntermediate",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandRing3","LeftRingDistal",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandPinky1","LeftLittleProximal",Limit{glm::vec3{0, -20, -50}, glm::vec3{0, 20, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandPinky2","LeftLittleIntermediate",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "LeftHandPinky3","LeftLittleDistal",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		//RightHand
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandThumb1","RightThumbProximal",Limit{glm::vec3{0, -25, -20}, glm::vec3{0, 25, 20},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandThumb2","RightThumbIntermediate",Limit{glm::vec3{0, 0, -40}, glm::vec3{0, 0, 35},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandThumb3","RightThumbDistal",Limit{glm::vec3{0, 0, -40}, glm::vec3{0, 0, 35},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandIndex1","RightIndexProximal",Limit{glm::vec3{0, -20, -50}, glm::vec3{0, 20, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandIndex2","RightIndexIntermediate",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandIndex3","RightIndexDistal",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandMiddle1","RightMiddleProximal",Limit{glm::vec3{0, -7.5f, -50}, glm::vec3{0, 7.5f, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandMiddle2","RightMiddleIntermediate",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandMiddle3","RightMiddleDistal",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandRing1","RightRingProximal",Limit{glm::vec3{0, -7.5f, -50}, glm::vec3{0, 7.5f, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandRing2","RightRingIntermediate",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandRing3","RightRingDistal",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandPinky1","RightLittleProximal",Limit{glm::vec3{0, -20, -50}, glm::vec3{0, 20, 50},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandPinky2","RightLittleIntermediate",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
		mHumanDescription.mHuman.push_back(HumanBone{ "RightHandPinky3","RightLittleDistal",Limit{glm::vec3{0, 0, -45}, glm::vec3{0, 0, 45},} });
	}

	//With mBones infos, construct HumanInfo
	void BuildHuman()
	{
		//Body
		for (int i = 0; i < Bones::LastBone; i++)
		{
			auto iter = std::find_if(mHumanDescription.mHuman.begin(), mHumanDescription.mHuman.end(), [&](const HumanBone& bone) {return bone.mHumanName == HumanBoneName[i]; });
			if (iter != mHumanDescription.mHuman.end())
			{
				std::string boneName = iter->mBoneName;
				auto boneIter = std::find_if(mBones.begin(), mBones.end(), [&](const Bone& bone) {return bone.GetBoneName() == boneName; });
				if (boneIter != mBones.end())
				{
					int boneId = boneIter - mBones.begin();
					mHuman.mHumanBoneIndex[i] = boneId;
					mHumanBoneIds[boneId] = i;
				}
				else
				{
					std::cout << "Error: Not find bone: " << boneName << "\n";
				}
			}
		}

		//Hand
		auto BuildHand = [&](std::string handFlag)
		{
				auto hand = &mHuman.mLeftHand;
				if (handFlag == "Right")
				{
					hand = &mHuman.mRightHand;
					hand->isLeft = false;
				}
				for (int i = 0; i < hBoneCount; i++)
				{
					auto iter = std::find_if(mHumanDescription.mHuman.begin(), mHumanDescription.mHuman.end(), [&](const HumanBone& bone) {return bone.mHumanName == (handFlag + HandBoneName[i]); });
					if (iter != mHumanDescription.mHuman.end())
					{
						std::string boneName = iter->mBoneName;
						auto boneIter = std::find_if(mBones.begin(), mBones.end(), [&](const Bone& bone) {return bone.GetBoneName() == boneName; });
						if (boneIter != mBones.end())
						{
							int boneId = boneIter - mBones.begin();
							hand->mHandBoneIndex[i] = boneId;
							//mHumanBoneIds[boneId] = i;//dont deal
						}
						else
						{
							std::cout << "Error: Not find bone: "<<boneName<<"\n";
						}
					}
				}

		};

		//LeftHand
		BuildHand("Left");
		//RightHand
		BuildHand("Right");

		//MARK mHumanSkeleton
		for (int i = 0; i < mHumanoidSkeleton.size(); i++)
		{
			int boneId = mHumanoidSkeleton[i].mBoneIndex;
			if (mHumanBoneIds[boneId] != -1)//isHumanBone
			{
				mHumanoidSkeleton[i].mHumanIndex = mHumanBoneIds[boneId];
				mHuman.mHumanSkeleton.push_back(&mHumanoidSkeleton[i]);
			}
		}
		//Calculate TransformMatrix
		int humanBoneNum = mHuman.mHumanSkeleton.size();
		mHuman.poseWS.resize(humanBoneNum);
		for (int i = 0; i < humanBoneNum; i++)
		{
			mHuman.pose.push_back(mHuman.mHumanSkeleton[i]->mNode->transformation);
			if (mHuman.mHumanSkeleton[i]->mParentId == -1)
			{
				mHuman.poseWS[i] = mHuman.pose[i];
			}
			else
			{
				mHuman.poseWS[i] = mHuman.poseWS[mHuman.mHumanSkeleton[i]->mParentId] * mHuman.pose[i];
			}
		}
		//SetupHumanAxes
		mHuman.mAxesArray.resize(humanBoneNum);
		auto FindIdInSkeleton = [&](int boneId) {
			for (int i = 0; i < humanBoneNum; i++)
			{
				if (mHuman.mHumanSkeleton[i]->mBoneIndex == boneId)
					return i;
			}
			return -1;
		};
		for (int i = 0; i < humanBoneNum; i++)
		{
			int skAxisBoneId = -1;//存parentNode的id
			float len = 1.0f;
			int humanIdTem = mHuman.mHumanSkeleton[i]->mHumanIndex;
			//calculate len and skAxisBoneId; but maybe not used by forcedAxis
			switch (humanIdTem)
			{
			case LeftEye:
			case RightEye:
			case Jaw:
				len = 0.1f;
				break;

			case Head:
				if (mHuman.mHumanBoneIndex[Neck] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Neck]);
					len = -1.0f;
				}
				else if (mHuman.mHumanBoneIndex[UpperChest] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[UpperChest]);
					len = -0.75f;
				}
				else if (mHuman.mHumanBoneIndex[Chest] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Chest]);
					len = -0.5f;
				}
				else
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Spine]);
					len = -0.25f;
				}
				break;

			case LeftFoot:
				len = -mHuman.poseWS[i][3][1];// .GetPosition().y;
				break;

			case RightFoot:
				len = -mHuman.poseWS[i][3][1];
				break;

			case LeftHand:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[LeftLowerArm]);
				len = -0.5f;
				break;

			case RightHand:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[RightLowerArm]);
				len = -0.5f;
				break;

			case LeftToes:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[LeftFoot]);
				len = 0.5f;
				break;

			case RightToes:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[RightFoot]);
				len = 0.5f;
				break;

			case Hips:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Spine]);
				break;

			case LeftUpperLeg:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[LeftLowerLeg]);
				break;

			case RightUpperLeg:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[RightLowerLeg]);
				break;

			case LeftLowerLeg:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[LeftFoot]);
				break;

			case RightLowerLeg:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[RightFoot]);
				break;

			case Spine:
				if (mHuman.mHumanBoneIndex[Chest] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Chest]);
				}
				else if (mHuman.mHumanBoneIndex[UpperChest] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[UpperChest]);
				}
				else if (mHuman.mHumanBoneIndex[Neck] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Neck]);
				}
				else
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Head]);
				}
				break;

			case Chest:
				if (mHuman.mHumanBoneIndex[UpperChest] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[UpperChest]);
				}
				else if (mHuman.mHumanBoneIndex[Neck] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Neck]);
				}
				else
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Head]);
				}
				break;

			case UpperChest:
				if (mHuman.mHumanBoneIndex[Neck] >= 0)
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Neck]);
				}
				else
				{
					skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Head]);
				}
				break;

			case Neck:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[Head]);
				break;

			case LeftShoulder:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[LeftUpperArm]);
				break;

			case RightShoulder:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[RightUpperArm]);
				break;

			case LeftUpperArm:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[LeftLowerArm]);
				break;

			case RightUpperArm:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[RightLowerArm]);
				break;

			case LeftLowerArm:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[LeftHand]);
				break;

			case RightLowerArm:
				skAxisBoneId = FindIdInSkeleton(mHuman.mHumanBoneIndex[RightHand]);
				break;
			}

			auto& axes = mHuman.mAxesArray[i];
			SetupAxesInfo axesInfo = bodySetupAxesInfoArray[humanIdTem];
			bool isLeft = true;//use when hand, body always true
			axes.mLimit.mMin = glm::radians(glm::vec3(axesInfo.mMin[0], axesInfo.mMin[1], axesInfo.mMin[2]));
			axes.mLimit.mMax = glm::radians(glm::vec3(axesInfo.mMax[0], axesInfo.mMax[1], axesInfo.mMax[2]));
			axes.mSgn = glm::vec3(axesInfo.mSgn[0], axesInfo.mSgn[1], axesInfo.mSgn[2]) * (isLeft? glm::vec3(1,1,1):glm::vec3(-1,1,-1));
			glm::vec3 mainAxis = glm::vec3(axesInfo.mMainAxis[0], axesInfo.mMainAxis[1], axesInfo.mMainAxis[2]);
			glm::fquat zeroQ = glm::fquat(/*wxyz*/axesInfo.mPreQ[3], axesInfo.mPreQ[0], axesInfo.mPreQ[1], axesInfo.mPreQ[2]);
			axes.mLength = 1.f;
			glm::vec3 u(1, 0, 0);
			glm::vec3 v(0, 1, 0);
			glm::vec3 w(0, 0, 1);
			if (skAxisBoneId != -1)
			{
				glm::mat parentX = mHuman.poseWS[skAxisBoneId];
				glm::mat boneX = mHuman.poseWS[i];
				u = glm::normalize((glm::vec3(parentX[3][0], parentX[3][1], parentX[3][2]) - glm::vec3(boneX[3][0], boneX[3][1], boneX[3][2]))*len);
				w = mainAxis;
				v = glm::normalize(glm::cross(w, u));
				w = glm::cross(u, v);
				axes.mLength = glm::length(glm::vec3(parentX[3][0], parentX[3][1], parentX[3][2]) - glm::vec3(boneX[3][0], boneX[3][1], boneX[3][2]));
			}
			if (axesInfo.mForceAxis)
			{
				switch (axesInfo.mForceAxis)
				{
				case +1:
					u = glm::vec3(+1, 0, 0);
					break;
				case -1:
					u = glm::vec3(-1, 0, 0);
					break;
				case +2:
					u = glm::vec3(0, +1, 0);
					break;
				case -2:
					u = glm::vec3(0, -1, 0);
					break;
				case +3:
					u = glm::vec3(0, 0, +1);
					break;
				default:
					u = glm::vec3(0, 0, -1);
					break;
				}
				w = mainAxis;
				v = glm::normalize(glm::cross(w, u));
				w = glm::cross(u, v);
			}
			axes.mLength *= len > 0 ? len : -len;

			int parentId = mHuman.mHumanSkeleton[i]->mParentId;
			glm::fquat parentQ = parentId != -1 ? glm::quat_cast(mHuman.poseWS[parentId]) : glm::fquat(1, 0, 0, 0);
			glm::mat3 pre; pre[0] = u; pre[1] = v; pre[2] = w;
			glm::fquat preQ = glm::quat_cast(pre);
			axes.mPostQ = glm::normalize(glm::conjugate(glm::quat_cast(mHuman.poseWS[i])) * preQ);
			axes.mPreQ = glm::normalize(glm::conjugate(parentQ) * (zeroQ * preQ));
		}//END LOOP
		
	}

	//zero state->bone
	void ApplyZeroStateWithMuscle()
	{
		int humanBoneNum = mHuman.mHumanSkeleton.size();
		auto SetLocalPose = [&](int humanBoneId, glm::vec3 dof) {
			glm::fquat q = FromAxes(mHuman.mAxesArray[humanBoneId], dof);
			glm::vec4 oldPos = mHuman.pose[humanBoneId][3];
			glm::mat4 transformMat = glm::identity<glm::mat4>();
			transformMat[3] = oldPos;
			glm::fquat oldQ = glm::quat_cast(mHuman.pose[humanBoneId]);
			mHuman.pose[humanBoneId] = transformMat * glm::mat4_cast(q);
			};
		for (int i = 0; i < humanBoneNum; i++)
		{
			/*Bone2DoF[i][2]
			Bone2DoF[i][1]
			Bone2DoF[i][0]*/
			glm::vec3 xyz(0, 0, 0);
			SetLocalPose(i, xyz);
		}

	}

	const Human& GetHuman() { return mHuman; }
private:
	HumanDescription mHumanDescription;
	std::vector<Bone> mBones;
	std::vector<int> mHumanBoneIds;

	std::vector<AssimpNodeData*> mNodeDatas;//m_Bones related node
	AssimpNodeData* mRootNode;//BONE HIERARCHY
	std::vector<HumanNodeData> mHumanoidSkeleton;//body+hand

	Human mHuman;

};