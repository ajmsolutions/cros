//==========================================================
// cros_core.h
//
// The 'kernel' of the CrOS is called "The Core"
//
// Version 0.99c - October 2018
//
// http://www.thecrowbox.com
//==========================================================
//   Except where otherwise noted, this work is licensed 
//   under a Creative Commons Attribution-ShareAlike 4.0 
//   International License
//==========================================================
#ifndef CROS_CORE_H
#define CROS_CORE_H

#include <Arduino.h>
#include <Servo.h>
#include "cros_types.h"
#include "cros_constants.h"
#include "Arduino_LED_Matrix.h"

// Forward declare IRrecv and decode_results
class IRrecv;
struct decode_results;

// Create global LED matrix object
extern ArduinoLEDMatrix matrix;

//==========================================================
// This fancy 'core' of ours is actually
// just an instance of this CCrowboxCore
// object.
//==========================================================
class CCrowboxCore
{
public:
    //--------------------
    // CTor/DTor
    //--------------------
    CCrowboxCore();
    ~CCrowboxCore();
    
    //--------------------------------------
    // To promote understanding, these methods
    // bear the same name as the Arduino
    // entry points which call them.
    //--------------------------------------
    void Setup();
    void Loop();

public:
    void DebugPrint( const char *pString );
    void ReportSystemError( cros_error_code_t errorCode );
    
    //--------------------------------------
    // Public Accessor methods
    //--------------------------------------
    cros_time_t GetUptimeSeconds();
    bool        IsABirdOnThePerch();
    cros_time_t HowLongHasBirdBeenHere();
    cros_time_t HowLongHasBirdBeenGone();
    
    bool  IsRewardBasketOpen() const    { return m_basketState == BASKET_STATE_OPEN; }
    
    unsigned char   GetCurrentTrainingPhase() const { return m_currentTrainingPhase; }    
    void            ReportCurrentTrainingPhase();
    
    //--------------------------------------
    // Coins & Rewards
    //--------------------------------------
    bool EnqueueCoin();
    void RemoveEnqueuedCoin();
    
    // IR transmitter codes and methods
    static const uint32_t IR_CODE_PERCH = 0x00FF01FE;  // Modified code
    static const uint32_t IR_CODE_COIN = 0x00FF02FD;   // Modified code
    void SendIRCode(uint32_t code);

    // Methods to set sensor flags
    void SetCoinDetected() { m_coinDetected = true; }
    void SetPerchPressed() { m_perchPressed = true; }

protected:
    //--------------------------------------
    // Native Methods
    //--------------------------------------
    void BlinkLED( int numTimes );
    void AttachBasketServo();
    void DetachBasketServo();
    
    void OpenRewardBasket();
    void CloseRewardBasket();
    
    bool Poll_IsPerchPressed();
    
    void ScheduleBasketCloseWithDelay( cros_time_t delayInSeconds );
    
    //--------------------------------------
    // Training Protocol
    //--------------------------------------
    void RunPhaseOneProtocol();
    void RunPhaseTwoProtocol();
    void RunPhaseThreeProtocol();
    void RunPhaseFourProtocol();
    void AdvanceCurrentTrainingPhase();

    void CheckTrainingPhaseSwitch();

    //--------------------------------------
    // EEPROM Methods
    //--------------------------------------
    bool  ValidateEEPROMData();
    void  CreateEEPROMData();
    void  LoadCurrentTrainingPhaseFromEEPROM();
    void  WriteCurrentTrainingPhaseToEEPROM();

    //--------------------------------------
    // Video
    //--------------------------------------
    void RecordVideo( cros_time_t duration );
    void StopRecordingVideo();

    void SetTrainingPhase(unsigned char phase);

private:
    cros_time_t m_uptimeWhenBirdLanded;
    cros_time_t m_uptimeWhenBirdDeparted;
    cros_time_t m_uptimeScheduledBasketClose;
    cros_time_t m_uptimeLastCoinDetected;
    
    //--------------------------------------
    // This semaphore is used to begin recording video
    // or to extend recording time if already doing so
    //--------------------------------------
    cros_time_t m_uptimeStopRecordingVideo;
    bool        m_isRecordingVideo;

    //--------------------------------------
    // Indicates which (1,2,3 or 4) training
    // phase the box is currently observing
    //--------------------------------------
    unsigned char m_currentTrainingPhase;

    //--------------------------------------
    // Used to track the current state of 
    // the basket. Whether it's open or 
    // closed, for instance.
    //--------------------------------------
    int m_basketState;
    
    //--------------------------------------
    // Each time a coin hits the coin deposit
    // sensor, we increment this value. We
    // decrement this each time we open the 
    // reward basket (Ph3+) so if several coins
    // are dropped at about the same time,
    // this will rack up three full cycles of
    // the door in order to square up the debt
    //--------------------------------------
    volatile int m_numEnqueuedDeposits;
    
    //--------------------------------------
    // The servo which controls the lid that
    // covers the reward basket.
    //--------------------------------------
    Servo m_basketServo;

    IRrecv* m_irReceiver;
    decode_results* m_irResults;

    // Flag to indicate if basket is in manual override mode
    bool m_basketManualOverride;

    // LED Matrix frame buffer
    byte matrixFrame[8][12];
    
    // Method to update matrix display
    void UpdateMatrixDisplay();
    void ClearMatrixFrame();

    // Add these flags
    volatile bool m_coinDetected;
    volatile bool m_perchPressed;
};

extern CCrowboxCore g_crOSCore;

#endif//CROS_CORE_H

