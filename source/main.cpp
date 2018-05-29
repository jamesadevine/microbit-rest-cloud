/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "MicroBit.h"
#include "DynamicType.h"
#include "Radio.h"
#include "WeatherService.h"
#include "CloudVariable.h"

MicroBit uBit;
WeatherService s(uBit.radio);
CloudVariable* c;
int i = 0;

void send_get(MicroBitEvent)
{
    uBit.display.scroll(s.getTemperature("lancaster,uk"));
}

void send_post(MicroBitEvent)
{
    int ret = s.setRoomTemperature("C20",uBit.thermometer.getTemperature());
    uBit.display.scroll(ret);
}

void modify_cv(MicroBitEvent)
{
    *c = ManagedString(i);
    i++;
}

void scroll_var(MicroBitEvent)
{
    uBit.display.scroll(c->value);
    i++;
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.radio.enable();

    uBit.display.print('A');
    uBit.serial.printf("HELLO send");

    c = new CloudVariable("school","test",uBit.radio);

    uBit.serial.printf("%d",c->variableNamespaceHash);

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, modify_cv);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, send_get);
    uBit.messageBus.listen(RADIO_CLOUD_VARIABLE_ID, c->variableNameHash, scroll_var);

    release_fiber();
}

