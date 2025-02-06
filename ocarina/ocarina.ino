#include<Wire.h>
#include <Adafruit_BMP085.h>

#define POT_PIN 26
#define A44 69
#define FIRST_PIN 6

Adafruit_BMP085 bmp;

int blow_low = 99800;
int blow_high = 101000;

float baseline_velocity = 0;
float velocity_range = 0;

const int scales[8] = {
    {0, 2, 4, 5, 7, 9, 11, 12}, // major
    {0, 2, 3, 5, 7, 8, 10, 12}, // minor
};


int base = A44;
int scale = 0;
int channel = 5;
int control = 1;

// todo add some setting mode

/**
 * @author Oliver Torr
 * todo led indications
 */
void calibrate_bmp() 
{
    float readings[50];
    int index = 0;
    unsigned long start_time = millis();

    while (millis() - start_time < 10000) {
        if (index < 50) {
            readings[index++] = bmp.readPressure();
        }
        delay(100);  
    }

    float sum = 0;
    for (int i = 0; i < index; i++) {
        sum += readings[i];
    }
    baseline_velocity = (index > 0) ? sum / index : bmp.readPressure();

    float max_deviation = 0;
    for (int i = 0; i < index; i++) {
        float deviation = abs(readings[i] - baseline_velocity);
        if (deviation > max_deviation) {
            max_deviation = deviation;
        }
    }
    velocity_range = max_deviation;
}

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(31250);
  bmp.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

int last = 0;

int note = 0;
int last_velocity = 0;
int last_pot = 0;

void loop() 
{
    int current = 0;
    for (int i = 0; i < 4; i++) {
        if (digitalRead(FIRST_PIN + i) == HIGH) {
            current |= 1 << i;
        }
    }
    
    float pressure = bmp.readPressure();
    int velocity = max(0, min(127, int(((pressure - baseline_pressure) / pressure_range) * 127))); // todo rewrite 

    if (current != last || velocity != last_velocity) {
        last = current;
        last_velocity = velocity;
        noteOn(
            channel, 
            base + scales[scale][last], 
            velocity);
    }

    int pot = analogRead(POT_PIN);
    if (pot != last_pot) {
        cc(channel, control, map(pot, 0, 1023, 0, 127));
    }


}

void noteOn(int channel, int note, int velocity)
{
  Serial1.write(0b10010000 | channel);
  Serial1.write(note);
  Serial1.write(velocity);
}

void cc(int channel, int control, int value)
{
  Serial1.write(0b10110000 | channel);
  Serial1.write(control);
  Serial1.write(value);
}
