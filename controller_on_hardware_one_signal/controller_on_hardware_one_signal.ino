/**
 * This sketch demonstrates a simplified and efficient two-way communication of
 * float values between an Arduino/ESP and a host computer (like MATLAB/Simulink).
 * It listens for a 4-byte float, adds 5.0 to it, and sends it back.
 */

// Union to easily convert between a float and its constituent bytes.
union FloatConverter {
  byte b[4];
  float fval;
} converter;

// A float is 4 bytes long.
const int BUFFER_SIZE = 4;

void setup() {
  // Initialize serial communication at 115200 baud rate.
  Serial.begin(115200, SERIAL_8N1);
}

void loop() {
  // Check if at least 4 bytes (the size of one float) are available to read.
  if (Serial.available() >= BUFFER_SIZE) {
    // Read the incoming float from Simulink.
    float receivedValue = readFromMatlab();
    
    // For testing, we send the exact same value back.
    float valueToSend = receivedValue;
    
    // Send the new value back to Simulink.
    writeToMatlab(valueToSend);
    
    // **NEW:** Clear the serial input buffer. This removes any old data
    // that might have arrived while we were busy, preventing a backlog.
    while(Serial.available() > 0) {
      Serial.read();
    }
  }
}

/**
 * @brief Reads 4 bytes from the serial port and converts them to a float.
 * This simplified version relies on Serial.readBytes().
 * @return The converted float value.
 */
float readFromMatlab() {
  // Read 4 bytes directly into our converter's byte array.
  // This works reliably now that the Simulink configuration is correct.
  Serial.readBytes(converter.b, BUFFER_SIZE);
  
  // The union now correctly represents the float value.
  return converter.fval;
}


/**
 * @brief Sends a 4-byte float over the serial port in Little Endian format,
 * followed by a carriage return and line feed terminator.
 * @param number The float value to be sent.
 */
void writeToMatlab(float number) {
  // Get a byte pointer to the float variable. This sends the raw bytes
  // in the Arduino's native Little Endian format.
  byte *b = (byte *) &number;
  
  // Write the 4 bytes that represent the float.
  Serial.write(b, 4);
  
  // Write the terminator characters.
  Serial.write(13);     // Carriage return '\r'
  Serial.write(10);     // Line feed '\n'
}

