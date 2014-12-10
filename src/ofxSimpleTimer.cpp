//
//  ofxSimpleTimer.cpp
//
//  Created by Ben McChesney on 3/8/13.
//  Maintained and open sourced by

//  Helios Interactive
//  301 8th Street
//  STE 250
//  San Francisco , CA

#include "ofxSimpleTimer.h"

//----------------------------------------------------------------------------
void ofxSimpleTimer::setup(unsigned long delayInMillis, string _name) {
    startTimeMillis = 0.0f;
    delayMillis = delayInMillis;
	name = _name;
}

//----------------------------------------------------------------------------
void ofxSimpleTimer::setup(unsigned long delayInMillis, ArgsTimer _argsTimer, string _name) {
    argsTimer.floats.clear();
    argsTimer.ints.clear();
    argsTimer.bools.clear();
    argsTimer.strings.clear();
    
    argsTimer = _argsTimer;
    
    this->setup(delayInMillis, _name);
}

//----------------------------------------------------------------------------
void ofxSimpleTimer::reset() {
	startTimeMillis = 0.0f ;
    bIsRunning = false ;
	bIsPaused = false;
    
    argsTimer.floats.clear();
    argsTimer.ints.clear();
    argsTimer.bools.clear();
    argsTimer.strings.clear();
}

//----------------------------------------------------------------------------
void ofxSimpleTimer::update() {
    if ( bIsRunning == true ) {
		if (bIsPaused)
			startTimeMillis = ofGetElapsedTimeMillis() - pauseTimeOffset;
        //calculate
        if( (ofGetElapsedTimeMillis() - startTimeMillis) > delayMillis )
        {
            //ofLog( OF_LOG_VERBOSE , name + " ofxSimpleTimer::update() TIMER COMPLETE" ) ; 
            bIsRunning = false ;
            if ( bLoop == true )
                start( bLoop ) ;
            
            ofNotifyEvent( TIMER_COMPLETE , argsTimer ) ;
        }
    }
}

//----------------------------------------------------------------------------
void ofxSimpleTimer::draw( float x , float y ) {
    string debugString = "" ;
    if ( bIsRunning )
    {
        float timeLeft = delayMillis - (ofGetElapsedTimeMillis() - startTimeMillis) ;
        debugString = " RUNNING ! " + ofToString( timeLeft ) + " % : " + ofToString( getNormalizedProgress() ) ;
    }
    else
    {
        debugString = " not running " ;
    }
    
    ofDrawBitmapStringHighlight( debugString , x , y ) ; 
}

//----------------------------------------------------------------------------
void ofxSimpleTimer::start( bool _bLoop , bool bForceReset ) {
	//ofLog( OF_LOG_VERBOSE , "ofxSimpleTimer::start() " ) ;

    //We don't want to start the timer if it's already running
    bool bStartTimer = !bIsRunning ;
   	 
    //Unless we specifically want it too
    if ( bForceReset == true ) {
        bStartTimer = true ;
    }
    
    if ( bStartTimer == true ) {
        bIsRunning = true ;
		bIsPaused = false;
        startTimeMillis = ofGetElapsedTimeMillis() ;
        bLoop = _bLoop ;

        ofNotifyEvent( TIMER_STARTED , argsTimer ) ;
        //ofLog( OF_LOG_VERBOSE , name + " ofxSimpleTimer::start() " ) ;
    }
}

//----------------------------------------------------------------------------
void ofxSimpleTimer::stop() {
   //We don't want to start the timer if it's already running
   //if ( bIsRunning == true ) 
	//	ofLog( OF_LOG_VERBOSE , "ofxSimpleTimer::stop() " ) ;
	
	bIsRunning = false ; 
	bIsPaused = false;
}

//----------------------------------------------------------------------------
void ofxSimpleTimer::togglePause() {
	cout << "is timer running? " << bIsRunning << ":" << bIsPaused << endl;
    
    if (bIsRunning) {
		bIsPaused = !bIsPaused;
    }

	if (bIsPaused) {
		pauseTimeOffset = ofGetElapsedTimeMillis() - startTimeMillis;
        ofNotifyEvent( TIMER_PAUSED , argsTimer) ;
    }

    if (!bIsPaused) {
		startTimeMillis = ofGetElapsedTimeMillis() - pauseTimeOffset;
    }

	cout << "is timer running? " << bIsRunning << ":" << bIsPaused << endl;
	cout << "timing: " << ofGetElapsedTimeMillis() << ":" << startTimeMillis << endl;
}

//----------------------------------------------------------------------------
float ofxSimpleTimer::getNormalizedProgress ( ) {
    if ( !bIsRunning ) {
        return 0.0f ;
    }

	float nDiff = delayMillis - (ofGetElapsedTimeMillis() - startTimeMillis) ;
	float percent = (float)nDiff / (float)delayMillis ; 
	return ( 1.0f - percent ) ; 
}