#include <Servo.h>

#define DEBUG 1

const int L_ENABLE = 4;
const int L_FORWARD = 9;
const int L_REVERSE = 6;

const int R_ENABLE = 2;
const int R_FORWARD = 5;
const int R_REVERSE = 3;

const int pan_servo = 10;
const int tilt_servo = 11;

const int pan_center = 113;
const int tilt_center = 86;

const int pan_min = 10;
const int pan_max = 170;

const int tilt_min = 10;
const int tilt_max = 180;

int pan_pos = pan_center;
int tilt_pos = tilt_center;

Servo pan;
Servo tilt;

// Serial buffer
const int COMMAND_LENGTH = 16;
char serial_buffer[COMMAND_LENGTH + 1];
char serial_buffer_size = 0;

void setup() {
    Serial.begin(115200);

    // Enable both drive motors
    digitalWrite(L_ENABLE, HIGH);
    digitalWrite(R_ENABLE, HIGH);
    
    init_servos();
    stop_servos();
}

void loop() {
    read_commands();
}

void init_servos() {
    pan.attach(pan_servo);
    tilt.attach(tilt_servo);

    pan.write(pan_center);
    tilt.write(tilt_center);

}

void stop_servos() {
    delay(50);

    pan.detach();
    tilt.detach();

    delay(50);

    digitalWrite(pan_servo, LOW);
    digitalWrite(tilt_servo, LOW);
}

void read_commands() {
    if (Serial.available() > 0) {
        serial_buffer[serial_buffer_size] = Serial.read();

        if (serial_buffer[serial_buffer_size] == '\r' || serial_buffer[serial_buffer_size] == '\n' || serial_buffer_size == COMMAND_LENGTH) {
            // Process command
            char* token = strtok(serial_buffer, ",");

            if (strcmp(token, "d") == 0) {
                // Drive command: look for two more values
                char* direction = strtok(NULL, ",");
                int speed = atoi(strtok(NULL, ","));

                log("drive\n\r");
                drive(direction, speed);
            }
            else if (strcmp(token, "stop") == 0) {
                log("stop\n\r");
                stop();
            }
            else if (strcmp(token, "s") == 0) {
                char* servo = strtok(NULL, ",");
                int position = atoi(strtok(NULL, ","));
                
                log("servo\n\r");
                if (strcmp(servo, "pan")) {
                    set_pan(position);
                } else if (strcmp(servo, "tilt")) {
                    set_tilt(position);
                }
            }
            else if (strcmp(token, "stop_servos") == 0) {
                log("stop servos\n\r");
                stop_servos();
            }
            else if (strcmp(token, "start_servos") == 0) {
                log("start servos\n\r");
                init_servos();
            }
            else {
                log("Bad command\n\r");
            }

            // Clear buffer
            for (int i = 0; i < serial_buffer_size; i++) {
                serial_buffer[i] = (byte) 0;
            }
            serial_buffer_size = 0;
        } else {
            serial_buffer_size++;
        }
    }
}

void drive(char* direction, int speed) {
    if (strcmp(direction, "left") == 0) {
        left(speed);
    }
    else if (strcmp(direction, "right") == 0) {
        right(speed);
    }
    else if (strcmp(direction, "forward") == 0) {
        forward(speed);
    }
    else if (strcmp(direction, "reverse") == 0) {
        reverse(speed);
    }
}

void forward(int speed) {
    analogWrite(L_REVERSE, 0);
    analogWrite(R_REVERSE, 0);
    analogWrite(L_FORWARD, speed);
    analogWrite(R_FORWARD, speed);
}

void reverse(int speed) {
    analogWrite(L_FORWARD, 0);
    analogWrite(R_FORWARD, 0);
    analogWrite(L_REVERSE, speed);
    analogWrite(R_REVERSE, speed);
}

void left(int speed) {
    analogWrite(L_FORWARD, 0);
    analogWrite(R_FORWARD, speed);
    analogWrite(L_REVERSE, speed);
    analogWrite(R_REVERSE, 0);
}

void right(int speed) {
    analogWrite(L_FORWARD, speed);
    analogWrite(R_FORWARD, 0);
    analogWrite(L_REVERSE, 0);
    analogWrite(R_REVERSE, speed);
}

void stop() {
    analogWrite(L_FORWARD, 0);
    analogWrite(L_REVERSE, 0);
    analogWrite(R_FORWARD, 0);
    analogWrite(R_REVERSE, 0);
}

void set_pan(int val) {
    if (val >= 0 && val <= 180){
        pan_pos = val;
        pan.write(pan_pos);
    }
}

void set_tilt(int val) {
    if (val >= 0 && val <= 180){
        tilt_pos = val;
        tilt.write(tilt_pos);
    }
}

void log(String message) {
    if (DEBUG) {
        Serial.print(message);
    }
}