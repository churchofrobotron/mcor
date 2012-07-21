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
#define SEPARATOR ":"
String preamble=MY_TYPE MY_ADDR SEPARATOR;
String command;

//#define DEBUG

const unsigned int num_lasers=10;

const unsigned int baud=9600;

void setup()
{
	pinMode(led_pin, OUTPUT);
	digitalWrite(led_pin, led_off);
	digitalWrite(reset_pin, HIGH);
	pinMode(reset_pin, OUTPUT);
	Serial.begin(baud);	// USB, communication to PC or Mac
  for (int i=num_lasers; i!=0;)
    {
    --i;
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
    }
}

void set_lasers(unsigned int mask)
  {
  for (int i=0; i<num_lasers; ++i)
    {
    digitalWrite(i,(mask & 1) != 0 ? HIGH : LOW);
    mask>>=1;
    }
  }

long led_on_time=0;

void parse_command(const String &cmd)
  {
  //command format: <my_type><my_addr>SEPARATOR<
  //check for command preamble
  if (!cmd.startsWith(preamble))
    {Serial.println("bad preamble");return;}
  
  String num=cmd.substring(preamble.length(),preamble.length()+4);
  
  char buf[4];
  num.toCharArray(buf,4); //up to 4 characters
  unsigned int mask;
  sscanf(buf,"%x",&mask);
  set_lasers(mask);
#ifdef DEBUG
  Serial.println("good command");
  Serial.println(mask,HEX);
#endif
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
}

