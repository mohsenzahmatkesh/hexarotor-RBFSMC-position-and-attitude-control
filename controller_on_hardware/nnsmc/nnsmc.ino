#include "config.h"
#include "function_declarations.h"

void setup() {
  // Initialize serial communication at 115200 baud rate.
  Serial.begin(115200, SERIAL_8N1);
  randomSeed(analogRead(A0)); 
  initializeRBF();
  initializeController();
}

void loop() {
  // Check if the full packet of state data is available to read.
  if (Serial.available() >= STATE_PACKET_SIZE) {
    // Read the incoming state packet from Simulink.
    readFromMatlab(); // This fills the global 'incomingData' struct.
    
    ControllerOutputData outputs;
    float W_dot[40][40];

    // 3. Run the controller calculations in order
    calculateNeuralNetwork(outputs.Delta_hat);
    calculateAltitudeControl(outputs.u1, outputs.Delta_hat[3]);
    calculatePsiControl(outputs.u4, outputs.Delta_hat[2]);
    calculateLongitudeControl(outputs.u3, outputs.Delta_hat[1]);
    calculateLateralControl(outputs.u2, outputs.Delta_hat[0]);
    calculateW_dot(W_dot, outputs.Delta_hat); // Placeholder for now

    // Send the newly calculated trajectory data back to Simulink.
    writeToMatlab(outputs);

    u_prev[0] = outputs.u1;
    u_prev[1] = outputs.u2;
    u_prev[2] = outputs.u3;
    u_prev[3] = outputs.u4;
    
    // Clear the serial input buffer to prevent a backlog.
    while(Serial.available() > 0) {
      Serial.read();
    }
  }
}