#include <FlexiTimer2.h>

/* USB to Serial - Teensy becomes a USB to Serial converter
   http://dorkbotpdx.org/blog/paul/teensy_as_benito_at_57600_baud

   You must select Serial from the "Tools > USB Type" menu

   This example code is in the public domain.
*/


const int reset_pin = 4;
const int led_pin = 11;  // 11=Teensy 2.0, 6=Teensy 1.0, 16=Benito
const int led_on = HIGH;
const int led_off = LOW;

#define MY_TYPE "LAS"
#define MY_ADDR "1"
#define BEAT_TYPE "BEAT"
#define BEAT_ADDR "1"
#define SEPARATOR ":"
String preamble=MY_TYPE MY_ADDR SEPARATOR;
String beat_pre=BEAT_TYPE BEAT_ADDR SEPARATOR;
String command;

//#define DEBUG

const unsigned int num_lasers=9;

const unsigned int baud=9600;

class laser
{
  public:
  laser(unsigned int pin_idx):
  _pin(pin_idx),
  _state(false),
  _pulse_mode(false),
  _off_time(0)
  {
  }

  ~laser() {
    _off();
    pinMode(_pin, INPUT);
  }

  void init() {
    pinMode(_pin, OUTPUT);
    _off();
  }

  void tick () {
    unsigned long time=millis();
    if (_state && ((_pulse_mode && _off_time<=time) || _beat_off<=time))
    {
      _off();
    }
  }

  void heartbeat() {
    _beat_off=millis()+_beat_timeout;
  }

  void pulse()
  {
    if (!_state) {
      _on();
      _pulse_mode=true;
      _off_time=millis()+_pulse_time;
    } else {
      _off();
    }
    heartbeat();
  }

  void steady()
  {
    _on();
    _pulse_mode=false;
    heartbeat();
  }

  void off()
  {
    _off();
    heartbeat();
  }

  private:
  unsigned int _pin;
  bool _state;
  bool _pulse_mode;
  unsigned long _off_time;
  static unsigned long _beat_off; //;-)
  static const unsigned long _beat_timeout=10000;
  static const unsigned int _pulse_time=250;

  void _off() {
    digitalWrite(_pin, LOW);
    _state=false;
  }

  void _on() {
    digitalWrite(_pin, HIGH);
    _state=true;
  }
};
  
unsigned long laser::_beat_off=0;  

laser laser0(0), laser1(1), laser2(2), laser3(3), laser4(4), laser5(5), laser6(6), laser7(7), laser8(8);
laser *lasers[num_lasers]={&laser0,&laser1,&laser2,&laser3,&laser4,&laser5,&laser6,&laser7,&laser8};

void setup()
{
	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, led_off);
	digitalWrite(reset_pin, HIGH);
	pinMode(reset_pin, OUTPUT);
	Serial.begin(baud);	// USB, communication to PC or Mac
  for (unsigned int i=0; i != num_lasers ; ++i)
  {
    lasers[i]->init();
  }
}

void set_lasers(unsigned int mask)
  {
  for (unsigned int i=0; i<num_lasers; ++i)
    {
    if ((mask & 1u) != 0u){
      lasers[i]->steady();
    } else {
      lasers[i]->off();
    }
    mask>>=1;
    }
  }

void random_laser_pulse() {
  static unsigned int last_laser=0;
  unsigned int next_laser=random(0,num_lasers-1);
  if (next_laser>=last_laser) {
    next_laser++;
  }
  lasers[next_laser]->pulse();
#ifdef debug
  Serial.println ("random pulse");
  Serial.println (next_laser,HEX);
#endif
  last_laser=next_laser;
}

void parse_command(const String &cmd)
{
  //command format: <my_type><my_addr>SEPARATOR<
  //check for command preamble
  if (cmd.startsWith(beat_pre))
  {
    for (unsigned int i=0; i!=0 ; ++i) {
      lasers[i]->heartbeat();
    }
#ifdef DEBUG
    Serial.println("good heartbeat");
#endif
  } else if (cmd.startsWith(preamble)) {
    String num=cmd.substring(preamble.length(),preamble.length()+4);
    if (num.startsWith('g') || num.startsWith('G')) {
      random_laser_pulse();
#ifdef DEBUG
      Serial.println("good pulse");
#endif
    } else {
      char buf[4];
      num.toCharArray(buf,4); //up to 4 characters
      unsigned int mask;
      sscanf(buf,"%x",&mask);
      set_lasers(mask);
#ifdef DEBUG
      Serial.println("good mask");
#endif
    }
  } else {
#ifdef debug
    Serial.println("bad preamble");
#endif
  }
}

void loop()
{
  char c;
  //check for new serial characters
  while (Serial.available())
    {
    c=Serial.read();
#ifdef DEBUG
    Serial.print(c);
#endif
    if (c=='\n' || c=='\r')
      {
#ifdef DEBUG
      Serial.println("got command");
      Serial.println(command);
#endif
      parse_command(command);
      command="";
      }
    else
      {command+=c;}
    }
  for (unsigned int i=0; i!=num_lasers; ++i) {
    lasers[i]->tick();
  }
}

