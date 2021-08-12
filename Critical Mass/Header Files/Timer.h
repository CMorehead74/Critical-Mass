//=============================================================================
//=============================================================================
// Timer.h
// 
// Class for logging gametime.
//=============================================================================
//=============================================================================
#ifndef _CTIMER_H_
#define _CTIMER_H_

// microsoft
#include "windows.h"


//=============================================================================
// CONSTANTS, MACROS, DEFINITIONS
//=============================================================================

//=============================================================================
// CLASSES
//=============================================================================

///////////////////////////////////////////////////////////////////////////////
// Timer
//
// Class for logging times.
///////////////////////////////////////////////////////////////////////////////
class CTimer
{
private:
    int             fps;
    float           timeStart;
    float           timeDelta;
    LARGE_INTEGER   tickFrequency;

public:
    ///////////////////////////////////////////////////////////////////////////////
    // Constructor list.
    ///////////////////////////////////////////////////////////////////////////////
    CTimer( void );
    ~CTimer( void );

    ///////////////////////////////////////////////////////////////////////////////
    // Use to initialize & to start timer.
    // All time calculations are calculated from game time at this point.
    ///////////////////////////////////////////////////////////////////////////////
    void Reset( void );

    ///////////////////////////////////////////////////////////////////////////////
    // Use to update timer.
    // ONLY CALL ONCE PER FRAME!!!
    ///////////////////////////////////////////////////////////////////////////////
    void Update( void );

    ///////////////////////////////////////////////////////////////////////////////
    // Returns ( in seconds ) the amount of time from last Reset call.
    ///////////////////////////////////////////////////////////////////////////////
    float GetTime( void );

    ///////////////////////////////////////////////////////////////////////////////
    // Returns ( in seconds ) the amount of time from last frame.
    ///////////////////////////////////////////////////////////////////////////////
    float GetDeltaTime( void );

    ///////////////////////////////////////////////////////////////////////////////
    // Returns the number of FRAMES PER SECOND.
    ///////////////////////////////////////////////////////////////////////////////
    int GetFPS( void );
};

#endif