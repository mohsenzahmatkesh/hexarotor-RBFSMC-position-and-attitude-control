#include <Arduino.h>
#include <math.h>
#include "config.h"
#include "function_declarations.h"

// === Global Variable Definition ===
StateAndParamsData incomingData;

float rbf_means[14][40];
float rbf_stds[40];
float W[40][40];
float u_prev[4]; // u_prev[0]=u1, [1]=u2, [2]=u3, [3]=u4

// === Helper Functions ===
float sgn(float val) {
  if (val > 1.0f) return 1.0f;
  if (val < -1.0f) return -1.0f;
  return val;
}

float norm(const float vec[], int size) {
    float sum_sq = 0.0;
    for (int i = 0; i < size; i++) {
        sum_sq += vec[i] * vec[i];
    }
    return sqrt(sum_sq);
}

float randomFloat(float min_val, float max_val) {
  return min_val + (float)random(0, 1000001) / 1000000.0 * (max_val - min_val);
}

// === Initialization Functions ===
void initializeRBF() {
  for (int i = 0; i < 14; i++) {
    for (int j = 0; j < 40; j++) {
      rbf_means[i][j] = randomFloat(-1.0, 1.0);
    }
  }
  for (int i = 0; i < 40; i++) {
    rbf_stds[i] = randomFloat(1.2, 4.2);
  }
}

void initializeController() {
    for(int i=0; i<4; i++) u_prev[i] = 0.0f;
    for(int i=0; i<40; i++) {
        for(int j=0; j<40; j++) {
            W[i][j] = 0.0f;
        }
    }
}
void getDesiredTrajectory(float t, float &xdes, float &ydes, float &zdes, float &phides, float &thetades, float &psides) {
    phides   = 0.0f;
    thetades = 0.0f;
    psides   = 0.0f;
    
    zdes = 2.0f;

    if (t < 15.0f) {
        xdes = 0.0f;
        ydes = t / 15.0f;              
    } else if (t < 30.0f) {
        xdes = (t - 15.0f) / 15.0f;    
        ydes = 1.0f;
    } else if (t < 45.0f) {
        xdes = 1.0f;
        ydes = 1.0f - (t - 30.0f) / 15.0f; 
    } else if (t <= 60.0f) {
        xdes = 1.0f - (t - 45.0f) / 15.0f; 
        ydes = 0.0f;
    } else {
        xdes = 0.0f;
        ydes = 0.0f;
        zdes = 0.0f;
    }
}

/**
 * @brief Reads a 40-byte data packet from serial into the ControlData struct.
 */
void readFromMatlab() {
  // Read bytes directly into the struct. This is efficient because the
  // struct is "packed" and matches the data order from Simulink's Mux block.
  byte* buffer = (byte*)&incomingData;
  Serial.readBytes(buffer, STATE_PACKET_SIZE);
}

/**
 * @brief Sends the entire 40-byte ControlData struct over the serial port.
 * @param data The ControlData struct to be sent.
 */
void writeToMatlab(const ControllerOutputData& data) {
  const byte* buffer = (const byte*)&data;
  Serial.write(buffer, sizeof(ControllerOutputData));
  Serial.write(13);
  Serial.write(10);
}

// === Controller Logic ===

void calculateNeuralNetwork(float Delta_hat_out[4]) {
    float zdes = 2.0f; // Assuming constant desired altitude for now
    float e_dot_z = currentStateData.zd - 0; // zddes = 0

    float nn_input[14];
    nn_input[0] = currentStateData.phi / 10.0f;
    nn_input[1] = currentStateData.theta / 10.0f;
    nn_input[2] = currentStateData.psi / 10.0f;
    nn_input[3] = currentStateData.z / 10.0f;
    nn_input[4] = zdes / 15.0f;
    nn_input[5] = currentStateData.phid / 10.0f;
    nn_input[6] = currentStateData.thetad / 10.0f;
    nn_input[7] = currentStateData.psid / 10.0f;
    nn_input[8] = currentStateData.zd / 10.0f;
    nn_input[9] = e_dot_z * 2.0f;
    nn_input[10] = u_prev[1] / 15.0f; // u2_prev
    nn_input[11] = u_prev[2] / 15.0f; // u3_prev
    nn_input[12] = u_prev[3] / 15.0f; // u4_prev
    nn_input[13] = u_prev[0] / 15.0f; // u1_prev

    float mu[40];
    for (int i = 0; i < 40; i++) {
        float diff[14];
        for(int j=0; j<14; j++){
            diff[j] = nn_input[j] - rbf_means[j][i];
        }
        mu[i] = exp(-norm(diff, 14) * norm(diff, 14) / (2.0f * rbf_stds[i] * rbf_stds[i]));
    }

    // Delta_hat = transpose(W) * mu;
    for (int i = 0; i < 4; i++) {
        Delta_hat_out[i] = 0.0f;
        for (int j = 0; j < 40; j++) {
            Delta_hat_out[i] += W[j][i] * mu[j]; // W is transposed here
        }
    }
}

void calculateAltitudeControl(float& u1_out, float delta_hat_4) {
    float zdes = 2.0f;
    const float cz = 1.0f, eps1 = 0.8f, eta1 = 50.0f;
    float s1 = cz * (currentStateData.z - zdes) + (currentStateData.zd - 0); // zddes=0
    u1_out = (mass / (cos(currentStateData.phi) * cos(currentStateData.theta))) *
             (cz * (currentStateData.zd - 0) + 0 + g + eps1 * sgn(s1) + eta1 * s1 + delta_hat_4);
}

void calculatePsiControl(float& u4_out, float delta_hat_3) {
    const float cpsi = 1.5f, eps2 = 0.8f, eta2 = 50.0f;
    float s2 = cpsi * (0 - currentStateData.psi) + (0 - currentStateData.psid); // psides=0, psiddes=0
    u4_out = (Iz / c) * (cpsi * (0 - currentStateData.psid) + 0 + eps2 * sgn(s2) + eta2 * s2 + 2.0f * delta_hat_3);
}

void calculateLongitudeControl(float& u3_out, float delta_hat_2) {
    const float eps3 = 0.5f, eta3 = 50.0f, c3 = 1.0f;
    float c1 = (11.0f * mass / (cos(currentStateData.phi) * cos(0) * cos(0))) * c3; // psi=0, thetades=0
    float c2 = (6.0f * mass / (cos(currentStateData.phi) * cos(0) * cos(0))) * c3;
    float c4 = 6.0f * c3;
    float s3 = c1 * (0 - currentStateData.xd) + c2 * (0 - currentStateData.x) + c3 * (0 - currentStateData.thetad) + c4 * (0 - currentStateData.theta);
    u3_out = (Iy / L) * ((c1 / c3) * (0 - currentStateData.xdd) + (c2 / c3) * (0 - currentStateData.xd) + 0 +
             (c4 / c3) * (0 - currentStateData.thetad) + (1.0f / c3) * (eps3 * sgn(s3) + eta3 * s3) + delta_hat_2);
}

void calculateLateralControl(float& u2_out, float delta_hat_1) {
    const float eps4 = 0.5f, eta4 = 50.0f, c7 = 1.0f;
    float c5 = (-11.0f * mass / ((sin(0) * sin(currentStateData.theta) * sin(0)) + (cos(0) * cos(0)))) * c7; //phides=0, psi=0
    float c6 = (-6.0f * mass / ((sin(0) * sin(currentStateData.theta) * sin(0)) + (cos(0) * cos(0)))) * c7;
    float c8 = 6.0f * c7;
    float s4 = c5 * (0 - currentStateData.yd) + c6 * (0 - currentStateData.y) + c7 * (0 - currentStateData.phid) + c8 * (0 - currentStateData.phi);
    u2_out = (Ix / L) * ((c5 / c7) * (0 - currentStateData.ydd) + (c6 / c7) * (0 - currentStateData.yd) + 0 +
             (c8 / c7) * (0 - currentStateData.phid) + (1.0f / c7) * (eps4 * sgn(s4) + eta4 * s4) + delta_hat_1);
}

// NOTE: This function calculates W_dot but the result is not yet sent to Simulink.
void calculateW_dot(float W_dot_out[40][40], const float Delta_hat[4]) {
    // This part is more complex and will be implemented in the next step.
    // For now, it does nothing.
}

