// cros_core.cpp
//
// The 'kernal' of the CrOS is called "The Core"
//
// Version 0.99c - October 2018
//
// http://www.thecrowbox.com
//==========================================================
//   Except where otherwise noted, this work is licensed 
//   under a Creative Commons Attribution-ShareAlike 4.0 
//   International License
//==========================================================
#include <EEPROM.h>
#include "cros_core.h"
#include <IRremote.h>

// Add these pattern definitions at the top of the file, after the includes
const uint32_t PHASE_ONE_DISPLAY[] = {
    0x000C0000,  // "I"
    0x000C0000,
    0x000C0000
};

const uint32_t PHASE_TWO_DISPLAY[] = {
    0x00CC0000,  // "II"
    0x00CC0000,
    0x00CC0000
};

const uint32_t PHASE_THREE_DISPLAY[] = {
    0x0CCC0000,  // "III"
    0x0CCC0000,
    0x0CCC0000
};

const uint32_t PHASE_FOUR_DISPLAY[] = {
    0xCCCC0000,  // "IIII"
    0xCCCC0000,
    0xCCCC0000
};

// Define the Roman numeral patterns
const byte PATTERN_I[8][12] = {
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte PATTERN_II[8][12] = {
    { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte PATTERN_III[8][12] = {
    { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte PATTERN_IIII[8][12] = {
    { 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
    { 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
    { 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
    { 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
    { 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
    { 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
    { 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte PATTERN_O[8][12] = {
    { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

const byte PATTERN_C[8][12] = {
    { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0 },
    { 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

//==========================================================
// Interrupt function called when the coin sensor is struck
// by a coin, bringing the coin pin to LOW (switched to ground).
// Contact bounce (look it up) may cause this interrupt to fire
// multiple times per coin, so the code within EnqueueCoin()
// is designed to accept only one deposit per second so that 
// each coin isn't counted multiple times due to contact bounce.
//==========================================================
void Interrupt_CoinDeposit()
{
    g_crOSCore.EnqueueCoin();
    // Open basket first, then queue IR transmission
    g_crOSCore.SetCoinDetected();
}

//----------------------------------------------------------
// Simple function to pipe the provided string to serial
//----------------------------------------------------------
void CCrowboxCore::DebugPrint( const char *pString )
{
#if defined( CROS_USE_SERIAL_DEBUG )
  Serial.println( pString );
#endif// CROS_USE_SERIAL_DEBUG
}

//----------------------------------------------------------
// Once we get here, we never leave. The arduino stays in a
// state where the indicator LED will blink out the error
// code so that the human operator can get an idea of what
// went wrong. As much idea as the code that called this
// SystemError() method can provide, in the form of a 
// cros_error_code_t (see cros_constants.h)
//
// NOTE: Ending up here will freeze the CrowBox in its 
// current state and it will not respond until reset.
//----------------------------------------------------------
void CCrowboxCore::ReportSystemError( cros_error_code_t errorCode )
{
  digitalWrite( OUTPUT_PIN_LED, LOW );

  while( 1 )
  {
    for( int i = 0 ; i < errorCode ; ++i )
    {
      digitalWrite( OUTPUT_PIN_LED, HIGH );
      delay( 500 );
      digitalWrite( OUTPUT_PIN_LED, LOW );
      delay( 500 );
    }

    // Delay for a little longer then blink out the sequence again
    delay( 1000 );
  }
}

//----------------------------------------------------------
// Here in the Core's constructor we initialize the variables
// that we want to make sure are set to known values even 
// before the Setup() method is called.
//----------------------------------------------------------
CCrowboxCore::CCrowboxCore()
{
    // Initialize the basket state. For now, we have no idea
    // what state the lid was in when the machine powered 
    // down last time so we set state to DONT_KNOW until some
    // other piece of code opens or closes the basket and
    // makes the state official. The basket lid will be properly
    // parked when the Core's Setup() method is called.
    m_basketState = BASKET_STATE_DONT_KNOW;
    
    // This pair of timers store the Uptime of the most recent
    // arrival (depression of perch) and departure (release of perch).
    m_uptimeWhenBirdLanded = TIME_NEVER;
    m_uptimeWhenBirdDeparted = TIME_NEVER;
    
    // Initialize this to never for now. It will come to use 
    // later as birds come and go.
    m_uptimeScheduledBasketClose = TIME_NEVER;    
    
    // Initialize IR receiver and results
    m_irReceiver = new IRrecv(INPUT_PIN_IR_RX);
    m_irResults = new decode_results();
    m_irReceiver->enableIRIn();

    m_basketManualOverride = false;
    m_coinDetected = false;
    m_perchPressed = false;
}

CCrowboxCore::~CCrowboxCore()
{
    delete m_irReceiver;
    delete m_irResults;
}

//----------------------------------------------------------
//----------------------------------------------------------
void CCrowboxCore::Setup()
{
    // Initialize Serial first thing
    Serial.begin(CROS_SERIAL_BAUD_RATE);
    delay(1000);  // Give serial port time to initialize
    
    Serial.println("\n\nCrowBox Starting Up...");
    Serial.println("Serial communication initialized");
    
    DebugPrint( "Setup() method CALLED...\n" );
 
    // Start with no enqueued deposits
    m_numEnqueuedDeposits = 0;
       
    // Set up the indicator LED pin, then turn the LED off to
    // save that microscopic amount of power. 
    pinMode( OUTPUT_PIN_LED, OUTPUT );
    digitalWrite( OUTPUT_PIN_LED, LOW );

    // Ensure that the stored EEPROM data is valid, then load
    // the current training phase from storage there.
    if( !ValidateEEPROMData() )
    {
      // Oops! The EEPROM data is not valid. This probably just
      // means that the Arduino board in use has not been used 
      // to operate a crowbox before. So we'll create valid 
      // EEPROM data that can be used henceforth. 
      CreateEEPROMData();

      // Now that we've created EEPROM data for CrOS, let's be
      // sure that it actually worked. If not, that's a fatal
      // error that needs to be reported! 
      if( !ValidateEEPROMData() )
      {
        ReportSystemError( kError_EEPROM );
      }
    }

    // If we reach this point, we're sure the EEPROM data is good
    // so we'll retrieve the stored data there which tells use which
    // phase of the training protocol is currently in use.
    LoadCurrentTrainingPhaseFromEEPROM();    

    switch( m_currentTrainingPhase )
    {
      case PHASE_ONE:   DebugPrint("Loaded PHASE ONE from EEPROM\n" );    break;
      case PHASE_TWO:   DebugPrint("Loaded PHASE TWO from EEPROM\n" );    break;
      case PHASE_THREE: DebugPrint("Loaded PHASE THREE from EEPROM\n" );  break;
      case PHASE_FOUR:  DebugPrint("Loaded PHASE FOUR from EEPROM\n" );   break;
      default:
        DebugPrint("Loaded garbage training phase from EEPROM!");
        ReportSystemError( kError_BadTrainingPhase );
        break;
    }

    // Set up the pin that is attached to the pushbutton
    // which is used to cycle the training phase
    pinMode( INPUT_PIN_PHASE_SELECT, INPUT_PULLUP );
    
    // Set up the PERCH switch
    pinMode( INPUT_PIN_PERCH, INPUT_PULLUP );
    
    // Set up the COIN detect switch AND its interrupt
    pinMode( INPUT_PIN_COIN, INPUT_PULLUP );
    attachInterrupt( digitalPinToInterrupt(INPUT_PIN_COIN), Interrupt_CoinDeposit, FALLING );

    DebugPrint( "  Servo initialization and lid parking...\n" );
    
    // Attach the servo device to the pin which controls the servo position
    AttachBasketServo();
    
    // Do this little dance to put the servo into a known good state and position
    m_basketServo.write( SERVO_POS_MIDPOINT );
    delay( 1500 );
    m_basketServo.write( SERVO_POS_OPEN );
    m_basketState = BASKET_STATE_OPEN;  
    delay( 1500 );
    
    // Direct the door to close right now. 
    CloseRewardBasket();

    // Set the sentinel that protects us from contact bounce on coin deposits.
    // Do this by setting it to the current time plus a little bit of slop.
    m_uptimeLastCoinDetected = GetUptimeSeconds() + 0.1f;

    // Ensure video is not being recorded
    StopRecordingVideo();

    // Ensure everything has settled out before proceeding. 
    delay( 1000 );

    // Initialize Serial communication with proper baud rate
    Serial.begin(CROS_SERIAL_BAUD_RATE);
    while (!Serial) {
        ; // Wait for serial port to connect
    }
    
    // Set up IR receiver
    Serial.print("\nInitializing HX1838 IR receiver on pin ");
    Serial.println(INPUT_PIN_IR_RX);
    
    // Configure for HX1838 with NEC protocol
    IrReceiver.begin(INPUT_PIN_IR_RX, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
    
    Serial.println("HX1838 IR receiver initialized");
    Serial.println("Protocol: NEC");
    Serial.println("Point remote at receiver and press buttons");
    
    // Test the IR pin
    Serial.print("IR Pin Initial State: ");
    Serial.println(digitalRead(INPUT_PIN_IR_RX));

    // Print the enabled protocols
    Serial.println("Enabled IR protocols:");
    #if defined(DECODE_NEC)
        Serial.println(" - NEC");
    #endif
    #if defined(DECODE_SONY)
        Serial.println(" - SONY"); 
    #endif
    #if defined(DECODE_RC5)
        Serial.println(" - RC5");
    #endif
    #if defined(DECODE_RC6) 
        Serial.println(" - RC6");
    #endif

    DebugPrint( "  Up and running!\n\n" );

    matrix.begin();
    UpdateMatrixDisplay();  // Show initial phase

    // Set up IR transmitter pin
    pinMode(OUTPUT_PIN_IR_TX, OUTPUT);
    digitalWrite(OUTPUT_PIN_IR_TX, LOW);
    
    // Initialize IR sender with explicit protocol settings
    IrSender.begin(OUTPUT_PIN_IR_TX, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
    IrSender.enableIROut(38); // Set carrier frequency to 38kHz
}

//----------------------------------------------------------
//----------------------------------------------------------
void CCrowboxCore::Loop() 
{
    static unsigned long lastDebugPrint = 0;
    static int noSignalCounter = 0;
    static uint32_t lastValidCode = 0;
    static unsigned long lastIRTime = 0;
    
    // Check for IR remote signals
    if (IrReceiver.decode()) {
        uint32_t currentTime = millis();
        uint32_t rawData = IrReceiver.decodedIRData.decodedRawData;
        uint8_t address = (rawData >> 24) & 0xFF;
        uint8_t command = (rawData >> 16) & 0xFF;
        
        // Always show received codes for debugging
        if (IrReceiver.decodedIRData.protocol == NEC &&
            rawData != lastValidCode &&
            (currentTime - lastIRTime) > 250 &&
            address != 0xE7) {  // Still filter obvious noise
            
            Serial.println("\n*** IR Signal Received! ***");
            Serial.print("Address: 0x");
            Serial.println(address, HEX);
            Serial.print("Command: 0x");
            Serial.println(command, HEX);
            Serial.print("Raw Data: 0x");
            Serial.println(rawData, HEX);
            
            // Only process our known command codes
            if (command == 0x45 || command == 0x46 || command == 0x47 || 
                command == 0x44 || command == 0x08 || command == 0x5A || 
                command == 0x16) {
                
                noSignalCounter = 0;
                lastValidCode = rawData;
                lastIRTime = currentTime;
                
                // Handle specific button presses
                switch(command) {
                    case 0x45:  // Button 1
                    case 0x46:  // Button 2
                    case 0x47:  // Button 3
                    case 0x44:  // Button 4
                        if (m_basketManualOverride) {
                            Serial.println("Cannot change phase while in manual override mode");
                        } else {
                            switch(command) {
                                case 0x45:
                                    Serial.println("Setting to Phase 1");
                                    SetTrainingPhase(PHASE_ONE);
                                    break;
                                case 0x46:
                                    Serial.println("Setting to Phase 2");
                                    SetTrainingPhase(PHASE_TWO);
                                    break;
                                case 0x47:
                                    Serial.println("Setting to Phase 3");
                                    SetTrainingPhase(PHASE_THREE);
                                    break;
                                case 0x44:
                                    Serial.println("Setting to Phase 4");
                                    SetTrainingPhase(PHASE_FOUR);
                                    break;
                            }
                        }
                        break;

                    case 0x08:  // Left button - Open and lock basket
                        Serial.println("Manual Override: Opening and locking basket");
                        m_basketManualOverride = true;
                        OpenRewardBasket();
                        m_uptimeScheduledBasketClose = TIME_NEVER;
                        UpdateMatrixDisplay();  // Update display to show 'O'
                        break;

                    case 0x5A:  // Right button - Close and lock basket
                        Serial.println("Manual Override: Closing and locking basket");
                        m_basketManualOverride = true;
                        CloseRewardBasket();
                        m_uptimeScheduledBasketClose = TIME_NEVER;
                        UpdateMatrixDisplay();  // Update display to show 'C'
                        break;

                    case 0x16:  // * button - Return to normal operation (was 0x00)
                        Serial.println("Returning to normal operation");
                        m_basketManualOverride = false;  // Turn off manual override FIRST
                        UpdateMatrixDisplay();  // Update display to show phase number
                        
                        // Now reset basket state based on current phase WITHOUT updating display
                        switch(m_currentTrainingPhase) {
                            case PHASE_ONE:
                                OpenRewardBasket();
                                m_uptimeScheduledBasketClose = TIME_NEVER;
                                break;
                            case PHASE_TWO:
                                if (IsABirdOnThePerch()) {
                                    OpenRewardBasket();
                                } else {
                                    CloseRewardBasket();
                                }
                                break;
                            case PHASE_THREE:
                            case PHASE_FOUR:
                                CloseRewardBasket();
                                m_uptimeScheduledBasketClose = TIME_NEVER;
                                m_numEnqueuedDeposits = 0;
                                break;
                        }
                        break;
                    
                    default:
                        Serial.print("Unknown button (0x");
                        Serial.print(command, HEX);
                        Serial.println(")");
                        break;
                }
            }
            else {
                Serial.println("Unknown command - not processed");
            }
            
            Serial.println("---------------");
        }
        
        IrReceiver.resume();
    }
    
    // Debug output every 5 seconds
    if (millis() - lastDebugPrint > 5000) {
        lastDebugPrint = millis();
        Serial.print("Waiting for IR signals... (");
        Serial.print(noSignalCounter++);
        Serial.println(" intervals without signal)");
    }

    // Take a quick sample of the uptime in milliseconds. We'll use this value
    // near the end of this function to determine how long this call to Loop()
    // will take.
    unsigned long msWhenLoopBegan = millis();

    // If the basket is scheduled to close on a timer and it is time to close 
    // the basket, then close it. Placing this code here makes automated closing 
    // of the basket a system-level service that takes place no matter which phase
    // of training is currently being observed.
    if( IsRewardBasketOpen() && m_uptimeScheduledBasketClose != TIME_NEVER ) 
    {
        // If the basket is open and a close command is scheduled
        // then check to see if it's time to shut the basket
        if( GetUptimeSeconds() >= m_uptimeScheduledBasketClose )
        {
          // Close the basket and de-schedule
          CloseRewardBasket();
          m_uptimeScheduledBasketClose = TIME_NEVER;
        }
    }
    
    // Now call the correct function to execute the desired behavior
    // of the currently-selected training phase. This isn't the most
    // elegant way to do this, but it is convenient to just place the
    // code that is specific to each phase in the respective discrete 
    // function.
    switch( GetCurrentTrainingPhase() )
    {
    case PHASE_ONE:
      RunPhaseOneProtocol();
      break;
    case PHASE_TWO:
      RunPhaseTwoProtocol();
      break;
    case PHASE_THREE:
      RunPhaseThreeProtocol();
      break;
    case PHASE_FOUR:
      RunPhaseFourProtocol();
      break;
    default:
        ReportSystemError( kError_BadTrainingPhase );
      break;
    }  

    // FOR FUTURE EXPANSION:
    // Run the logic for video recording. Note that the way this logic 
    // is arranged, calling RecordVideo() while the system is already 
    // recording video will magically extend the duration  
    if( m_isRecordingVideo )
    {
        // Should we turn off?
        if( GetUptimeSeconds() >= m_uptimeStopRecordingVideo )
        {
            StopRecordingVideo();
        }
    }

    // Poll to see if the human operator has pressed the switch which is
    // used to change the selected training phase.
    CheckTrainingPhaseSwitch();

    // Handle any pending IR transmissions
    if (m_coinDetected) {
        Serial.println("\n*** Transmitting Coin Detection IR Code ***");
        Serial.print("Code: 0x");
        Serial.println(IR_CODE_COIN, HEX);
        SendIRCode(IR_CODE_COIN);
        m_coinDetected = false;
    }
    
    if (m_perchPressed) {
        Serial.println("\n*** Transmitting Perch Detection IR Code ***");
        Serial.print("Code: 0x");
        Serial.println(IR_CODE_PERCH, HEX);
        SendIRCode(IR_CODE_PERCH);
        m_perchPressed = false;
    }

    // Now we do some time arithmetic to figure out how long this loop took to
    // execute. If it's less than IDEAL_LOOP_MS, then we make the system 
    // delay() for the balance. We intentionlly slow how often the software loop
    // can run mainly to hedge against problems with contact bounce from the 
    // perch switches and birds hopping around erratically. It's cheaper and 
    // easier than a hardware debouncing solution. 
    unsigned long msLoopDuration = millis() - msWhenLoopBegan;
    if( msLoopDuration < CROS_IDEAL_LOOP_MS )
    {
        unsigned long slackTime = CROS_IDEAL_LOOP_MS - msLoopDuration;
        delay( slackTime );
    }
}  

//----------------------------------------------------------
// Uptime is computed and converted to seconds each time 
// this method is called. 
//
// @@@BUG - The way this is implemented, a CrowBox that runs
// continuously for around 40 days will run into problems
// when the float value rolls over. This is easily avoided 
// by manually resetting the CrowBox each time the food or 
// coins needs to be refilled.
//----------------------------------------------------------
cros_time_t CCrowboxCore::GetUptimeSeconds()
{
  // Grab the current Arduino uptime in milliseconds
  unsigned long currentTimeMS = millis();
  
  // Convert down to seconds. Use a double to protect
  // precision and range.
  double currentTimeSeconds = (double)currentTimeMS;
  currentTimeSeconds /= 1000.0;
  
  // Downconvert to a float and return
  return ((cros_time_t)currentTimeSeconds);    
}

//----------------------------------------------------------
// Simple logic- if the timer that tells us the last time
// a bird landed is more recent than the timer that tells us
// the last time a bird departed, then there is a bird present
// right now.
//----------------------------------------------------------
bool CCrowboxCore::IsABirdOnThePerch()
{
  return (m_uptimeWhenBirdLanded > m_uptimeWhenBirdDeparted );
}

//----------------------------------------------------------
// NOTE: If  you call this and no bird is actually here,
// you'll get garbage. Well, you'll get TIME_NEVER.
// Make sure there's a bird present before you call.
//----------------------------------------------------------
cros_time_t CCrowboxCore::HowLongHasBirdBeenHere()
{
  if( m_uptimeWhenBirdLanded == TIME_NEVER )
  {
    return TIME_NEVER;
  }

  return GetUptimeSeconds() - m_uptimeWhenBirdLanded;
}

//----------------------------------------------------------
// If you call this while a bird is present, you'll get 
// garbage. First make sure there's no bird present
//----------------------------------------------------------
cros_time_t CCrowboxCore::HowLongHasBirdBeenGone()
{
  if( m_uptimeWhenBirdDeparted == TIME_NEVER )
  {
    return TIME_NEVER;
  }

  return GetUptimeSeconds() - m_uptimeWhenBirdDeparted;
}

//----------------------------------------------------------
// When a bird deposits a coin we account for it here by
// incrementing the internal count of enqueued deposits. 
// Elsewhere, the training protocol code will notice the 
// deposit and take action.
//
// NOTE: This method is usually called by an interrupt
// function, so we're required to keep this code as lean and
// fast as possible. Also, we cannot touch the serial port(s)
// during this time.
//
// Returns true if the coin count was actually affected,
// false if the deposit was ignored because it occurred 
// too near in time to the prior deposit. See further notes
// below on contact bounce, and the debouncing strategy.
//----------------------------------------------------------
bool CCrowboxCore::EnqueueCoin()              
{
    if( GetUptimeSeconds() - m_uptimeLastCoinDetected < 1.0f )
    {
        // We must only accept one coin deposit per second. Because of the
        // type of switch we use to detect coin deposits (a conductive copper
        // strip), there's a high likelihood of low-frequency
        // contact bounces (10-20hz, etc) after the sensor is first struck
        // by a rolling coin.
        //
        // So, when a coin hits the sensor and this function is called, we 
        // will count the coin then ignore the coin sensor for one second. 
        // Without this debouncing feature, a single coin might be counted 
        // dozens of times as it rolls along the copper rail of the sensor.
        return false;
    }
    
    m_numEnqueuedDeposits++;    
    m_uptimeLastCoinDetected = GetUptimeSeconds();
    return true;
}

//----------------------------------------------------------
// Called when we pay off a deposit by opening the reward 
// basket. 
//----------------------------------------------------------
void CCrowboxCore::RemoveEnqueuedCoin()      
{ 
    m_numEnqueuedDeposits--; 
}

//----------------------------------------------------------
// Used mainly for debugging or conveying information when
// no serial connection is available. Don't ship any code
// that calls this function- it's strictly intended for use
// during development and debugging. 
//
// NOTE: This is a BLOCKING operation.
//----------------------------------------------------------
void CCrowboxCore::BlinkLED( int numTimes )
{
    for(  unsigned char i = 0 ; i < numTimes ; ++i )
    {
      // Turn the LED on for a moment
      digitalWrite( OUTPUT_PIN_LED, HIGH );
      delay( 500 );

      // Now off for a moment
      digitalWrite( OUTPUT_PIN_LED, LOW );
      delay( 500 );
    }

    // Make sure the LED is off when we are done.
    digitalWrite( OUTPUT_PIN_LED, LOW );
}

//----------------------------------------------------------
// Helper function to attach the basket servo
//
// NOTE: This function is safe to call even if the servo is
// already attached. If the servo is attached, this function
// will do nothing.
//----------------------------------------------------------
void CCrowboxCore::AttachBasketServo()
{
  if( !m_basketServo.attached() )
  {
    m_basketServo.attach( OUTPUT_PIN_SERVO );
  }
}

//----------------------------------------------------------
// Helper function to detach the basket servo. This is done
// so that the servo can be detached at the end of any 
// operation to open or close the sliding lid, to address
// the issue of buzzing/clicking/chattering servos
//
// NOTE: This function is safe to call even if the servo is
// already detached. If the servo is not attached, this 
// function will do nothing.
//----------------------------------------------------------
void CCrowboxCore::DetachBasketServo()
{
  if( m_basketServo.attached() )
  {
    m_basketServo.detach();
  }
}

//----------------------------------------------------------
// This is a BLOCKING operation. When you call this function 
// it doesn't return until the lid over the reward basket
// is fully open.
//----------------------------------------------------------
void CCrowboxCore::OpenRewardBasket()
{
    DebugPrint( "  Reward Basket OPENING....\n");

    // Don't bother with executing the state change if we are
    // already in the wished state.
    if( !IsRewardBasketOpen() )
    {
        // Make sure the servo is attached to the signal pin. The last operation
        // involving the servo may have detached it.
        AttachBasketServo();
        
        // For now we just whip the door open!
        m_basketServo.write( SERVO_POS_OPEN );
        
        // Give it time to finish. One second is more than enough time but
        // we need to ensure the servo finishes moving before detaching
        // the servo so we pad the time a little bit.
        delay( 1000 ); 
        DetachBasketServo(); 
    }
    
    // Now we know the basket is open.
    m_basketState = BASKET_STATE_OPEN;
    
    DebugPrint( "Reward Basket is now OPEN!\n" );
}

//----------------------------------------------------------
// This is a BLOCKING operation. When you call this function 
// it doesn't return until the lid over the reward basket
// is fully closed.
//
// As a safety feature, we close the door over a series of 
// small steps so that the closing door will bump into any
// part of a bird which happens to still be in the basket, 
// which hopefully will startle the creature away. This 
// safety feature means it will take several seconds to fully
// close the reward basket, so keep in mind that we'll be 
// stuck in this function for a little while.
//----------------------------------------------------------
void CCrowboxCore::CloseRewardBasket()
{
  DebugPrint( "  Reward Basket CLOSING...\n");
  
  if( IsRewardBasketOpen() )
  {
    // Ensure the basket servo is attached
    AttachBasketServo();
    
    // We can't know the true position of the servo when this
    // method is called so we start by sending the servo to 
    // "full open" position and then delay() long enough for
    // the servo to track to this position from wherever it was
    // before. It's probably already open, but we have to be sure
    // so we have to put it there ourselves.
    m_basketServo.write( SERVO_POS_OPEN );
    delay( 1000 );
  
    // Now we know where the servo is, truly, and can safely set the
    // internal field that tracks position.
    int servoPosition = SERVO_POS_OPEN;
  
    // We're going to close the basket lid over a series of small steps, a 
    // little bit at a time. This gives critters an opportunity to get 
    // their body out of the way before any significant pressure is applied.
    // This is a safety feature that protects the animals that may use
    // this CrowBox. DO NOT alter this behavior unless you're absolutely
    // sure of what you're doing!
    int servoStepSize = servoPosition / BASKET_CLOSE_NUM_STEPS;
  
    while( servoPosition > SERVO_POS_CLOSED )
    {
      servoPosition -= servoStepSize;
      m_basketServo.write( servoPosition );
      delay( BASKET_CLOSE_STEP_DELAY_MS );
      DebugPrint("...basket step...");
    }   
  }

  // Stuff the final closed position
  m_basketServo.write( SERVO_POS_CLOSED );
  delay( 400 );
  m_basketState = BASKET_STATE_CLOSED;
  
  // Any time the sliding basket lid reaches the 'fully open' or 'fully closed'
  // state, we detach the servo from the signal pin. This is an attempt to remedy 
  // situations where some CrowBox users have observed their servos to continue 
  // clicking or whining after the servo has finished moving. Detaching the servo
  // will eliminate the signal that the Arduino is constantly sending to the servo.
  DetachBasketServo();
  
  DebugPrint( "Reward basket closed and locked\n" );
}

//----------------------------------------------------------
// Reads the perch switch hardware directly. This means signal
// is subject to noise from bounce. CrOS is designed to 
// tolerate rapid fluctuations in this switch, so it's not
// necessary to have debouncing hardware.
//----------------------------------------------------------
bool CCrowboxCore::Poll_IsPerchPressed()
{
  int result = digitalRead( INPUT_PIN_PERCH );
  return result == LOW;
}

//----------------------------------------------------------
// Call this method and provide a delay (in seconds). The 
// reward basket will automatically close that many seconds
// later.
//----------------------------------------------------------
void CCrowboxCore::ScheduleBasketCloseWithDelay( cros_time_t delayInSeconds )
{
    m_uptimeScheduledBasketClose = GetUptimeSeconds() + delayInSeconds;
}

//----------------------------------------------------------
// The Rules of Phase One: "Discovery & Free Feeding"
//
//  -Reward Basket always open
//  -Morsels are freely available while supplies last
//  -Coin deposits are not processed or acknowledged
//
//----------------------------------------------------------
void CCrowboxCore::RunPhaseOneProtocol()
{
    if (m_basketManualOverride) return;

    // If a bird is on the perch...
    if (IsABirdOnThePerch()) {
        if (!Poll_IsPerchPressed()) {
            DebugPrint("A customer has left the perch!\n");
            m_uptimeWhenBirdDeparted = GetUptimeSeconds();
        }
    } else {
        if (Poll_IsPerchPressed()) {
            // EDGE CASE: A new bird has arrived!
            DebugPrint("A customer has landed on the perch!\n");
            m_uptimeWhenBirdLanded = GetUptimeSeconds();
            RecordVideo(VIDEO_RECORD_DURATION_ARRIVAL);
            
            // Open basket first
            OpenRewardBasket();
            
            // Then queue IR transmission
            SetPerchPressed();
        }
    }
}

//----------------------------------------------------------
// The Rules of Phase Two: "Reward on Arrival"
//
//  -Reward basket usually closed
//  -Tripping the perch instantly opens the reward basket
//  -Morsels are freely available for a number of seconds
//  -Reward basket closes in a safe, responsible manner
//
//----------------------------------------------------------
void CCrowboxCore::RunPhaseTwoProtocol()
{
    if (m_basketManualOverride) return;

    if (IsABirdOnThePerch()) {
        if (!Poll_IsPerchPressed()) {
            DebugPrint("Customer has left the perch!\n");
            m_uptimeWhenBirdDeparted = GetUptimeSeconds();
        }
    } else {
        if (Poll_IsPerchPressed()) {
            DebugPrint("A customer has landed on the perch!\n");
            m_uptimeWhenBirdLanded = GetUptimeSeconds();
            RecordVideo(VIDEO_RECORD_DURATION_ARRIVAL);
            
            // Open basket first
            OpenRewardBasket();
            ScheduleBasketCloseWithDelay(BASKET_REMAIN_OPEN_DURATION);
            
            // Then queue IR transmission
            SetPerchPressed();
        }
    }
}      

//----------------------------------------------------------
// The rules of Phase Three: "Reward on deposit, coins provided"
//
//  -Reward basket usually closed
//  -'Training Coins' are loaded into the machine 
//  -Reward basket opens ONLY if a coin deposit is detected
//  -Morsels are freely available for a number of seconds
//  -Reward basket closes in a safe, responsible manner
//
// In this phase the machine dispenses training coins onto
// the reward lid so that birds may discover and manipulate
// the training coins until they discover how to use coins
// to receive rewards. 
//----------------------------------------------------------
void CCrowboxCore::RunPhaseThreeProtocol()
{
    if (m_basketManualOverride) return;  // Skip if in manual override
    if( m_numEnqueuedDeposits > 0 && !IsRewardBasketOpen() )
    {
        RemoveEnqueuedCoin();// Un-count this deposit since we're paying it off now.

        OpenRewardBasket();// We're giving out food access in exchange for the deposit

        // Set it up to close.
        ScheduleBasketCloseWithDelay( BASKET_REMAIN_OPEN_DURATION );
    }
}

//----------------------------------------------------------
// The rules of Phase Four: "Reward on deposit"
//
//  -Reward basket usually closed
//  -'Training Coins' NO LONGER provided by the machine
//  -Reward basket opens ONLY if a coin deposit is detected
//  -Morsels are freely available for a number of seconds
//  -Reward basket closes in a safe, responsible manner
//
// This means the birds must locate and carry a coin to the
// Crowbox, as the Crowbox no longer provides training coins.
// This is the ideal steady operating state for an urban
// Crowbox.
//----------------------------------------------------------
void CCrowboxCore::RunPhaseFourProtocol()
{
    if (m_basketManualOverride) return;  // Skip if in manual override
    // Right now the only difference between Phase Three and Phase Four
    // protocols involves the hardware configuration of the Crowbox.
    // The software rules of Phase Four are identical to Phase Three,
    // so we just use those.
    RunPhaseThreeProtocol();
}

//----------------------------------------------------------
// This is not ideal. Ideal would be an interrupt-based 
// check for changes to this switch. However, we've already
// used the only two digital interrupt pins (2,3) on the 
// Arduino UNO for our Crowbox's coin sensor and perch sensor
//
// NOTE: The training phase switch pin (pin4) is pulled UP
// so we need to check to see if it's pulled to ground. If 
// yes, the physical switch is pressed.
//----------------------------------------------------------
void CCrowboxCore::CheckTrainingPhaseSwitch()
{
    if( digitalRead( INPUT_PIN_PHASE_SELECT ) != LOW )
    {
        // Button not depressed- do nothing more.
        return;
    }

    DebugPrint(" Training switch pressed!\n" ); 
    
    // Advance to the next training phase
    AdvanceCurrentTrainingPhase();
    
    // Update the LED matrix display to show new phase
    UpdateMatrixDisplay();
    
    // Delay a bit to debounce the switch
    delay( 500 );
}

//----------------------------------------------------------
// Push ahead to the next training phase. If we pass phase
// four, wrap to phase one.
//----------------------------------------------------------
void CCrowboxCore::AdvanceCurrentTrainingPhase()
{
    // Get the next phase number
    unsigned char nextPhase = m_currentTrainingPhase + 1;
    
    // If we've gone beyond Phase Four, wrap back to Phase One
    if( nextPhase > PHASE_FOUR )
    {
        nextPhase = PHASE_ONE;
    }
    
    // Set the new phase (this will also update the display)
    SetTrainingPhase(nextPhase);
}

//----------------------------------------------------------
// Check the EEPROM data aboard this Arduino board, looking
// to see if the first four characters contain the CrOS header.
// If they do, great, we know that the last information written
// to the EEPROM were written by CrOS. If not, we'll need to
// nuke the EEPROM and write the header.
//----------------------------------------------------------
bool CCrowboxCore::ValidateEEPROMData()
{
  const char *pHeaderCharacter = CROS_EEPROM_HEADER_STRING;

  for( int addr = 0 ; addr < 4 ; ++addr )
  {
    if( *pHeaderCharacter != EEPROM[ addr ] )
    {
      // We found a character in the EEPROM data which does
      // not match the CrOS header. 
      DebugPrint( "EEPROM data header is invalid\n" );
      return false;
    }

    // On to the next character
    pHeaderCharacter++;
  }
  
  // Data header is in order
  DebugPrint( "EEPROM Header Validated\n" );
  return true;
}

//----------------------------------------------------------
// When an Arduino board is brand new, or when it has been
// used in another project which writes EEPROM data, we need
// to write our data header and a temporary training phase 
// (phase one) to the EEPROM to 'make it ours'
//----------------------------------------------------------
void CCrowboxCore::CreateEEPROMData()
{
  DebugPrint( "Creating EEPROM data...\n" );

  const char *pHeaderCharacter = CROS_EEPROM_HEADER_STRING;

  for( int addr = 0 ; addr < 4 ; ++addr )
  {
    EEPROM[ addr ] = *pHeaderCharacter;

    // On to the next character
    pHeaderCharacter++;
  }

  // Immediately after the header we write out a byte with a 
  // value of 1 so that the default for a brand-new Crowbox
  // would be to start in training phase one.
  EEPROM[ CROS_EEPROM_ADDRESS_TRAINING_PHASE ] = PHASE_ONE;

  DebugPrint( "...Done!\n" );
}

//----------------------------------------------------------
// Get the training phase we wrote to EEPROM the last time
// it changed.
//----------------------------------------------------------
void CCrowboxCore::LoadCurrentTrainingPhaseFromEEPROM()
{
  m_currentTrainingPhase = EEPROM.read( CROS_EEPROM_ADDRESS_TRAINING_PHASE );
}

//----------------------------------------------------------
// Save the current training phase in the EEPROM so that it 
// can be restored next time the Crowbox is rebooted.
//
// We use the EEPROM.update() method here because this will
// only actually write to the eeprom if the write value is
// different than what's already there, which saves us from
// wasting write cycles on the eeprom.
//----------------------------------------------------------
void CCrowboxCore::WriteCurrentTrainingPhaseToEEPROM()
{
  EEPROM.update( CROS_EEPROM_ADDRESS_TRAINING_PHASE, m_currentTrainingPhase );
  DebugPrint(" EEPROM Updated!\n" );
}

//----------------------------------------------------------
// The report the training phase, the LED blinks one time to
// indicate "Phase One", two times for "Phase Two", and so on.
//
// NOTE: This is a blocking operation so it's best if the 
// pattern doesn't take very long to emit before the rest of
// the system code can continue running.
//----------------------------------------------------------
void CCrowboxCore::ReportCurrentTrainingPhase()
{
    for(  unsigned char i = 0 ; i < m_currentTrainingPhase ; ++i )
    {
      // Turn the LED on for a moment
      digitalWrite( OUTPUT_PIN_LED, HIGH );
      delay( 250 );

      // Now off for a moment
      digitalWrite( OUTPUT_PIN_LED, LOW );
      delay( 250 );
    }

    // Make sure the LED is off when we are done.
    digitalWrite( OUTPUT_PIN_LED, LOW );
}

//----------------------------------------------------------
// -The Crowbox is going to ask a camera to record video.
//
// -We send in the desired duration of the video recording.
//
// -If the camera is NOT recording, it will begin.
//
// -If the camera IS recording, we'll just push out the 
//  duration so that the recording continues.
//----------------------------------------------------------
void CCrowboxCore::RecordVideo( cros_time_t duration )
{
  // Does nothing presently, but here is where you would 
  // interface with your camera, through a relay or perhaps
  // a serial communication message.
}

//----------------------------------------------------------
//----------------------------------------------------------
void CCrowboxCore::StopRecordingVideo()
{
  // Does nothing presently, but here is where you would 
  // interface with your camera, through a relay or perhaps
  // a serial communication message.
}

//----------------------------------------------------------
// Add this new method implementation
void CCrowboxCore::SetTrainingPhase(unsigned char phase)
{
    // Validate the phase
    if (phase < PHASE_ONE || phase > PHASE_FOUR) {
        ReportSystemError(kError_BadTrainingPhase);
        return;
    }

    // Set the new phase
    m_currentTrainingPhase = phase;
    
    // Store it in EEPROM
    WriteCurrentTrainingPhaseToEEPROM();
    
    // Report the change
    Serial.print("Training phase set to: ");
    Serial.println(phase);
    ReportCurrentTrainingPhase();

    // Reset basket state based on phase
    switch(phase) {
        case PHASE_ONE:
            // Phase 1: Basket stays open
            OpenRewardBasket();
            m_uptimeScheduledBasketClose = TIME_NEVER;
            break;
            
        case PHASE_TWO:
            // Phase 2: Basket stays open when bird is on perch
            if (IsABirdOnThePerch()) {
                OpenRewardBasket();
            } else {
                CloseRewardBasket();
            }
            break;
            
        case PHASE_THREE:
        case PHASE_FOUR:
            // Phases 3 & 4: Basket stays closed until coin deposit
            CloseRewardBasket();
            m_uptimeScheduledBasketClose = TIME_NEVER;
            m_numEnqueuedDeposits = 0;  // Reset any pending rewards
            break;
    }

    UpdateMatrixDisplay();  // Update the LED matrix display
}

//----------------------------------------------------------
// Add this new method implementation
void CCrowboxCore::UpdateMatrixDisplay()
{
    ClearMatrixFrame();
    
    // Copy the appropriate pattern based on state
    const byte (*pattern)[12];
    
    if (m_basketManualOverride) {
        // Only show O or C when in manual override mode
        if (IsRewardBasketOpen()) {
            pattern = PATTERN_O;
        } else {
            pattern = PATTERN_C;
        }
    } else {
        // Show phase number when not in manual override
        switch(m_currentTrainingPhase) {
            case PHASE_ONE:
                pattern = PATTERN_I;
                break;
            case PHASE_TWO:
                pattern = PATTERN_II;
                break;
            case PHASE_THREE:
                pattern = PATTERN_III;
                break;
            case PHASE_FOUR:
                pattern = PATTERN_IIII;
                break;
        }
    }
    
    // Copy pattern to frame buffer
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 12; j++) {
            matrixFrame[i][j] = pattern[i][j];
        }
    }
    
    // Render the frame
    matrix.renderBitmap(matrixFrame, 8, 12);
}

void CCrowboxCore::ClearMatrixFrame() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 12; j++) {
            matrixFrame[i][j] = 0;
        }
    }
}

// Add this new method to send IR codes
void CCrowboxCore::SendIRCode(uint32_t code)
{
    // Completely delete and recreate IR receiver after sending
    if (m_irReceiver != nullptr) {
        delete m_irReceiver;
        m_irReceiver = nullptr;
    }
    
    // Send the code
    IrSender.sendNEC(0x00, code, 32);
    delay(50);  // Brief delay to ensure transmission completes
    
    // Recreate and reinitialize the IR receiver
    m_irReceiver = new IRrecv(INPUT_PIN_IR_RX);
    m_irReceiver->enableIRIn();
    
    Serial.println("IR code sent and receiver reinitialized");
}

