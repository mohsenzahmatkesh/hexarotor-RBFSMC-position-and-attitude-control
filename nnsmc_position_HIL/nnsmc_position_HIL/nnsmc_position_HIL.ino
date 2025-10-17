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

void receive_from_simulink() {

  uint8_t buf[3 + 264];
  Serial.readBytes(buf, 267);
  if (buf[0] == 0xAA && buf[1] == 0xBB && buf[2] == 0xCC) {
    float* val = (float*)&buf[3];
    float x        = val[0];
    float y        = val[1];
    float z        = val[2];
    float xd       = val[3];
    float yd       = val[4];
    float zd       = val[5];
    float xdd      = val[6];
    float ydd      = val[7];
    float phi      = val[8];
    float theta    = val[9];
    float psi      = val[10];
    float phid     = val[11];
    float thetad   = val[12];
    float psid     = val[13];
    float xdes     = val[14];
    float ydes     = val[15];
    float zdes     = val[16];
    float phides   = val[17];
    float thetades = val[18];
    float psides   = val[19];
    float* M = &val[20];   // 40 floats (10x4 matrix)
    float u1_prev  = val[60];
    float u2_prev  = val[61];
    float u3_prev  = val[62];
    float u4_prev  = val[63];
  }
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
  nn_weight(W);
  nn_RBF_std(RBF_std);
  
  
}

void loop() {
  receive_from_simulink();
  send_to_simulink();
}




