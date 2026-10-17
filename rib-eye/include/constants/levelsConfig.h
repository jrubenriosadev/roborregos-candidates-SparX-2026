
#ifndef DEF_LEVELSCONFIG
#define DEF_LEVELSCONFIG

#include "Arduino.h"

// TCRT5000 (seguidor de linea)
namespace LineConfig {
    constexpr bool kDigital = false;    
    constexpr bool kWhiteIsHigh = false;   
    constexpr uint16_t kThreshold = 2000;  
    constexpr uint16_t kHysteresis = 150;  
    constexpr uint8_t kConfirm = 2;       
}

namespace LevelsConfig {
    constexpr float kTrackYawDeg = 0.0f;

    constexpr float kYawUpDeg = kTrackYawDeg;
    constexpr float kYawRightDeg = kTrackYawDeg + 90.0f;
    constexpr float kYawDownDeg = kTrackYawDeg + 180.0f;
    constexpr float kYawLeftDeg = kTrackYawDeg - 90.0f;

    constexpr unsigned long kColorSettleMs = 400;  
    constexpr unsigned long kColorWindowMs = 300;  
    constexpr int kColorMinSamples = 8;
    constexpr int kColorAgreePct = 60;
    constexpr uint8_t kColorRetries = 3;
    constexpr int kMaxTiles = 25;              
    constexpr unsigned long kTurnTimeoutMs = 3500;
    constexpr unsigned long kMoveTimeoutMs = 4500;
    constexpr unsigned long kTurnDelayMs = 300;

    constexpr float kFieldWidthM = 0.60f;  
    constexpr float kLanePitchM = kFieldWidthM / 3.0f;
    constexpr float kTravelM = 1.50f;             
    constexpr float kBackoffM = 0.07f;             

    constexpr float kClearM = 0.05f;
    constexpr float kLineAheadM = 0.08f;  
    constexpr float kFieldEndM = 1.35f; 
    constexpr float kLineMaskEndM = 0.10f; 
    constexpr float kLaneSpeed = 120.0f;
    constexpr unsigned long kLaneMoveTimeoutMs = 12000;
    constexpr unsigned long kShiftTimeoutMs = 4000;

    constexpr unsigned kBallMinMm = 130;          
    constexpr unsigned kBallMaxMm = 260;        
    constexpr int kPeekSettleTicks = 10;    
    constexpr int kPeekWindowTicks = 12;
    constexpr int kPeekMinSamples = 6;
    constexpr int kBallAgreePct = 60;
    constexpr float kBallReverseM = 0.25f;

}
#endif

