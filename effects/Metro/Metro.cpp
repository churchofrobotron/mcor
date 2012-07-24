
#include "WProgram.h"
#include "Metro.h"


Metro::Metro(unsigned long interval_millis)
{
	interval(interval_millis);
	reset();
}

// New creator so I can use either the original check behavior or benjamin.soelberg's
// suggested one (see below)

Metro::Metro(unsigned long interval_millis, uint8_t autoreset)
{   
	this->autoreset = autoreset;
	interval(interval_millis);
	reset();
}

void Metro::interval(unsigned long interval_millis)
{
  this->interval_millis = interval_millis;
}

char Metro::check()
{
  if (millis() - this->previous_millis >= this->interval_millis) {
    // As suggested by benjamin.soelberg@gmail.com, the following line 
    // this->previous_millis = millis();
    // was changed to
    // this->previous_millis += this->interval_millis;
    
    // But then a bug would sometimes occur when the interval was set with random, 
    // so I added the following check to reinstate the previous behavior, but I am 
    // not sure this fixes it
    
    if (this->interval_millis == 0 || this->autoreset ) {
    	this->previous_millis = millis();
	} else {
		this->previous_millis += this->interval_millis; 
	}
    
    return 1;
  }
  
  
  
  return 0;

}

void Metro::reset() 
{
 
  this->previous_millis = millis();

}


