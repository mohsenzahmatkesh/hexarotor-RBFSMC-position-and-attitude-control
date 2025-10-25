double g = 9.81;
double mass = 1; 
double L = 0.2; 
double Ix = 0.008; 
double Iy = 0.008;
double Iz = 0.017;
double c = 0.088;

// Global states received from Simulink
float x, y, z;
float xd, yd, zd;
float xdd, ydd;
float phi, theta, psi;
float phid, thetad, psid;
float xdes, ydes, zdes;
float phides, thetades, psides;
float u1_prev, u2_prev, u3_prev, u4_prev;
float W[10][4];   //10x4 matrix


double phiddes = 0, phidddes = 0;
double thetaddes = 0, thetadddes = 0;
double psiddes = 0, psidddes = 0;
double xddes = 0, xdddes = 0;
double yddes = 0, ydddes = 0;
double zddes = 0, zdddes = 0;

double Gamma = 100; 

float RBF_means[14][10];
float RBF_std[10][1];
float mu[10];
float Delta_hat[4];

void receive_from_simulink() {

  uint8_t buf[3 + 256];
  Serial.readBytes(buf, 259);
  if (buf[0] == 0xAA && buf[1] == 0xBB && buf[2] == 0xCC) {
    float* val = (float*)&buf[3];
    x        = val[0];
    y        = val[1];
    z        = val[2];
    xd       = val[3];
    yd       = val[4];
    zd       = val[5];
    xdd      = val[6];
    ydd      = val[7];
    phi      = val[8];
    theta    = val[9];
    psi      = val[10];
    phid     = val[11];
    thetad   = val[12];
    psid     = val[13];
    xdes     = val[14];
    ydes     = val[15];
    zdes     = val[16];
    phides   = val[17];
    thetades = val[18];
    psides   = val[19];

    for (int j = 0; j < 4; j++) {
      for (int i = 0; i < 10; i++) {
        W[i][j] = val[20 + j * 10 + i];
      }
    }


    // float* M = &val[20];
    u1_prev  = val[60];
    u2_prev  = val[61];
    u3_prev  = val[62];
    u4_prev  = val[63];
  }
}


void nn_smc(){
  float e_z = z - zdes;
  float e_dot_z = zd - zddes;

  float x_n[5] = {phi/10.0f, theta/10.0f, psi/10.0f, z/10.0f, zdes/15.0f};
  float x_dot_n[5] = {phid/10.0f, thetad/10.0f, psid/10.0f, zd/10.0f, e_dot_z*2.0f};
  float u_n[4] = {u2_prev/15.0f, u3_prev/15.0f, u4_prev/15.0f, u1_prev/15.0f};

  float nn_input[14];
  for (int i = 0; i < 5; i++) nn_input[i] = x_n[i];
  for (int i = 0; i < 5; i++) nn_input[i+5] = x_dot_n[i];
  for (int i = 0; i < 4; i++) nn_input[i+10] = u_n[i];


  for (int i = 0; i < 10; i++) {
      mu[i] = 0.0f;
      float sum = 0.0f;
      for (int j = 0; j < 14; ++j) {
          float d = nn_input[j] - RBF_means[j][i];
          sum += d * d;
      }
      float s = RBF_std[i][0];
      mu[i] = expf(-sum / (2.0f * s * s));
  }

  
  for (int j = 0; j < 4; j++) {
    Delta_hat[j] = 0.0f;
    for (int i = 0; i < 10; i++) {
      Delta_hat[j] += W[i][j] * mu[i];
    }
  }

}



void NN_RBF_means(float RBF_means[14][10]){
  for (int i = 0; i < 14; i++) {
    for (int j = 0; j < 10; j++){
      RBF_means[i][j] = -1.0f + 2.0f * (float)rand() / (float)RAND_MAX;
    }
  }
}

void NN_RBF_std(float RBF_std[10][1]){
  for (int i = 0; i < 10; i++)
  RBF_std[i][0] = 1.2f + 3.0f*(float)rand() / (float)RAND_MAX;
}



void send_to_simulink() {
  uint8_t buf[3 + 10*4 + 14*10*4 + 4*4];
  buf[0] = 0xAA;
  buf[1] = 0xBB;
  buf[2] = 0xCC;

  memcpy(&buf[3], &RBF_std[0][0], 40);       // 10 floats
  memcpy(&buf[43], &RBF_means[0][0], 560);   // 140 floats
  memcpy(&buf[603], &Delta_hat[0], 16);      // 4 floats
  // memcpy(&buf[603], &mu[0], 40);

  Serial.write(buf, sizeof(buf));
}

void setup() {
  Serial.begin(115200);
  NN_RBF_means(RBF_means);
  NN_RBF_std(RBF_std);
  
  
}

void loop() {
  receive_from_simulink();
  nn_smc();
  send_to_simulink();
}




