#pragma once

#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>

//Body exclude hand
enum Bones :uint8_t
{
    Hips = 0,
    LeftUpperLeg = 1,
    RightUpperLeg = 2,
    LeftLowerLeg = 3,
    RightLowerLeg = 4,
    LeftFoot = 5,
    RightFoot = 6,
    Spine = 7,
    Chest = 8,
    Neck = 9,
    Head = 10,
    LeftShoulder = 11,
    RightShoulder = 12,
    LeftUpperArm = 13,
    RightUpperArm = 14,
    LeftLowerArm = 15,
    RightLowerArm = 16,
    LeftHand = 17,
    RightHand = 18,
    LeftToes = 19,
    RightToes = 20,
    LeftEye = 21,
    RightEye = 22,
    Jaw = 23,
    UpperChest = 24,
    LastBone = 25
};


static const std::string HumanBoneName[LastBone] =
{
    "Hips",
    "LeftUpperLeg",
    "RightUpperLeg",
    "LeftLowerLeg",
    "RightLowerLeg",
    "LeftFoot",
    "RightFoot",
    "Spine",
    "Chest",
    "Neck",
    "Head",
    "LeftShoulder",
    "RightShoulder",
    "LeftUpperArm",
    "RightUpperArm",
    "LeftLowerArm",
    "RightLowerArm",
    "LeftHand",
    "RightHand",
    "LeftToes",
    "RightToes",
    "LeftEye",
    "RightEye",
    "Jaw",
    "UpperChest"
};


enum BodyDoF : uint8_t
{
    SpineFrontBack = 0,//w
    SpineLeftRight = 1,//v
    SpineRollLeftRight = 2,//u
    ChestFrontBack = 3,
    ChestLeftRight = 4,
    ChestRollLeftRight = 5,
    UpperChestFrontBack = 6,
    UpperChestLeftRight = 7,
    UpperChestRollLeftRight = 8,
    LastBodyDoF = 9
};

enum HeadDoF : uint8_t
{
    NeckFrontBack = 0,
    NeckLeftRight = 1,
    NeckRollLeftRight = 2,
    HeadFrontBack = 3,
    HeadLeftRight = 4,
    HeadRollLeftRight = 5,
    LeftEyeDownUp = 6,
    LeftEyeLeftRight = 7,
    RightEyeDownUp = 8,
    RightEyeLeftRight = 9,
    JawDownUp = 10,
    JawLeftRight = 11,
    LastHeadDoF = 12
};

enum LegDoF : uint8_t
{
    UpperLegFrontBack = 0,
    UpperLegInOut = 1,
    UpperLegRollInOut = 2,
    LegCloseOpen = 3,
    LegRollInOut = 4,
    FootCloseOpen = 5,
    FootInOut = 6,
    ToesUpDown = 7,
    LastLegDoF = 8
};

enum ArmDoF : uint8_t
{
    ShoulderDownUp = 0,
    ShoulderFrontBack = 1,
    ArmDownUp = 2,
    ArmFrontBack = 3,
    ArmRollInOut = 4,
    ForeArmCloseOpen = 5,
    ForeArmRollInOut = 6,
    HandDownUp = 7,
    HandInOut = 8,
    LastArmDoF = 9
};

enum DoF : uint8_t
{
    BodyDoFStart = 0,
    HeadDoFStart = BodyDoFStart + BodyDoF::LastBodyDoF,//9
    LeftLegDoFStart = HeadDoFStart + HeadDoF::LastHeadDoF,//21
    RightLegDoFStart = LeftLegDoFStart + LegDoF::LastLegDoF,//29
    LeftArmDoFStart = RightLegDoFStart + LegDoF::LastLegDoF,//37
    RightArmDoFStart = LeftArmDoFStart + ArmDoF::LastArmDoF,//46
    LastDoF = RightArmDoFStart + ArmDoF::LastArmDoF//55
};

static const int Bone2DoF[LastBone][3] = {
    {-1, -1, -1},  // Hips
    {LeftLegDoFStart + UpperLegFrontBack,
     LeftLegDoFStart + UpperLegInOut,
     LeftLegDoFStart + UpperLegRollInOut},  // LeftUpperLeg
    {RightLegDoFStart + UpperLegFrontBack,
     RightLegDoFStart + UpperLegInOut,
     RightLegDoFStart + UpperLegRollInOut},                                                             // RightUpperLeg
    {LeftLegDoFStart + LegCloseOpen, -1, LeftLegDoFStart + LegRollInOut},                               // kLeftLeg
    {RightLegDoFStart + LegCloseOpen, -1, RightLegDoFStart + LegRollInOut},                             // kRightLeg
    {LeftLegDoFStart + FootCloseOpen, LeftLegDoFStart + FootInOut, -1},                                 // LeftFoot
    {RightLegDoFStart + FootCloseOpen, RightLegDoFStart + FootInOut, -1},                               // RightFoot
    {BodyDoFStart + SpineFrontBack, BodyDoFStart + SpineLeftRight, BodyDoFStart + SpineRollLeftRight},  // Spine
    {BodyDoFStart + ChestFrontBack, BodyDoFStart + ChestLeftRight, BodyDoFStart + ChestRollLeftRight},  // Chest
    {HeadDoFStart + NeckFrontBack, HeadDoFStart + NeckLeftRight, HeadDoFStart + NeckRollLeftRight},     // Neck
    {HeadDoFStart + HeadFrontBack, HeadDoFStart + HeadLeftRight, HeadDoFStart + HeadRollLeftRight},     // Head
    {LeftArmDoFStart + ShoulderDownUp, LeftArmDoFStart + ShoulderFrontBack, -1},                        // LeftShoulder
    {RightArmDoFStart + ShoulderDownUp, RightArmDoFStart + ShoulderFrontBack, -1},                      // RightShoulder
    {LeftArmDoFStart + ArmDownUp, LeftArmDoFStart + ArmFrontBack, LeftArmDoFStart + ArmRollInOut},      // kLeftArm
    {RightArmDoFStart + ArmDownUp, RightArmDoFStart + ArmFrontBack, RightArmDoFStart + ArmRollInOut},   // kRightArm
    {LeftArmDoFStart + ForeArmCloseOpen, -1, LeftArmDoFStart + ForeArmRollInOut},                       // kLeftForeArm
    {RightArmDoFStart + ForeArmCloseOpen, -1, RightArmDoFStart + ForeArmRollInOut},                     // kRightForeArm
    {LeftArmDoFStart + HandDownUp, LeftArmDoFStart + HandInOut, -1},                                    // LeftHand
    {RightArmDoFStart + HandDownUp, RightArmDoFStart + HandInOut, -1},                                  // RightHand
    {LeftLegDoFStart + ToesUpDown, -1, -1},                                                             // LeftToes
    {RightLegDoFStart + ToesUpDown, -1, -1},                                                            // RightToes
    {HeadDoFStart + LeftEyeDownUp, HeadDoFStart + LeftEyeLeftRight, -1},                                // LeftEye
    {HeadDoFStart + RightEyeDownUp, HeadDoFStart + RightEyeLeftRight, -1},                              // RightEye
    {HeadDoFStart + JawDownUp, HeadDoFStart + JawLeftRight, -1},                                        // Jaw
    {BodyDoFStart + UpperChestFrontBack,
     BodyDoFStart + UpperChestLeftRight,
     BodyDoFStart + UpperChestRollLeftRight},  // UpperChest
};

static const float HumanBoneDefaultMass[LastBone] = {
    12.0f,  // Hips
    10.0f,  // LeftUpperLeg
    10.0f,  // RightUpperLeg
    4.0f,   // LeftLowerLeg
    4.0f,   // RightLowerLeg
    0.8f,   // LeftFoot
    0.8f,   // RightFoot
    2.5f,   // Spine
    12.0f,  // Chest
    1.0f,   // Neck
    4.0f,   // Head
    0.5f,   // LeftShoulder
    0.5f,   // RightShoulder
    2.0f,   // LeftUpperArm
    2.0f,   // RightUpperArm
    1.5f,   // LeftLowerArm
    1.5f,   // RightLowerArm
    0.5f,   // LeftHand
    0.5f,   // RightHand
    0.2f,   // LeftToes
    0.2f,   // RightToes
    0.0f,   // LeftEye
    0.0f,   // RightEye
    0.0f,   // Jaw
    12.0f   // UpperChest
};

static bool requiredBone[LastBone] = {
        true,   //Hips
        true,   //LeftUpperLeg
        true,   //RightUpperLeg
        true,   //LeftLowerLeg
        true,   //RightLowerLeg
        true,   //LeftFoot
        true,   //RightFoot
        true,   //Spine
        false,  //Chest
        false,  //Neck
        true,   //Head
        false,  //LeftShoulder
        false,  //RightShoulder
        true,   //LeftUpperArm
        true,   //RightUpperArm
        true,   //LeftLowerArm
        true,   //RightLowerArm
        true,   //LeftHand
        true,   //RightHand
        false,  //LeftToes
        false,  //RightToes
        false,  //LeftEye,
        false,  //RightEye,
        false,  //Jaw,
        false   //UpperChest
};

struct SetupAxesInfo
{
    float mPreQ[4];
    float mMainAxis[4];
    float mMin[3];
    float mMax[3];
    float mSgn[4];
    //AxesType:ZYRoll
    int mForceAxis;
};

const static SetupAxesInfo bodySetupAxesInfoArray[LastBone] = {
        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 0},  // Hips,
        {{-0.268f, 0, 0, 1},
         {1, 0, 0, 0},
         {-60, -60, -90},
         {60, 60, 50},
         {1, 1, 1, 1},
        /* AxesType::ZYRoll,*/
         0},  // LeftUpperLeg,
        {{-0.268f, 0, 0, 1},
         {1, 0, 0, 0},
         {-60, -60, -90},
         {60, 60, 50},
         {-1, -1, 1, 1},
        /* AxesType::ZYRoll,*/
         0},  // RightUpperLeg,
        {{0.839f, 0, 0, 1}, {1, 0, 0, 0}, {-90, 0, -80}, {90, 0, 80}, {1, 1, -1, 1},/* AxesType::ZYRoll,*/ 0},  // kLeftLeg,
        {{0.839f, 0, 0, 1},
         {1, 0, 0, 0},
         {-90, 0, -80},
         {90, 0, 80},
         {-1, 1, -1, 1},
        /* AxesType::ZYRoll,*/
         0},                                                                                              // kRightLeg,
        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, -30, -50}, {0, 30, 50}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ -2},     // LeftFoot,
        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, -30, -50}, {0, 30, 50}, {1, -1, 1, 1},/* AxesType::ZYRoll,*/ -2},    // RightFoot,
        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 0},  // Spine,
        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 0},  // Chest,
        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 0},  // Neck,
        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 2},  // Head,
        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, -15, -15}, {0, 15, 30}, {1, 1, -1, 1},/* AxesType::ZYRoll,*/ 0},  // LeftShoulder,
        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, -15, -15}, {0, 15, 30}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 0},   // RightShoulder,
        {{0, 0.268f, 0.364f, 1},
         {0, 0, 1, 0},
         {-90, -100, -60},
         {90, 100, 100},
         {1, 1, -1, 1},
        /* AxesType::ZYRoll,*/
         0},  // kLeftArm,
        {{0, -0.268f, -0.364f, 1},
         {0, 0, 1, 0},
         {-90, -100, -60},
         {90, 100, 100},
         {-1, 1, 1, 1},
        /* AxesType::ZYRoll,*/
         0},  // kRightArm,
        {{0, 0.839f, 0, 1},
         {0, 1, 0, 0},
         {-90, 0, -80},
         {90, 0, 80},
         {1, 1, -1, 1},
        /* AxesType::ZYRoll,*/
         0},  // kLeftForeArm,
        {{0, -0.839f, 0, 1},
         {0, 1, 0, 0},
         {-90, 0, -80},
         {90, 0, 80},
         {-1, 1, 1, 1},
        /* AxesType::ZYRoll,*/
         0},                                                                                           // kRightForeArm,
        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, -40, -80}, {0, 40, 80}, {1, 1, -1, 1},/* AxesType::ZYRoll,*/ 0},  // LeftHand,
        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, -40, -80}, {0, 40, 80}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 0},   // RightHand,
        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, 0, -50}, {0, 0, 50}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 3},      // LeftToes,
        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, 0, -50}, {0, 0, 50}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 3},      // RightToes,
        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, -20, -10}, {0, 20, 15}, {1, 1, -1, 1},/* AxesType::ZYRoll,*/ 3},  // LeftEye,
        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, -20, -10}, {0, 20, 15}, {1, -1, -1, 1},/* AxesType::ZYRoll,*/ 3},     // RightEye,
        {{0.09f, 0, 0, 1}, {1, 0, 0, 0}, {0, -10, -10}, {0, 10, 10}, {1, 1, -1, 1},/* AxesType::ZYRoll,*/ 3},  // Jaw,
        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-20, -20, -20}, {20, 20, 20}, {1, 1, 1, 1},/* AxesType::ZYRoll,*/ 0}  // UpperChest,
};


//Hand
//Five fingers
enum Fingers : uint8_t
{
    Thumb = 0,
    Index = 1,
    Middle = 2,
    Ring = 3,
    Little = 4,
    LastFinger = 5
};

//Each Finger has three phalanges
enum Phalanges : uint8_t
{
    Proximal = 0,//unity finger1
    Intermediate = 1,//unity finger2
    Distal = 2,//unity finger3
    LastPhalange = 3
};

//Each finger har four dof
enum FingerDoF : uint8_t
{
    ProximalDownUp = 0,
    ProximalInOut = 1,
    IntermediateCloseOpen = 2,
    DistalCloseOpen = 3,
    LastFingerDoF = 4
};

//Each hand
const int hBoneCount = Fingers::LastFinger * Phalanges::LastPhalange;//15
const int hDoFCount = Fingers::LastFinger * FingerDoF::LastFingerDoF;//20


static const std::string HandBoneName[hBoneCount] =
{
    "ThumbProximal",
    "ThumbIntermediate",
    "ThumbDistal",

    "IndexProximal",
    "IndexIntermediate",
    "IndexDistal",

    "MiddleProximal",
    "MiddleIntermediate",
    "MiddleDistal",

    "RingProximal",
    "RingIntermediate",
    "RingDistal",

    "LittleProximal",
    "LittleIntermediate",
    "LittleDistal",
};



struct Hand
{
    int mHandBoneIndex[hBoneCount];
    bool isLeft = true;
};


const static SetupAxesInfo handSetupAxesInfoArray[hBoneCount] = {
        {{0, 0.125, 0.125f, 1},
         {0, 1, 0, 0},
         {0, -25, -20},
         {0, 25, 20},
         {-1, -1, 1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Thumb.Proximal
        {{0, -0.2f, 0, 1},
         {0, 1, 0, 0},
         {0, 0, -40},
         {0, 0, 35},
         {-1, 1, 1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Thumb.Intermediate
        {{0, -0.2f, 0, 1},
         {0, 1, 0, 0},
         {0, 0, -40},
         {0, 0, 35},
         {-1, 1, 1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Thumb.Distal
        {{0, 0.08f, 0.3f, 1},
         {0, 0, 1, 0},
         {0, -20, -50},
         {0, 20, 50},
         {-1, -1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Index.Proximal
        {{0, 0, 0.33f, 1},
         {0, 0, 1, 0},
         {0, 0, -45},
         {0, 0, 45},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Index.Intermediate
        {{0, 0, 0.33f, 1},
         {0, 0, 1, 0},
         {0, 0, -45},
         {0, 0, 45},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Index.Distal
        {{0, 0.04f, 0.3f, 1},
         {0, 0, 1, 0},
         {0, -7.5f, -50},
         {0, 7.5f, 50},
         {-1, -1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Middle.Proximal
        {{0, 0, 0.33f, 1},
         {0, 0, 1, 0},
         {0, 0, -45},
         {0, 0, 45},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Middle.Intermediate
        {{0, 0, 0.33f, 1},
         {0, 0, 1, 0},
         {0, 0, -45},
         {0, 0, 45},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Middle.Distal
        {{0, -0.04f, 0.3f, 1},
         {0, 0, 1, 0},
         {0, -7.5f, -50},
         {0, 7.5f, 50},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Ring.Proximal
        {{0, 0, 0.33f, 1},
         {0, 0, 1, 0},
         {0, 0, -45},
         {0, 0, 45},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Ring.Intermediate
        {{0, 0, 0.33f, 1},
         {0, 0, 1, 0},
         {0, 0, -45},
         {0, 0, 45},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Ring.Distal
        {{0, -0.08f, 0.3f, 1},
         {0, 0, 1, 0},
         {0, -20, -50},
         {0, 20, 50},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Little.Proximal
        {{0, 0, 0.33f, 1},
         {0, 0, 1, 0},
         {0, 0, -45},
         {0, 0, 45},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0},  // Little.Intermediate
        {{0, 0, 0.33f, 1},
         {0, 0, 1, 0},
         {0, 0, -45},
         {0, 0, 45},
         {-1, 1, -1, -1},
         /*AxesType::ZYRoll,*/
         0}  // Little.Distal
};