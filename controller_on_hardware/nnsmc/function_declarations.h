#pragma once
#include "config.h" // Include config to know about ControlData type

// === Function Declarations ===
float randomFloat(float min_val, float max_val);
void initializeRBF(); // Renamed from initializeMatrices
void initializeController();
void readFromMatlab();
void writeToMatlab(const ControllerOutputData& data);
void getDesiredTrajectory(float t, float &xdes, float &ydes, float &zdes, float &phides, float &thetades, float &psides);
// Controller functions
// Controller functions
void calculateNeuralNetwork(float Delta_hat_out[4], const StateAndParamsData& state, const float u_prev_in[4]);
void calculateAltitudeControl(float& u1_out, float delta_hat_4, const StateAndParamsData& state);
void calculatePsiControl(float& u4_out, float delta_hat_3, const StateAndParamsData& state);
void calculateLongitudeControl(float& u3_out, float delta_hat_2, const StateAndParamsData& state);
void calculateLateralControl(float& u2_out, float delta_hat_1, const StateAndParamsData& state);
void updateControllerWeights(const StateAndParamsData& state, const float u_prev_in[4]);
