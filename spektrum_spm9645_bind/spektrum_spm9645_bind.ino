
/***************************************************************************
# Spektrum SPM9645 binding using an Arduino
# Copyright (c) 2018, Kjeld Jensen <kjen@mmmi.sdu.dk> <kj@kjen.dk>
# SDU UAS Center, http://sdu.dk/uas 
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name of the copyright holder nor the names of its
#      contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#****************************************************************************

This source code is based on documentation obtained from:
https://www.spektrumrc.com/ProdInfo/Files/Remote%20Receiver%20Interfacing%20Rev%20A.pdf

Revision
2018-05-01 KJ First released version
****************************************************************************/
/* parameters */

// Define the number of pulses sent to the Spektrum RX. The number determines
// the bind types accepted by the receiver. For more info please see the
// documentation referenced above.
// 9 pulses: DSMX 11(or 22) ms recommended by Horizon Hobby, LLC
// Tested on Spektrum DX5e, DX9 and OrangeRX TX module.
#define NUM_BIND_PULSES 9

// The Spektrum RX consumes approx. 20mA according to Horizon Hobby so it
// can be connected directly to an Arduino digital port.
#define PIN_RX_POWER 2

// Using the digital port 0 (serial RX) as bind port enables the arduino to
// relay data outputted by the Spektrum RX after a succesful binding.
#define PIN_RX_BIND 0 

/***************************************************************************/
/* global variables */
unsigned long count;

/***************************************************************************/
void spektrum_send_pulses (char num)
{
  char i;
  
  for (i=0; i<num; i++)
  {
    // no documentation of requirements for pulse width but 200us and a
    // duty cycle of 50% was tested succesfully
    digitalWrite(PIN_RX_BIND, LOW);
    delayMicroseconds(100);
    digitalWrite(PIN_RX_BIND, HIGH);
    delayMicroseconds(100);
  }
}
/***************************************************************************/
void setup()
{
  // setup digital output pins
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(PIN_RX_POWER, OUTPUT); 

  // power on the Spektrum RX
  digitalWrite(PIN_RX_POWER, HIGH);

  // put the Spektrum RX into bind mode
  // pulses must be sent within 200ms of Spektrum RX powerup
  pinMode(PIN_RX_BIND, OUTPUT); 
  delay(100); 
  spektrum_send_pulses(NUM_BIND_PULSES);
  pinMode(PIN_RX_BIND, INPUT);

  // now prepare for monitoring serial data from the Spektrum RX
  Serial.begin(115200);
}
/***************************************************************************/
void loop()
{
  count ++;

  // update LED flashing (to show that the software is running)
  if (count % 10000 == 0)
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
 
  // relay serial data from the Spektrum RX to the Arduino serial TX.
  if (Serial.available() > 0)
  {
    Serial.write(Serial.read()); 
  }
}
/***************************************************************************/

