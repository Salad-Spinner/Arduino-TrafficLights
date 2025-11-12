/*
 * CombinedReceiver.cpp
 *
 * Demonstrates receiving NEC protocol IR codes with IRremote and controlling LEDs based on 
 * specific Apple protocol commands.
 * This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 */
 
#include <Arduino.h>
#include <IRremote.hpp> // Include the library
 
// Define IR_RECEIVE_PIN to correspond with Digital Pin 22
#define IR_RECEIVE_PIN 22
 
int RED = 10;      // Define pin for red LED
int GREEN = 11;    // Define pin for green LED
 
void setup() {
    Serial.begin(115200);
 
    // Notification on startup
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
 
    // Start the IR receiver
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
 
    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F(" at pin "));
    Serial.println(String(IR_RECEIVE_PIN));
 
    // Setup LED pins
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
}
 
void loop() {
    if (IrReceiver.decode()) {
        // Print a summary of received data
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or unknown protocol"));
            IrReceiver.printIRResultRawFormatted(&Serial, true);
            IrReceiver.resume();
        } else {
            IrReceiver.resume(); // Enable receiving of the next IR frame
            IrReceiver.printIRResultShort(&Serial);
            IrReceiver.printIRSendUsage(&Serial);
            Serial.println();
 
            // Check received data and perform actions
            if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
                Serial.println(F("Repeat received. Here you can repeat the same action as before."));
            } else {
                handleIRCommands();  // Call function without parameters
            }
        }
    }
}
 
void handleIRCommands() {
    // Check for the specific commands from the Apple protocol
    if (IrReceiver.decodedIRData.protocol == APPLE) {
        if (IrReceiver.decodedIRData.address == 0xF5 && IrReceiver.decodedIRData.command == 0xB) {
            Serial.println(F("Received Apple command 0xB with address 0xF5."));
            red_light();  // Turn on the red LED
        } else if (IrReceiver.decodedIRData.address == 0xF5 && IrReceiver.decodedIRData.command == 0xD) {
            Serial.println(F("Received Apple command 0xD with address 0xF5."));
            green_light();  // Turn on the green LED
        }
    }
}
 
void red_light() {  // A function for turning only the red LED on
    digitalWrite(GREEN, LOW);  // Turning off the green LED
    digitalWrite(RED, HIGH);    // Turning on the red LED
}
 
void green_light() {  // A function for turning only the green LED on
    digitalWrite(GREEN, HIGH);  // Turning on the green LED
    digitalWrite(RED, LOW);      // Turning off the red LED
}
 
void yellow_light() {  // A function for turning both LEDs on (if needed)
    digitalWrite(GREEN, HIGH); // Turning on the green LED
    digitalWrite(RED, HIGH);   // Turning on the red LED
}
