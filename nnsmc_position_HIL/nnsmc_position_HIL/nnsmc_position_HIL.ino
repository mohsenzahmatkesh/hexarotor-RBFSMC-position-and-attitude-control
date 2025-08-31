double g = 9.81;
double mass = 1; 
double L = 0.2; 
double Ix = 0.008; 
double Iy = 0.008;
double Iz = 0.017;
double c = 0.088;
float W[14][10];
float RBF_std[10][1];

void nn_weight(float W[14][10]){
  for (int i = 0; i < 14; i++) {
    for (int j = 0; j < 10; j++){
      W[i][j] = -1.0f + 2.0f * (float)rand() / (float)RAND_MAX;
    }
  }
}

void nn_RBF_std(float RBF_std[10][1]){
  for (int i = 0; i < 10; i++)
  RBF_std[i][0] = 1.2f + 3.0f*(float)rand() / (float)RAND_MAX;
}



void send_to_simulink() {
  uint8_t buf[3 + 10*4 + 14*10*4];
  buf[0] = 0xAA;
  buf[1] = 0xBB;
  buf[2] = 0xCC;

  memcpy(&buf[3], &RBF_std[0][0], 40);
  memcpy(&buf[43], &W[0][0], 560);

  Serial.write(buf, sizeof(buf));
}

void setup() {
  Serial.begin(115200);
  
  
}

void loop() {
  nn_weight(W);
  nn_RBF_std(RBF_std);
  send_to_simulink();
}

// void readFromMatlab() {
//   // Read bytes directly into the struct. This is efficient because the
//   // struct is "packed" and matches the data order from Simulink's Mux block.
//   byte* buffer = (byte*)&incomingData;
//   Serial.readBytes(buffer, STATE_PACKET_SIZE);
// }

// /**
//  * @brief Sends the entire 40-byte ControlData struct over the serial port.
//  * @param data The ControlData struct to be sent.
//  */
// void writeToMatlab(const StateAndParamsData& data) {
//   const byte* buffer = (const byte*)&data;
//   Serial.write(buffer, sizeof(StateAndParamsData));
//   Serial.write(13);
//   Serial.write(10);
// }
// }



