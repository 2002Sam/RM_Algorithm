#pragma once
#include"RMDefine.h"
//装甲识别参数
struct ArmorPara {
    EnemyColor enemyColor;
    int minLightBarArea = 20;//10
    int minLightBarLength = 5;
    int minLightBarWidth = 2;
    int maxLightBarLength = 200;
    int maxLightBarWidth = 50;
    int maxLightBarArea = 15000;
    float maxErrorAngle = 15;
    int grayThreshold_PURPLE = 150;
    int grayThreshold_RED = 100;
    int grayThreshold_BLUE = 100;
    int separationThreshold_RED = 10;
    int separationThreshold_BLUE = 20;
    int separationThreshold_GREEN = 40;
    float light_contour_min_solidity = 0.5;
};