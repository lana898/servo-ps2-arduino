#include <PS2X_lib.h>
#include <Servo.h>

// =========================== PS2 Controller ===========================
PS2X ps2x;
int error = 0;
byte type = 0;
byte vibrate = 0;

// =========================== Servos ===========================
Servo servo_8;   // Stick esquerdo X
Servo servo_9;   // Stick esquerdo Y
Servo servo_7;   // Stick direito X
Servo servo_6;   // Stick direito Y

const int controleServo8 = 8;
const int controleServo9 = 9;
const int controleServo7 = 7;
const int controleServo6 = 6;

// Inicia todos os servos na posição 0
int pos8 = 0;
int pos9 = 0;
int pos7 = 0;
int pos6 = 0;

const int angMinimo = 0;
const int angMaximo = 180;

// Limite de atualização (velocidade de resposta)
int contador8 = 0;
int contador9 = 0;
int contador7 = 0;
int contador6 = 0;

const int limite8 = 1;
const int limite9 = 1;
const int limite7 = 1;
const int limite6 = 1;

// =========================== Setup ===========================
void setup() {
  Serial.begin(57600);

  // Configura PS2 (Clock, Command, Attention, Data, Pressures?, Rumble?)
  error = ps2x.config_gamepad(13, 11, 10, 12, true, true);

  if (error == 0) {
    Serial.println("Controller Found and configured!");
  } else {
    Serial.println("Controller not found or not accepting commands!");
  }

  type = ps2x.readType();
  if (type == 1) Serial.println("DualShock Controller Found");

  // Inicializa servos
  servo_8.attach(controleServo8);
  servo_9.attach(controleServo9);
  servo_7.attach(controleServo7);
  servo_6.attach(controleServo6);

  // Define todos servos na posição inicial 0
  servo_8.write(pos8);
  servo_9.write(pos9);
  servo_7.write(pos7);
  servo_6.write(pos6);
}

// =========================== Loop ===========================
void loop() {
  if (error == 1) return; // Se não encontrou o controle

  ps2x.read_gamepad(false, vibrate);  // Lê controle, sem vibrar

  // Lê valores do stick esquerdo
  int lx = ps2x.Analog(PSS_LX); // 0-255
  int ly = ps2x.Analog(PSS_LY); // 0-255

  // Lê valores do stick direito
  int rx = ps2x.Analog(PSS_RX); // 0-255
  int ry = ps2x.Analog(PSS_RY); // 0-255

  // Converte de 0-255 para -128 a 127
  int vrx = lx - 128;
  int vry = ly - 128;
  int vrx2 = rx - 128;
  int vry2 = ry - 128;

  // Atualiza posição do servo 8 (stick esquerdo X)
  if (++contador8 >= limite8) {
    contador8 = 0;
    pos8 += vrx / 10;
    if (pos8 < angMinimo) pos8 = angMinimo;
    if (pos8 > angMaximo) pos8 = angMaximo;
    servo_8.write(pos8);
  }

  // Atualiza posição do servo 9 (stick esquerdo Y)
  if (++contador9 >= limite9) {
    contador9 = 0;
    pos9 += vry / 10;
    if (pos9 < angMinimo) pos9 = angMinimo;
    if (pos9 > angMaximo) pos9 = angMaximo;
    servo_9.write(pos9);
  }

  // Atualiza posição do servo 7 (stick direito X)
  if (++contador7 >= limite7) {
    contador7 = 0;
    pos7 += vrx2 / 10;
    if (pos7 < angMinimo) pos7 = angMinimo;
    if (pos7 > angMaximo) pos7 = angMaximo;
    servo_7.write(pos7);
  }

  // Atualiza posição do servo 6 (stick direito Y)
  if (++contador6 >= limite6) {
    contador6 = 0;
    pos6 += vry2 / 10;
    if (pos6 < angMinimo) pos6 = angMinimo;
    if (pos6 > angMaximo) pos6 = angMaximo;
    servo_6.write(pos6);
  }

  // Debug no Serial: Stick Esquerdo (X,Y), Stick Direito (X,Y)
  Serial.print("Stick Values: ");
  Serial.print(pos8); Serial.print(", ");
  Serial.print(pos9); Serial.print(", ");
  Serial.print(pos7); Serial.print(", ");
  Serial.println(pos6);

  delay(20);
}

