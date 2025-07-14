
// Use an enum to define our packet types. This makes the code readable.
enum PacketType : uint8_t {
  HEADER_INVALID          = 0x00,
  PACKET_STATE_AND_PARAMS = 0x01
};

// A struct for the state variables coming from Simulink.
// The order here MUST EXACTLY MATCH the order in your Simulink Mux block.
struct __attribute__((packed)) StateAndParamsData {
  float t;
  // float x, y, z;
  // float xd, yd, zd;
  // float xdd, ydd, zdd; // Assuming you have zdd
  // float phi, theta, psi;
  // float phid, thetad, psid;
};

// === Global Constants ===
const float g = 9.81;
const float mass = 1.0;
const float L = 0.2;
const float Ix = 0.008;
const float Iy = 0.008;
const float Iz = 0.017;
const float c = 0.088;

// The size of the packet we expect from Simulink
const int STATE_PACKET_SIZE = sizeof(StateAndParamsData);


// === Global Variables ===
float matrix1[14][40]; // For values between -1.0 and 1.0
float vector1[40];     // For values between 1.2 and 4.2
StateAndParamsData currentStateData; // Holds incoming data from Simulink


// === Function Implementations ===

// /**
//  * @brief Generates a random float within a specified range.
//  */
// float randomFloat(float min_val, float max_val) {
//   return min_val + (float)random(0, 1000001) / 1000000.0 * (max_val - min_val);
// }

// /**
//  * @brief Fills the global matrices with random values.
//  */
// void initializeMatrices() {
//   for (int i = 0; i < 14; i++) {
//     for (int j = 0; j < 40; j++) {
//       matrix1[i][j] = randomFloat(-1.0, 1.0);
//     }
//   }
//   for (int i = 0; i < 40; i++) {
//     vector1[i] = randomFloat(1.2, 4.2);
//   }
// }

/**
 * @brief Reads the state variables packet from serial.
 */
void receiveStatePacket() {
  // Wait for the complete packet to arrive.
  while (Serial.available() < STATE_PACKET_SIZE) {
    // This loop can cause delays if data arrives slowly.
  }
  
  byte* buffer = (byte*)&currentStateData;
  Serial.readBytes(buffer, STATE_PACKET_SIZE);
}

/**
 * @brief Sends the entire StateAndParamsData struct back to Simulink.
 */
void sendStatePacket(const StateAndParamsData& data) {
  const byte* buffer = (const byte*)&data;
  Serial.write(buffer, STATE_PACKET_SIZE);
  Serial.write(13); // Carriage return '\r'
  Serial.write(10); // Line feed '\n'
}


// === Main Program ===

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  // Use a noisy analog pin to seed the random number generator
  // randomSeed(analogRead(A0)); 
  // initializeMatrices(); // Initialize the matrices with random values
}

void loop() {
  // Check if there's at least one byte to read (for the header).
  if (Serial.available() > 0) {
    PacketType header = (PacketType)Serial.read();

    if (header == PACKET_STATE_AND_PARAMS) {
      // Receive the data packet from Simulink
      receiveStatePacket();
      
      // For verification, immediately send the same packet back.
      // This is the primary cause of the slowdown in Simulink.
      sendStatePacket(currentStateData);
    }
  }
}
