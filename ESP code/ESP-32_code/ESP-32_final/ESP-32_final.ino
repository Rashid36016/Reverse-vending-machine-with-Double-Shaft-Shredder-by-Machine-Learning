#include <ESP32Servo.h>

// =============================
// ===== USER CHANGE ZONE =====
// =============================

#define SERVO1_PIN 4
#define SERVO2_PIN 5

#define READY_LED 12        //Ready for taking bottle
#define STOP_LED 13         //Not ready for taking bottle
#define PROCESSING_LED 18   //Processing 
#define SERVO_OPEN_LED 21   //Get is opening
#define SERVO_CLOSE_LED 22  //Get is closing
#define PRINT_LED 23        //Voucher printing

// Printer UART pins
#define PRINTER_RX 16    // ESP32 receive from printer (optional)
#define PRINTER_TX 17    // ESP32 transmit to printer RX

// Servo speed control
#define SERVO_STEP_DELAY 1
#define SERVO_STEP_SIZE 5

// Servo angles
#define SERVO1_OPEN_ANGLE 180
#define SERVO1_CLOSE_ANGLE 0

#define SERVO2_OPEN_ANGLE 0
#define SERVO2_CLOSE_ANGLE 180

// =============================

Servo servo1;
Servo servo2;

HardwareSerial printer(2);   // UART2

int servo1Current = SERVO1_CLOSE_ANGLE;
int servo2Current = SERVO2_CLOSE_ANGLE;

void setup() {

  Serial.begin(115200);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  pinMode(PROCESSING_LED, OUTPUT);
  pinMode(SERVO_OPEN_LED, OUTPUT);
  pinMode(SERVO_CLOSE_LED, OUTPUT);
  pinMode(PRINT_LED, OUTPUT);
  pinMode(READY_LED, OUTPUT);
  pinMode(STOP_LED, OUTPUT);

  // Start printer UART
  printer.begin(9600, SERIAL_8N1, PRINTER_RX, PRINTER_TX);

  readyState();
}


//Receiving data loop
void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');

    if (cmd == "TRIGGER") {
      processCycle();
    }
  }
}


// =============================

void moveServoSmooth(Servo &servo, int &currentPos, int targetPos) {

  while (currentPos != targetPos) {

    if (currentPos < targetPos) {
      currentPos += SERVO_STEP_SIZE;
      if (currentPos > targetPos) currentPos = targetPos;
    }
    else {
      currentPos -= SERVO_STEP_SIZE;
      if (currentPos < targetPos) currentPos = targetPos;
    }

    servo.write(currentPos);
//    delay(SERVO_STEP_DELAY);   // শুধু servo step delay থাকবে
  }
}

// =============================

void printVoucher() {

  digitalWrite(PRINT_LED, HIGH);

printer.write(27); 
printer.write(64);

  // -------- RVM System (Center, Big) --------
  printer.write(27); printer.write(97); printer.write(1);   // Center
  printer.write(29); printer.write(33); printer.write(17);  // Big size
  printer.println("RVM System");

  // -------- MIST Cafeteria (Center, Big) --------
  printer.println("MIST Cafeteria");

  // Reset size to normal
  printer.write(29); printer.write(33); printer.write(0);

  // Line
  printer.println("------------------------------------");

  // -------- Bottle Accepted (Left, Small) --------
  printer.write(27); printer.write(97); printer.write(0);   // Left
  printer.println("Bottle Accepted");

  // -------- Reward Label --------
  printer.println("Reward:");

  // -------- 10 (Very Big) --------
  printer.write(27); printer.write(97); printer.write(1);   // Center
  printer.write(29); printer.write(33); printer.write(34);  // Very Big
  printer.print("2");

  // Back to small size
  printer.write(29); printer.write(33); printer.write(0);
  printer.println(" Taka");

  // -------- Thank You (Center, Medium) --------
  printer.write(27); printer.write(97); printer.write(1);   // Center
  printer.write(29); printer.write(33); printer.write(16);  // Medium size
  printer.println("Thank You!");

  // Reset size
  printer.write(29); printer.write(33); printer.write(0);

  // Line
  printer.println("------------------------------------");

  // -------- IPE PD (Right, Smallest) --------
  printer.write(27); printer.write(97); printer.write(2);   // Right
  printer.println("IPE PD");

  printer.println();
  printer.println();

  delay(1000);

  digitalWrite(PRINT_LED, LOW);
}

// =============================

void processCycle() {

  digitalWrite(READY_LED, LOW);
  digitalWrite(STOP_LED, HIGH);
  digitalWrite(PROCESSING_LED, HIGH);


  delay(250);


// Servo open
  digitalWrite(SERVO_OPEN_LED, HIGH);
  moveServoSmooth(servo1, servo1Current, SERVO1_OPEN_ANGLE);
  moveServoSmooth(servo2, servo2Current, SERVO2_OPEN_ANGLE);
  digitalWrite(SERVO_OPEN_LED, LOW);


  delay(250);


// Print Voucher
  printVoucher();


  delay(500);


// Servo close
  digitalWrite(SERVO_CLOSE_LED, HIGH);
  moveServoSmooth(servo1, servo1Current, SERVO1_CLOSE_ANGLE);
  moveServoSmooth(servo2, servo2Current, SERVO2_CLOSE_ANGLE);
  digitalWrite(SERVO_CLOSE_LED, LOW);


  delay(2000);
  readyState();
}

// =============================

void readyState() {

// Initial states 
digitalWrite(READY_LED, HIGH); 
digitalWrite(STOP_LED, LOW); 
digitalWrite(PROCESSING_LED, LOW); 
digitalWrite(SERVO_OPEN_LED, LOW); 
digitalWrite(SERVO_CLOSE_LED, LOW); 
digitalWrite(PRINT_LED, LOW);

}


