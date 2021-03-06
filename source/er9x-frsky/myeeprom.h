/*
 * Author - Erez Raviv <erezraviv@gmail.com>
 *
 * Based on th9x -> http://code.google.com/p/th9x/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef eeprom_h
#define eeprom_h

#include <inttypes.h>

#ifndef PACK
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

//eeprom data
//#define EE_VERSION 2
#define MAX_MODELS  16
#define MAX_MIXERS  32
#define MAX_CURVE5  8
#define MAX_CURVE9  8
#define MDVERS_r9   1
#define MDVERS_r14  2
#define MDVERS_r22  3
#define MDVERS_r77  4
#define MDVERS_r85  5
#define MDVERS_r261 6
#define MDVERS_r352 7
#define MDVERS_r365 8
#define MDVERS_r668 9
#define MDVERS_r803 10
#define MDVERS      11

//OBSOLETE - USE ONLY MDVERS NOW
//#define GENERAL_MYVER_r261 3
//#define GENERAL_MYVER_r365 4
//#define GENERAL_MYVER      5


// eeprom ver <9 => mdvers == 1
// eeprom ver >9 => mdvers ==2

#define WARN_THR_BIT  0x01
#define WARN_BEP_BIT  0x80
#define WARN_SW_BIT   0x02
#define WARN_MEM_BIT  0x04
#define WARN_BVAL_BIT 0x38

#define WARN_THR     (!(g_eeGeneral.warnOpts & WARN_THR_BIT))
#define WARN_BEP     (!(g_eeGeneral.warnOpts & WARN_BEP_BIT))
#define WARN_SW      (!(g_eeGeneral.warnOpts & WARN_SW_BIT))
#define WARN_MEM     (!(g_eeGeneral.warnOpts & WARN_MEM_BIT))
#define BEEP_VAL     ( (g_eeGeneral.warnOpts & WARN_BVAL_BIT) >>3 )

#define GENERAL_OWNER_NAME_LEN 10
#define MODEL_NAME_LEN         10

#define MAX_GVARS 7

#define MAX_MODES		4

PACK(typedef struct t_TrainerMix {
    uint8_t srcChn:3; //0-7 = ch1-8
    int8_t  swtch:5;
    int8_t  studWeight:6;
    uint8_t mode:2;   //off,add-mode,subst-mode
})  TrainerMix; //

PACK(typedef struct t_TrainerData {
    int16_t        calib[4];
    TrainerMix     mix[4];
}) TrainerData;

PACK(typedef struct t_EEGeneral {
    uint8_t   myVers;
    int16_t   calibMid[7];
    int16_t   calibSpanNeg[7];
    int16_t   calibSpanPos[7];
    uint16_t  chkSum;
    uint8_t   currModel; //0..15
    uint8_t   contrast;
    uint8_t   vBatWarn;
    int8_t    vBatCalib;
    int8_t    lightSw;
    TrainerData trainer;
    uint8_t   view;
    uint8_t   disableThrottleWarning:1;
    uint8_t   disableSwitchWarning:1;
    uint8_t   disableMemoryWarning:1;
    uint8_t   beeperVal:3;
    uint8_t   reserveWarning:1;
    uint8_t   disableAlarmWarning:1;
    uint8_t   stickMode;
    int8_t    inactivityTimer;
    uint8_t   throttleReversed:1;
    uint8_t   minuteBeep:1;
    uint8_t   preBeep:1;
    uint8_t   flashBeep:1;
    uint8_t   disableSplashScreen:1;
    uint8_t   disablePotScroll:1;
    uint8_t   disableBG:1;
    uint8_t   frskyinternalalarm:1;
    uint8_t   spare_filter ;		// No longer needed, left for eepe compatibility for now
    uint8_t   lightAutoOff;
    uint8_t   templateSetup;  //RETA order according to chout_ar array
    int8_t    PPM_Multiplier;
    uint8_t   unused1;
    uint8_t   unused2:4;
    uint8_t   hideNameOnSplash:1;
    uint8_t   enablePpmsim:1;
    uint8_t   blightinv:1;
    uint8_t   stickScroll:1;
    uint8_t   speakerPitch;
    uint8_t   hapticStrength;
    uint8_t   speakerMode;
    uint8_t   lightOnStickMove;
    char      ownerName[GENERAL_OWNER_NAME_LEN];
    uint8_t   switchWarningStates;
		int8_t		volume ;
    uint8_t   res[3];
    uint8_t   crosstrim:1 ;
    uint8_t   FrskyPins:1 ;
    uint8_t   rotateScreen:1 ;
    uint8_t   serialLCD:1 ;
    uint8_t   SSD1306:1 ;
    uint8_t   spare1:3 ;
		uint8_t		stickReverse ;
}) EEGeneral;





//eeprom modelspec
//expo[3][2][2] //[Norm/Dr][expo/weight][R/L]

PACK(typedef struct t_ExpoData {
    int8_t  expo[3][2][2];
    int8_t  drSw1;
    int8_t  drSw2;
}) ExpoData;


PACK(typedef struct t_LimitData {
    int8_t  min;
    int8_t  max;
    bool    reverse;
    int16_t  offset;
}) LimitData;

#define MLTPX_ADD  0
#define MLTPX_MUL  1
#define MLTPX_REP  2

PACK(typedef struct t_MixData {
    uint8_t destCh;            //        1..NUM_CHNOUT
    uint8_t srcRaw;            //
    int8_t  weight;
    int8_t  swtch;
    int8_t curve;             //0=symmetrisch 1=no neg 2=no pos
    uint8_t delayUp:4;
    uint8_t delayDown:4;
    uint8_t speedUp:4;         // Servogeschwindigkeit aus Tabelle (10ms Cycle)
    uint8_t speedDown:4;       // 0 nichts
    uint8_t carryTrim:1;
    uint8_t mltpx:2;           // multiplex method 0=+ 1=* 2=replace
    uint8_t lateOffset:1;      // Add offset later
    uint8_t mixWarn:2;         // mixer warning
#ifdef FMODE_TRIM
    uint8_t enableFmTrim:1;
#else
    uint8_t spareenableFmTrim:1;
#endif
    uint8_t differential:1;
    int8_t  sOffset;
		uint8_t modeControl:5 ;
    uint8_t res:3 ;
}) MixData;


PACK(typedef struct t_CSwData { // Custom Switches data
    int8_t  v1; //input
    int8_t  v2; //offset
    uint8_t func:4;
    uint8_t andsw:4;
}) CSwData;

PACK(typedef struct t_CxSwData { // Extra Custom Switches data
    int8_t  v1; //input
    int8_t  v2; //offset
    uint8_t func ;
    int8_t andsw ;
}) CxSwData ;

PACK(typedef struct t_SafetySwData { // Custom Switches data
	union opt
	{
		struct ss
		{	
	    int8_t  swtch:6;
			uint8_t mode:2;
    	int8_t  val;
		} ss ;
		struct vs
		{
  		uint8_t vswtch:5 ;
			uint8_t vmode:3 ; // ON, OFF, BOTH, 15Secs, 30Secs, 60Secs, Varibl
    	uint8_t vval;
		} vs ;
	} opt ;
}) SafetySwData;

PACK(typedef struct t_FrSkyChannelData
{
	union opt
	{
		struct alarm
		{
    	uint8_t   ratio;                // 0.0 means not used, 0.1V steps EG. 6.6 Volts = 66. 25.1V = 251, etc.
    	uint8_t   alarms_value[2];      // 0.1V steps EG. 6.6 Volts = 66. 25.1V = 251, etc.
    	uint8_t   alarms_level:4;
    	uint8_t   alarms_greater:2;     // 0=LT(<), 1=GT(>)
    	uint8_t   type:2;               // 0=volts, 1=raw, 2=volts*2, 3=Amps
		} alarm ;
		struct scale
		{
    	uint8_t   ratio ;       	// 0.0 means not used, 0.1V steps EG. 6.6 Volts = 66. 25.1V = 251, etc.
    	uint8_t   offset ;    	  // 0.0 means not used, 0.1V steps EG. 6.6 Volts = 66. 25.1V = 251, etc.
			uint8_t   units ;					// 0.1V steps EG. 6.6 Volts = 66. 25.1V = 251, etc.
    	uint8_t   multiplier:3 ;  // 1, 2, 3 or 4
    	uint8_t   unused1:1 ;
    	uint8_t   decimals:2 ;		// 0, 1 or 2
    	uint8_t   type:2;       	// 0=volts, 1=raw, 2=volts*2, 3=Amps
		} scale ;
	} opt ;
}) FrSkyChannelData;

//PACK(typedef struct t_FrSkyalarms
//{
//	uint8_t frskyAlarmType ;
//	uint8_t frskyAlarmLimit ;
//	uint8_t frskyAlarmSound ;
//}) FrSkyAlarmData;

PACK(typedef struct t_FrSkyData {
    FrSkyChannelData channels[2];
		uint8_t voltSource:2 ;
		uint8_t ampSource:2 ;
		uint8_t FASoffset:4 ;			// 0.0 to 1.5
		uint8_t frskyAlarmLimit ;		// For mAh
		uint8_t frskyAlarmSound ;		// For mAh
//		FrSkyAlarmData alarmData[4] ;
}) FrSkyData;

PACK(typedef struct t_gvar {
	int8_t gvar ;
	uint8_t gvsource ;
//	int8_t gvswitch ;
}) GvarData ;

PACK(typedef struct t_PhaseData {
	// Trim store as -1001 to -1, trim value-501, 0-5 use trim of phase 0-5
  int16_t trim[4];     // -500..500 => trim value, 501 => use trim of phase 0, 502, 503, 504 => use trim of modes 1|2|3|4 instead
  int8_t swtch;        // Try 0-5 use trim of phase 0-5, 1000-2000, trim + 1500 ???
  uint8_t fadeIn:4;
  uint8_t fadeOut:4;
}) PhaseData;
	 
PACK(typedef struct t_FunctionData { // Function data
  int8_t  swtch ; //input
  uint8_t func ;
  uint8_t param ;
}) FunctionData ;

PACK(typedef struct t_Vario
{
  uint8_t varioSource ;
  int8_t  swtch ;
  uint8_t sinkTones:1 ;
  uint8_t spare:1 ;
  uint8_t param:6 ;
}) VarioData ;


//PACK(typedef struct t_swVoice {
//  uint8_t  vswtch:5 ;
//	uint8_t vmode:3 ; // ON, OFF, BOTH, 15Secs, 30Secs, 60Secs, Varibl
//  uint8_t  val ;
//}) voiceSwData ;

// Scale a value
PACK(typedef struct t_scale
{
  uint8_t source ;
	int16_t offset ;
	uint8_t mult ;
	uint8_t div ;
	uint8_t unit ;
	uint8_t neg:1 ;
	uint8_t precision:2 ;
	uint8_t offsetLast:1 ;
	uint8_t spare:4 ;
	uint8_t name[4] ;
}) ScaleData ;

PACK(typedef struct t_ModelData {
    char      name[MODEL_NAME_LEN];             // 10 must be first for eeLoadModelName
//    uint8_t   reserved_spare;  //used to be MDVERS - now depreciated
    uint8_t   modelVoice ;		// Index to model name voice (260+value)
    int8_t    tmrMode;              // timer trigger source -> off, abs, stk, stk%, sw/!sw, !m_sw/!m_sw
    uint8_t   tmrDir:1;    //0=>Count Down, 1=>Count Up
    uint8_t   traineron:1;  // 0 disable trainer, 1 allow trainer
    uint8_t   t2throttle:1 ;  // Start timer2 using throttle
    uint8_t   FrSkyUsrProto:1 ;  // Protocol in FrSky User Data, 0=FrSky Hub, 1=WS HowHigh
    uint8_t   FrSkyGpsAlt:1 ;  	// Use Gps Altitude as main altitude reading
    uint8_t   FrSkyImperial:1 ;  // Convert FrSky values to imperial units
    uint8_t   FrSkyAltAlarm:2;
    uint16_t  tmrVal;
    uint8_t   protocol:4 ;
    uint8_t   country:2 ;
    uint8_t   sub_protocol:2 ;
    int8_t    ppmNCH;
    uint8_t   thrTrim:1;            // Enable Throttle Trim
		uint8_t   xnumBlades:2;					// RPM scaling
		uint8_t   mixTime:1 ;						// Scaling for slow/delay
    uint8_t   thrExpo:1;            // Enable Throttle Expo
		uint8_t   ppmStart:3 ;					// Start channel for PPM
    int8_t    trimInc;              // Trim Increments (0-4)
    int8_t    ppmDelay;
    int8_t    trimSw;
    uint8_t   beepANACenter;        // 1<<0->A1.. 1<<6->A7
    uint8_t   pulsePol:1;
    uint8_t   extendedLimits:1;
    uint8_t   swashInvertELE:1;
    uint8_t   swashInvertAIL:1;
    uint8_t   swashInvertCOL:1;
    uint8_t   swashType:3;
    uint8_t   swashCollectiveSource;
    uint8_t   swashRingValue;
    int8_t    ppmFrameLength;    //0=22.5  (10msec-30msec) 0.5msec increments
    MixData   mixData[MAX_MIXERS];
    LimitData limitData[NUM_CHNOUT];
    ExpoData  expoData[4];
    int8_t    trim[4];
    int8_t    curves5[MAX_CURVE5][5];
    int8_t    curves9[MAX_CURVE9][9];
    CSwData   customSw[NUM_CSW];
    uint8_t   frSkyVoltThreshold ;
    int8_t		tmrModeB;
    uint8_t   numVoice:5;		// 0-16, rest are Safety switches
		uint8_t		anaVolume:3 ;	// analog volume control
    SafetySwData  safetySw[NUM_CHNOUT];
    FrSkyData frsky;
		uint8_t numBlades ;
		uint8_t frskyoffset[2] ;		// Offsets for A1 and A2 (pending)
		uint8_t Mavlink;			    	// TODO: 1. shrink to 2 bit, 2. eePe
		uint8_t unused5[4] ;
		uint8_t sub_trim_limit ;
		uint8_t CustomDisplayIndex[6] ;
		GvarData gvars[MAX_GVARS] ;
		PhaseData phaseData[MAX_MODES] ;
		VarioData varioData ;
		uint8_t modelVersion ;
		int8_t pxxFailsafe[16] ;
		SafetySwData xvoiceSw[EXTRA_VOICE_SW] ;
    CxSwData xcustomSw[EXTRA_CSW];
	uint8_t   currentSource ;
	uint8_t   altSource ;
	uint8_t phaseNames[MAX_MODES][6] ;
	ScaleData Scalers[NUM_SCALERS] ;

//		uint8_t   altSource ;
}) ModelData;

#define TOTAL_EEPROM_USAGE (sizeof(ModelData)*MAX_MODELS + sizeof(EEGeneral))


extern EEGeneral g_eeGeneral;
extern ModelData g_model;











#endif
/*eof*/



