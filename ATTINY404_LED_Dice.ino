/**
 **************************************************
 *
 * @file        ATTINY404_LED_Dice.ino
 * @brief       Arduino code for LED Dice
 *              Soldering Kit from Soldered.
 *
 * @note        In order to use this code, install megaTinyCore from https://github.com/SpenceKonde/megaTinyCore
 *              Core version: 2.4.2
 *              Chip: ATtiny404
 *              Clock: 1MHz Internal
 *              millis()/micros Timer: Enabled (default timer)
 *
 * @authors     Borna Biro for soldered.com
 ***************************************************/

// Define pins (names are actually nets from KiCAD SCH of this kit).
#define NET_D0 PIN_PA4
#define NET_D1 PIN_PA5
#define NET_D2 PIN_PA6
#define NET_D3 PIN_PA7
#define NET_D4 PIN_PA2

// Interrupt flag
volatile uint8_t intFlag = 0;

// Variable that keeps track on how long the dice will keep "rolling".
uint8_t rollCounter = 0;

void setup()
{
    // First, set pins connected to LEDs to outputs.
    pinMode(NET_D0, OUTPUT);
    pinMode(NET_D1, OUTPUT);
    pinMode(NET_D2, OUTPUT);
    pinMode(NET_D3, OUTPUT);

    // Set pin connected to vibration sensor as input.
    pinMode(NET_D4, INPUT);

    // Attach interrput on the same pin, so we don't lose any rising edge.
    attachInterrupt(digitalPinToInterrupt(NET_D4), interruptFunction, RISING);

    // Init the pseudo-random number generator.
    randomSeed((analogRead(6) << 10) | analogRead(7));

    // Show some random number at start.
    showNumber(random(0, 5));
}

void loop()
{
    // If there is a event from vibration sensor, roll the dice!
    if (intFlag)
    {
        // Generate random number of dice rolls.
        randomSeed(random());
        rollCounter = random(2, 20);

        while (rollCounter)
        {
            if (intFlag) // Each time when you hit the dice again, while it's rolling, add 2 to 7 more rolls, but no
                         // more than 50 rolls in total.
            {
                intFlag = 0;
                rollCounter += random(2, 7);
                if (rollCounter > 50)
                    rollCounter = 50;
            }

            // Generate random number from 0 to 255 and use middle bits (it's more random I guess).
            uint8_t rnd = random(0, 255);
            rnd >>= 3;
            rnd %= 6;

            // Display the number.
            showNumber(rnd);

            // Wait a little and decrement the roll counter.
            delay(50);
            rollCounter--;
        }
    }
}

/**
 * @brief       Displays the number on LEDs.
 *
 * @param       uint8_t _n
 *              Dice number that has to be showed (0 means 1 on dice, 1 menas 2 on dice,...).
 */
void showNumber(uint8_t _n)
{
    turnOffLeds();
    switch (_n)
    {
    case 0:
        digitalWrite(NET_D0, HIGH);
        break;

    case 1:
        digitalWrite(NET_D2, HIGH);
        break;

    case 2:
        digitalWrite(NET_D0, HIGH);
        digitalWrite(NET_D2, HIGH);
        break;

    case 3:
        digitalWrite(NET_D1, HIGH);
        digitalWrite(NET_D2, HIGH);
        break;

    case 4:
        digitalWrite(NET_D0, HIGH);
        digitalWrite(NET_D1, HIGH);
        digitalWrite(NET_D2, HIGH);
        break;

    case 5:
        digitalWrite(NET_D1, HIGH);
        digitalWrite(NET_D2, HIGH);
        digitalWrite(NET_D3, HIGH);
        break;
    }
}

/**
 * @brief       Turns off all LEDs.
 */
void turnOffLeds()
{
    digitalWrite(NET_D0, LOW);
    digitalWrite(NET_D1, LOW);
    digitalWrite(NET_D2, LOW);
    digitalWrite(NET_D3, LOW);
}

/**
 * @brief       Function called when interrup on pin PA2 happens.
 */
void interruptFunction()
{
    intFlag = 1;
}
