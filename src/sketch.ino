#define DEBUG 1

const int R_FORWARD = 3;
const int R_REVERSE = 5;
const int L_FORWARD = 6;
const int L_REVERSE = 9;

// Serial buffer
const int COMMAND_LENGTH = 16;
char serial_buffer[COMMAND_LENGTH + 1];
char serial_buffer_size = 0;

void setup() {
    Serial.begin(115200);
}

void loop() {
    read_commands();
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

                drive(direction, speed);
            }
            else if (strcmp(token, "stop") == 0) {
                stop();
            }
            else {
                log("Bad command");
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

void log(String message) {
    if (DEBUG) {
        Serial.print(message);
    }
}