//
//struct SetupAxesInfo
//{
//    float m_PreQ[4];//t-pose to muscle-zero-pose
//    float m_MainAxis[4];
//    float m_Min[3];
//    float m_Max[3];
//    float m_Sgn[4];
//    AxesType m_Type;
//    int m_ForceAxis;
//};
//
//
////body
//const static SetupAxesInfo setupAxesInfoArray[LastBone] = {
//        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1}, AxesType::ZYRoll, 0},  // Hips,
//        {{-0.268f, 0, 0, 1},
//         {1, 0, 0, 0},
//         {-60, -60, -90},
//         {60, 60, 50},
//         {1, 1, 1, 1},
//         AxesType::ZYRoll,
//         0},  // LeftUpperLeg,
//        {{-0.268f, 0, 0, 1},
//         {1, 0, 0, 0},
//         {-60, -60, -90},
//         {60, 60, 50},
//         {-1, -1, 1, 1},
//         AxesType::ZYRoll,
//         0},  // RightUpperLeg,
//        {{0.839f, 0, 0, 1}, {1, 0, 0, 0}, {-90, 0, -80}, {90, 0, 80}, {1, 1, -1, 1}, AxesType::ZYRoll, 0},  // kLeftLeg,
//        {{0.839f, 0, 0, 1},
//         {1, 0, 0, 0},
//         {-90, 0, -80},
//         {90, 0, 80},
//         {-1, 1, -1, 1},
//         AxesType::ZYRoll,
//         0},                                                                                              // kRightLeg,
//        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, -30, -50}, {0, 30, 50}, {1, 1, 1, 1}, AxesType::ZYRoll, -2},     // LeftFoot,
//        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, -30, -50}, {0, 30, 50}, {1, -1, 1, 1}, AxesType::ZYRoll, -2},    // RightFoot,
//        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1}, AxesType::ZYRoll, 0},  // Spine,
//        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1}, AxesType::ZYRoll, 0},  // Chest,
//        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1}, AxesType::ZYRoll, 0},  // Neck,
//        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-40, -40, -40}, {40, 40, 40}, {1, 1, 1, 1}, AxesType::ZYRoll, 2},  // Head,
//        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, -15, -15}, {0, 15, 30}, {1, 1, -1, 1}, AxesType::ZYRoll, 0},  // LeftShoulder,
//        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, -15, -15}, {0, 15, 30}, {1, 1, 1, 1}, AxesType::ZYRoll, 0},   // RightShoulder,
//        {{0, 0.268f, 0.364f, 1},
//         {0, 0, 1, 0},
//         {-90, -100, -60},
//         {90, 100, 100},
//         {1, 1, -1, 1},
//         AxesType::ZYRoll,
//         0},  // kLeftArm,
//        {{0, -0.268f, -0.364f, 1},
//         {0, 0, 1, 0},
//         {-90, -100, -60},
//         {90, 100, 100},
//         {-1, 1, 1, 1},
//         AxesType::ZYRoll,
//         0},  // kRightArm,
//        {{0, 0.839f, 0, 1},
//         {0, 1, 0, 0},
//         {-90, 0, -80},
//         {90, 0, 80},
//         {1, 1, -1, 1},
//         AxesType::ZYRoll,
//         0},  // kLeftForeArm,
//        {{0, -0.839f, 0, 1},
//         {0, 1, 0, 0},
//         {-90, 0, -80},
//         {90, 0, 80},
//         {-1, 1, 1, 1},
//         AxesType::ZYRoll,
//         0},                                                                                           // kRightForeArm,
//        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, -40, -80}, {0, 40, 80}, {1, 1, -1, 1}, AxesType::ZYRoll, 0},  // LeftHand,
//        {{0, 0, 0, 1}, {0, 0, 1, 0}, {0, -40, -80}, {0, 40, 80}, {1, 1, 1, 1}, AxesType::ZYRoll, 0},   // RightHand,
//        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, 0, -50}, {0, 0, 50}, {1, 1, 1, 1}, AxesType::ZYRoll, 3},      // LeftToes,
//        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, 0, -50}, {0, 0, 50}, {1, 1, 1, 1}, AxesType::ZYRoll, 3},      // RightToes,
//        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, -20, -10}, {0, 20, 15}, {1, 1, -1, 1}, AxesType::ZYRoll, 3},  // LeftEye,
//        {{0, 0, 0, 1}, {1, 0, 0, 0}, {0, -20, -10}, {0, 20, 15}, {1, -1, -1, 1}, AxesType::ZYRoll, 3},     // RightEye,
//        {{0.09f, 0, 0, 1}, {1, 0, 0, 0}, {0, -10, -10}, {0, 10, 10}, {1, 1, -1, 1}, AxesType::ZYRoll, 3},  // Jaw,
//        {{0, 0, 0, 1}, {-1, 0, 0, 0}, {-20, -20, -20}, {20, 20, 20}, {1, 1, 1, 1}, AxesType::ZYRoll, 0}  // UpperChest,
//};
//
//
////hand
//const static skeleton::SetupAxesInfo setupAxesInfoArray[sBoneCount] = {
//        {{0, 0.125, 0.125f, 1},
//         {0, 1, 0, 0},
//         {0, -25, -20},
//         {0, 25, 20},
//         {-1, -1, 1, -1},
//         AxesType::ZYRoll,
//         0},  // Thumb.Proximal
//        {{0, -0.2f, 0, 1},
//         {0, 1, 0, 0},
//         {0, 0, -40},
//         {0, 0, 35},
//         {-1, 1, 1, -1},
//         AxesType::ZYRoll,
//         0},  // Thumb.Intermediate
//        {{0, -0.2f, 0, 1},
//         {0, 1, 0, 0},
//         {0, 0, -40},
//         {0, 0, 35},
//         {-1, 1, 1, -1},
//         AxesType::ZYRoll,
//         0},  // Thumb.Distal
//        {{0, 0.08f, 0.3f, 1},
//         {0, 0, 1, 0},
//         {0, -20, -50},
//         {0, 20, 50},
//         {-1, -1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Index.Proximal
//        {{0, 0, 0.33f, 1},
//         {0, 0, 1, 0},
//         {0, 0, -45},
//         {0, 0, 45},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Index.Intermediate
//        {{0, 0, 0.33f, 1},
//         {0, 0, 1, 0},
//         {0, 0, -45},
//         {0, 0, 45},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Index.Distal
//        {{0, 0.04f, 0.3f, 1},
//         {0, 0, 1, 0},
//         {0, -7.5f, -50},
//         {0, 7.5f, 50},
//         {-1, -1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Middle.Proximal
//        {{0, 0, 0.33f, 1},
//         {0, 0, 1, 0},
//         {0, 0, -45},
//         {0, 0, 45},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Middle.Intermediate
//        {{0, 0, 0.33f, 1},
//         {0, 0, 1, 0},
//         {0, 0, -45},
//         {0, 0, 45},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Middle.Distal
//        {{0, -0.04f, 0.3f, 1},
//         {0, 0, 1, 0},
//         {0, -7.5f, -50},
//         {0, 7.5f, 50},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Ring.Proximal
//        {{0, 0, 0.33f, 1},
//         {0, 0, 1, 0},
//         {0, 0, -45},
//         {0, 0, 45},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Ring.Intermediate
//        {{0, 0, 0.33f, 1},
//         {0, 0, 1, 0},
//         {0, 0, -45},
//         {0, 0, 45},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Ring.Distal
//        {{0, -0.08f, 0.3f, 1},
//         {0, 0, 1, 0},
//         {0, -20, -50},
//         {0, 20, 50},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Little.Proximal
//        {{0, 0, 0.33f, 1},
//         {0, 0, 1, 0},
//         {0, 0, -45},
//         {0, 0, 45},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0},  // Little.Intermediate
//        {{0, 0, 0.33f, 1},
//         {0, 0, 1, 0},
//         {0, 0, -45},
//         {0, 0, 45},
//         {-1, 1, -1, -1},
//         AxesType::ZYRoll,
//         0}  // Little.Distal
//};
//
//
//static void SetupAxes(Skeleton& arSkeleton,
//    const SkeletonPose& skeletonPoseG,
//    const SetupAxesInfo& setupAxesInfo,
//    int index,
//    int axisIndex,
//    bool left,
//    float len)
//{
//    SkeletonPoint& point = arSkeleton.m_Points[index];
//    int parentIndex = point.m_ParentId;
//
//    if (point.m_AxesId != -1)
//    {
//        Axes& axes = arSkeleton.m_AxesArray[point.m_AxesId];
//
//        float4x4 boneX = skeletonPoseG[index];
//
//        axes.m_Limit.m_Min
//            = math::radians(float3(setupAxesInfo.m_Min[0], setupAxesInfo.m_Min[1], setupAxesInfo.m_Min[2]));
//        axes.m_Limit.m_Max
//            = math::radians(float3(setupAxesInfo.m_Max[0], setupAxesInfo.m_Max[1], setupAxesInfo.m_Max[2]));
//        axes.m_Sgn = float3(setupAxesInfo.m_Sgn[0], setupAxesInfo.m_Sgn[1], setupAxesInfo.m_Sgn[2])
//            * (left ? float3(1) : float3(-1, 1, -1));
//
//        float3 mainAxis = float3(setupAxesInfo.m_MainAxis[0], setupAxesInfo.m_MainAxis[1], setupAxesInfo.m_MainAxis[2]);
//        float4 zeroQ
//            = float4(setupAxesInfo.m_PreQ[0], setupAxesInfo.m_PreQ[1], setupAxesInfo.m_PreQ[2], setupAxesInfo.m_PreQ[3])
//            * (left ? float4(1) : float4(1, 1, 1, -1));
//
//        float3 u(1, 0, 0);
//        float3 w(0, 1, 0);
//        float3 v(0, 0, 1);
//
//        axes.m_Type = setupAxesInfo.m_Type;
//
//        axes.m_Length = 1.0f;
//
//        if (axisIndex != -1)
//        {
//            float4x4 axisX = skeletonPoseG[axisIndex];
//
//            u = math::normalize((axisX.GetPosition() - boneX.GetPosition()) * len);
//
//            w = mainAxis;
//            v = math::normalize(math::cross(w, u));
//            w = math::cross(u, v);
//
//            axes.m_Length = math::length(axisX.GetPosition() - boneX.GetPosition());
//        }
//
//        if (setupAxesInfo.m_ForceAxis)
//        {
//            switch (setupAxesInfo.m_ForceAxis)
//            {
//            case +1:
//                u = float3(+1, 0, 0);
//                break;
//            case -1:
//                u = float3(-1, 0, 0);
//                break;
//            case +2:
//                u = float3(0, +1, 0);
//                break;
//            case -2:
//                u = float3(0, -1, 0);
//                break;
//            case +3:
//                u = float3(0, 0, +1);
//                break;
//            default:
//                u = float3(0, 0, -1);
//                break;
//            };
//
//            w = mainAxis;
//            v = math::normalize(math::cross(w, u));
//            w = math::cross(u, v);
//        }
//
//        axes.m_Length *= math::abs(len);
//
//        quaternion parentQ = parentIndex != -1 ? skeletonPoseG[parentIndex].GetRotation() : quaternion::identity();
//
//        axes.m_PreQ = quaternion(float3x3(u, v, w));
//        axes.m_PostQ = math::normalize(math::mul(math::conjugate(boneX.GetRotation()), axes.m_PreQ));
//        axes.m_PreQ = math::normalize(math::mul(math::conjugate(parentQ), math::mul(zeroQ, axes.m_PreQ)));
//    }
//}
//
////转axes的quaternion
//static inline quaternion AxesProject(const Axes& a, const quaternion& q)
//{
//    return math::normalizesafe(math::mul(math::conjugate(a.m_PreQ), math::mul(q, a.m_PostQ)));
//}
//
////转bone localRotation
//static inline quaternion AxesUnproject(const Axes& a, const quaternion& q)
//{
//    return math::normalizesafe(math::mul(a.m_PreQ, math::mul(q, math::conjugate(a.m_PostQ))));
//}