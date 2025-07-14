#pragma once
#include <Arduino.h>

// Struct for the state variables coming from Simulink
struct __attribute__((packed)) StateAndParamsData {
  float t;
  float x, y, z;
  float xd, yd, zd;
  float xdd, ydd, zdd;
  float phi, theta, psi;
  float phid, thetad, psid;
};

// NEW: Struct for the controller outputs being sent back to Simulink
struct __attribute__((packed)) ControllerOutputData {
  float u1, u2, u3, u4;
  float Delta_hat[4]; // Array to hold the 4 Delta_hat values
};

// The size of the packet we expect from Simulink
const int STATE_PACKET_SIZE = sizeof(StateAndParamsData);

// === Global Constants ===
const float g = 9.81;
const float mass = 1.0;
const float L = 0.2;
const float Ix = 0.008;
const float Iy = 0.008;
const float Iz = 0.017;
const float c = 0.088;

// === Extern Declarations ===
// These tell the compiler that these variables exist and will be defined elsewhere.
extern StateAndParamsData incomingData;
extern float rbf_means[14][40]; // From your previous file
extern float rbf_stds[40];      // From your previous file
extern float W[40][40];         // NEW: For the controller weights
extern float u_prev[4];         // NEW: To store previous control inputs


