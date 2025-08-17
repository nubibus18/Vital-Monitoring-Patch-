#include <Wire.h>

#define SDA_PIN 21        // Change if needed
#define SCL_PIN 22        // Change if needed
#define DEVICE_ADDR 0x64  // Replace with your I2C device address

void setup() {
    Serial.begin(115200);
    Wire.begin(SDA_PIN, SCL_PIN);
    // Set I2C speed to 400 kHz
    Serial.println("Enter R/W followed by register address (HEX), and value (for W):");
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n'); // Read user input
        input.trim();  // Remove whitespace
        
        if (input.length() < 3) {
            Serial.println("Invalid input. Use: R <reg> or W <reg> <value>");
            return;
        }

        char operation = toupper(input.charAt(0)); // Read operation type (R/W)
        input = input.substring(2); // Remove operation from string

        int spaceIndex = input.indexOf(' '); 
        if (spaceIndex == -1) spaceIndex = input.length(); 
        
        String regStr = input.substring(0, spaceIndex);
        uint8_t reg = strtol(regStr.c_str(), NULL, 16); // Convert register address
        
        if (operation == 'R') {
            // READ Operation
            Serial.printf("Reading 2 bytes from register 0x%02X...\n", reg);
            uint16_t value = i2cRead2Bytes(reg);
            Serial.printf("Value at register 0x%02X: 0x%04X (%d)\n", reg, value, value);
        } 
        else if (operation == 'W' && spaceIndex != input.length()) {
            // WRITE Operation
            String valueStr = input.substring(spaceIndex + 1);
            uint16_t value = strtol(valueStr.c_str(), NULL, 16); // Convert value to HEX

            Serial.printf("Writing 0x%04X to register 0x%02X...\n", value, reg);
            i2cWrite2Bytes(reg, value);
            Serial.println("Write successful!");
        } 
        else {
            Serial.println("Invalid input. Use: R <reg> or W <reg> <value>");
        }

        Serial.println("Enter next command:");
    }
}

// Function to read 2 bytes from the given register
uint16_t i2cRead2Bytes(uint8_t reg) {
    Wire.beginTransmission(DEVICE_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false); // Send repeated start condition
    Wire.requestFrom(DEVICE_ADDR, (uint8_t)2); // Request 2 bytes

    if (Wire.available() >= 2) {
        uint8_t highByte = Wire.read(); // Read MSB
        uint8_t lowByte = Wire.read();  // Read LSB
        return (highByte << 8) | lowByte; // Combine MSB and LSB
    } else {
        Serial.println("Error: No data received!");
        return 0xFFFF; // Return error value
    }
}

// Function to write 2 bytes to the given register
void i2cWrite2Bytes(uint8_t reg, uint16_t value) {
    Wire.beginTransmission(DEVICE_ADDR);
    Wire.write(reg);
    Wire.write(value >> 8);  // Write high byte
    Wire.write(value & 0xFF);  // Write low byte
    Wire.endTransmission();
}
