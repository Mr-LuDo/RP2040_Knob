
#include <Arduino.h>

#include "MyKnob.hpp"


void setup() {

	Serial.begin(115200, true);
	sleep_ms(3000);

	Serial.println("setup done");

}

void loop() {

	static uint32_t counter = 0;
	myKnob.updateKnobPossition();
	myKnob.setReqPrint();

	static bool encoder_changed = false;
	encoder_changed = myKnob.encoderStatus();
	if (encoder_changed)
		myKnob.printEncoder();

	if (myKnob.reqPrint()) {
		Serial.print(counter);
		Serial.print(". ");
		
		myKnob.printDirection();
		++counter;
	}

	sleep_ms(10);

}