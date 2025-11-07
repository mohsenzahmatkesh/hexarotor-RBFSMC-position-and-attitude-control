float g = 9.81;
float mass = 1; 
float L = 0.2; 
float Ix = 0.008; 
float Iy = 0.008;
float Iz = 0.017;
float c = 0.088;

// Global states received from Simulink
float x, y, z;
float xd, yd, zd;
float xdd, ydd;
float phi, theta, psi;
float phid, thetad, psid;
float xdes, ydes, zdes;
float phides, thetades, psides;


float phiddes = 0, phidddes = 0;
float thetaddes = 0, thetadddes = 0;
float psiddes = 0, psidddes = 0;
float xddes = 0, xdddes = 0;
float yddes = 0, ydddes = 0;
float zddes = 0, zdddes = 0;


// SMC parameters
float cz = 1;
float eps1 = 0.8;
float eta1 = 50;

float cpsi = 1.5; 
float eps2 = 0.8; 
float eta2 = 50; 

float ctheta = 1.5;
float eps3 = 0.5;
float eta3 = 50; 
float c3 = 1;
float c4 = 6;

float chphi = 1.5;
float eps4 = 0.5;
float eta4 = 50; 
float c7 = 1;
float c8 = 6;


// NN parameters
float Gamma = 100; 
float RBF_means[14][10];
float RBF_std[10][1];
float mu[10];
float Delta_hat[4];
float s[4] = {0.0f, 0.0f, 0.0f, 0.0f};
float W_dot[10][4] = {0.0f};
float W[10][4];   //10x4 matrix
float W_sim[10][4];   //10x4 matrix
float W_T[4][10];


float u1, u2, u3, u4;
float s1, s2, s3, s4;
float s11, s22, s33, s44;
float u1_prev, u2_prev, u3_prev, u4_prev;


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
        W_sim[i][j] = val[20 + j * 10 + i];
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
      Delta_hat[j] += W_sim[i][j] * mu[i];
    }
  }

  s1 = cz*(z - zdes) + (zd - zddes);
  s11 = tanhf(s1);
  u1 = (mass/(cos(phi)*cos(theta)))*(cz*(zd-(zddes)) + zdddes + g + eps1*s11 + eta1*s1 + Delta_hat[3]);

  s2 = cpsi*(psides - psi) + (psiddes - psid);
  s22 = tanhf(s2);
  u4 = (Iz/c)*(cpsi*(psiddes-psid) + psidddes + eps2*s22 + eta2*s2 + 0.42*Delta_hat[2]);


  float c1 = (11*mass/(cos(phi)*cos(psi)*cos(thetades))) * c3;
  float c2 = (6*mass/(cos(phi)*cos(psi)*cos(thetades))) * c3;
  s3 = c1*(xddes - xd) + c2*(xdes - x) + c3*(thetaddes - thetad) + c4*(thetades - theta);
  s33 = tanhf(s3);
  u3 = (Iy/L)*((c1/c3)*(xdddes-xdd) + (c2/c3)*(xddes-xd) + thetadddes + (c4/c3)*(thetaddes-thetad)  + (1/c3)*(eps3*s33 + eta3*s3) + Delta_hat[1]);


  float c5 = (-11*mass/((sin(phides)*sin(theta)*sin(psi))+(cos(phides)*cos(psi)))) * c7;
  float c6 = (-6*mass/((sin(phides)*sin(theta)*sin(psi))+(cos(phides)*cos(psi)))) * c7;
  s4 = c5*(yddes - yd) + c6*(ydes - y) + c7*(phiddes - phid) + c8*(phides - phi);
  s44 = tanhf(s4);
  u2 = (Ix/L)*((c5/c7)*(ydddes-ydd) + (c6/c7)*(yddes-yd) + phidddes + (c8/c7)*(phiddes-phid) +  (1/c7)*(eps4*s44 + eta4*s4) + Delta_hat[0]);

  s[0] = s4;
  s[1] = s3;
  s[2] = s2;
  s[3] = s1;

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 4; j++) {
      W_dot[i][j] = Gamma * mu[i] * s[j];
    }
  }

  float dt = 0.005f;  // 200 Hz control loop

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 4; j++) {
      W[i][j] += W_dot[i][j] * dt;
    }
  }
  // float W_T[4][10];
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 4; j++) {
      W_T[j][i] = W[i][j];
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
  memcpy(&buf[603], &u1, 4);
  memcpy(&buf[607], &u2, 4);
  memcpy(&buf[611], &u3, 4);
  memcpy(&buf[615], &u4, 4);

  // memcpy(&buf[603], &mu[0], 40);

  Serial.write(buf, sizeof(buf));
}

void setup() {
  Serial.begin(921600);
  NN_RBF_means(RBF_means);
  NN_RBF_std(RBF_std);
  
  
}

void loop() {
  receive_from_simulink();
  nn_smc();
  send_to_simulink();
}




