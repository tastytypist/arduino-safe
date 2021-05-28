// Initialisation
#include <LiquidCrystal.h>
#include <Servo.h>
Servo servo_0;

// Declaration for pins
const int button_1 = A0;
const int button_2 = A1;
const int button_3 = A2;
const int button_4 = A3;
const int photo_pin = A4;

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int piezo_pin = 8;
const int pir_pin = 9;
const int servo_pin = 10;
const int red_pin = 11;
const int blue_pin = 12;
const int green_pin = 13;

// Length and frequencies of alarm
const int alarm_length = 900000;
const int tone_1 = 600;
const int tone_2 = 450;

// Output frequencies for button inputs
const float c_sharp = 138.59132;
const float e = 164.81378;
const float f_sharp = 184.99721;
const float g_sharp = 207.65235;

int position = 0;
int pir_state, photo_value;

int note_code[] = {A2, A0, A3, A1};  // Correct note combination
int note_entered[4];  // Initiate empty array for note input

void setup() {
    lcd.begin(16, 2);
    lcd.print("Standby");
    
    servo_0.attach(servo_pin);
    servo_0.write(position);
    
    pinMode(button_1, INPUT);
    pinMode(button_2, INPUT);
    pinMode(button_3, INPUT);
    pinMode(button_4, INPUT);
    pinMode(pir_pin, INPUT);
    pinMode(photo_pin, INPUT);
    
    pinMode(piezo_pin, OUTPUT);
    pinMode(servo_pin, OUTPUT);
    pinMode(red_pin, OUTPUT);
    pinMode(blue_pin, OUTPUT);
    pinMode(green_pin, OUTPUT);
    
    // Set LED to yellow -> standby mode
    digitalWrite(red_pin, HIGH);
    digitalWrite(green_pin, HIGH);
}

// Main loop for Arduino
void loop() {
    pir_state = digitalRead(pir_pin);
    photo_value = analogRead(photo_pin);
    
    if (pir_state != LOW and photo_value > 127) {
        digitalWrite(green_pin, LOW);
        lcd.clear();
        lcd.print("Intruder Alert!");
        
        for (int start = millis(); (millis() - start) < alarm_length;) {
            tone(piezo_pin, tone_1, 2000);
            tone(piezo_pin, tone_2, 2000);
        }
        
        digitalWrite(green_pin, HIGH);
        lcd.clear();
        lcd.print("Standby");
    }
    else if (digitalRead(button_1) == HIGH) {
        tone(piezo_pin, c_sharp, 500);
        enter_check(button_1, 0);
        delay(500);
    }
    else if (digitalRead(button_2) == HIGH) {
        tone(piezo_pin, e, 500);
        enter_check(button_2, 0);
        delay(500);
    }
    else if (digitalRead(button_3) == HIGH) {
        tone(piezo_pin, f_sharp, 500);
        enter_check(button_3, 0);
        delay(500);
    }
    else if (digitalRead(button_4) == HIGH) {
        tone(piezo_pin, g_sharp, 500);
        enter_check(button_4, 0);
        delay(500);
    }
}

// Check and input note to note_entered
void enter_check(int button_num, int check_array) {
    if (check_array == 3) {
        note_entered[check_array] = button_num;
        code_check();
    }
    else if (note_entered[check_array] == 0) {
        note_entered[check_array] = button_num;
    }
    else {
        enter_check(button_num, check_array + 1);
    }
}

// Check whether note_code and note_entered match
void code_check() {
    bool code_match = true;
    
    for (int i = 0; i < 4; i += 1) {
        if (note_code[i] != note_entered[i]) {
            code_match = false;
        }
    }
    
    if (code_match) {
        digitalWrite(red_pin, LOW);
        operate_safe();
        
        for (int i = 0; i < 4; i += 1) {
            note_entered[i] = 0;
        }
        
        delay(5000);
        digitalWrite(red_pin, HIGH);
        lcd.clear();
        lcd.print("Standby");
    }
    else {
        digitalWrite(green_pin, LOW);
        lcd.clear();
        lcd.print("Try again..");
        
        for (int i = 0; i < 4; i += 1) {
            note_entered[i] = 0;
        }
        
        delay(5000);
        digitalWrite(green_pin, HIGH);
        lcd.clear();
        lcd.print("Standby");
    }
}

// Open and close safe
void operate_safe() {
    lcd.clear();
    lcd.print("Opening safe..");
    
    for (position = 0; position <= 180; position += 1) {
        servo_0.write(position);
        delay(20);
    }
    
    delay(60000);
    
    lcd.clear();
    lcd.print("Closing safe..");
    
    for (position = 180; position >= 0; position -= 1) {
        servo_0.write(position);
        delay(20);
    }
}

