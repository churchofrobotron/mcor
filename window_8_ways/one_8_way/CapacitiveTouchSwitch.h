#ifndef __CAPACITIVETOUCHSWITCH_H__
#define __CAPACITIVETOUCHSWITCH_H__
#include <CapacitiveSensor.h>

#define TEENSY3

#ifdef TEENSY3
class CapacitiveTouchSwitch {
  public:
    CapacitiveTouchSwitch(const uint8_t pin, const int32_t threshold)
    : mPin(pin)
#else
class CapacitiveTouchSwitch : CapacitiveSensor {
  public:
    CapacitiveTouchSwitch(const uint8_t sendPin, const uint8_t receivePin, const uint8_t samples, const int32_t threshold)
    : CapacitiveSensor(sendPin, receivePin)
#endif
    , mInitialized(false)
    , mSwitchState(false)
    , mChanged(false)
    , mLastResult(0)
    , mBaseline(0) 
    , mThreshold(threshold)
    , mHysteresis(DEFAULT_HYSTERESIS)
    , mBaselineCount(0) 
    , mBaselineRate(BASELINE_RATE_DEFAULT)
#ifndef TEENSY3
    , mSamples(samples)
#endif
    , mBaselineAlways(true)
    , mLastTimeOnMs(0)
    , mTimeoutMs(0)
    {
#ifndef TEENSY3
      set_CS_AutocaL_Millis(DEFAULT_AUTOCAL);
#endif
    }

    bool Update() {
      //get raw sensor value
#ifdef TEENSY3
      mLastResult=touchRead(mPin);
#else
      mLastResult=capacitiveSensorRaw(mSamples);
#endif

      //check baseline initialization flag
      //reset baseline value if flag is unset
      if (!mInitialized) {
        mBaseline=mLastResult;
        mInitialized=true;
        mBaselineCount=0;
      }

      //compute sensor level as difference between raw and baseline
      const int32_t difference=mLastResult-mBaseline;

      //adjust baseline
      //Baseline drifts toward result value when switch off or mBaselineAlways set
      //baseline not affected while switch on if mBaselineAlways is false
      //Baseline adjustment rate determined by mBaselineRate
      mBaselineCount = (mBaselineCount+1) % mBaselineRate;
      if (mBaselineCount==0 && (!mSwitchState || mBaselineAlways)) {

        if (difference < -mThreshold) {
          mBaseline=mLastResult;
        } else {
          int32_t baseline_adjust=difference>>BASELINE_SHIFT;
          //make sure adjustment is at least 1 if difference in non-zero
          if (baseline_adjust==0 && difference!=0) {
            baseline_adjust=difference>0 ? 1 : -1;
          }

          mBaseline+=baseline_adjust;
        }
      }

      //Check switch state
      if (!mSwitchState && difference > (mThreshold)) {
        //if switch was off and diff now greater than threshold, turn on
        mSwitchState=true;
        mChanged=true;
        mLastTimeOnMs=millis();
      } else if (mSwitchState && difference < (mThreshold-mHysteresis)) {
        //if was on and diff now less than thresh-hysteresis, turn off
        mSwitchState=false;
        mChanged=true;
      } else {
      }

      //if on, check timeout
      if (mSwitchState && mTimeoutMs!=0) {
        uint32_t time_on_ms=millis()-mLastTimeOnMs;
        if (time_on_ms > mTimeoutMs) {
          mInitialized=false;
        }
      }

      return mSwitchState;
    }

    bool State() const {
      return mSwitchState;
    }

    bool Changed() {
      bool rVal=mChanged;
      mChanged=false;
      return rVal;
    }

    int32_t LastResult() const {
      return mLastResult;
    }

    int32_t Baseline() const {
      return mBaseline;
    }

    void ResetBaseline() {
      mInitialized=0;
    }

    int32_t Diff() const {
      return LastResult() - Baseline();
    }

    uint16_t BaselineRate() const {
      return mBaselineRate;
    }

    void SetBaselineRate (const uint16_t val) {
      mBaselineRate=val;
    }

    int32_t Hysteresis() const {
      return mHysteresis;
    }

    void SetHysteresis(const int32_t val) {
      if (val<0) {
        mHysteresis=0;
      } else {
        mHysteresis=val;
      }
    }

    uint32_t TimeoutMs() const {
      return mTimeoutMs;
    }

    void SetTimeoutMs(const uint32_t val) {
      mTimeoutMs=val;
    }

    bool BaselineAlways() const {
      return mBaselineAlways;
    }

    void SetBaselineAlways(const bool val) {
      mBaselineAlways=val;
    }

  private:
    static const int32_t BASELINE_SHIFT=3;
    static const uint32_t DEFAULT_TIMEOUT=100;
    static const uint32_t DEFAULT_AUTOCAL=0xFFFFFFFF;
    static const int32_t DEFAULT_HYSTERESIS=50;
    static const uint16_t BASELINE_RATE_DEFAULT=50;
    
    uint8_t mPin;

    bool mInitialized; //has the baseline been initialized? Also used for reset.
    bool mSwitchState; //Is the switch off or on?
    bool mChanged; //Did the switch state just change?

    int32_t mLastResult; //Last raw result value from sensor.
    int32_t mBaseline; //Sensor baseline value.
    int32_t mThreshold; //on/off threshold
    int32_t mHysteresis; //on/off hysteresis

    uint16_t mBaselineCount; //counter to control baseline adjustment rate
    uint16_t mBaselineRate; //baseline adjustment counter period

#ifndef TEENSY3
    uint8_t mSamples; //samples value for capacitiveSensorRaw
#endif

    bool mBaselineAlways;

    uint32_t mLastTimeOnMs;
    uint32_t mTimeoutMs;
};

#endif //#ifndef __CAPACITIVETOUCHSWITCH_H__
