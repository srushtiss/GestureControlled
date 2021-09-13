



/* 
 *  THIS IS THE ACCELEROMETER PART 
 *  
 *  
 */

#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL


#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

int roll=0;
int yaw=0;
int pitch=0;



void setup_mpu6050(){
  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3V or Arduino
    // Pro Mini running at 3.3V, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);
    
    // verify connection
    mpu.testConnection();

    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
       
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
       
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
      
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
}




/* 
 *  THIS IS THE FLEX PART 
 *  
 *  
 */



int analogPinOliveA0= 14;
int analogPinWhiteA1= 15;
int analogPinRedA2= 16;
int analogPinBlackA3= 17;

int mcuOlive=4;
int mcuWhite=7;
int mcuRed=8;
int mcuBlack=12;


int rawRedA2= 0;
int rawWhiteA1= 0;
int rawBlackA3= 0;
int rawOliveA0= 0;

int Vin= 5;

float VoutOliveA0= 0;
float VoutWhiteA1= 0;
float VoutRedA2= 0;
float VoutBlackA3= 0;

float R1=49500;

float ROlive= 0;
float RWhite= 0;
float RRed= 0;
float RBlack= 0;

float bufferOlive= 0;
float bufferWhite= 0;
float bufferRed= 0;
float bufferBlack= 0;

int olive=0;
int white=0;
int red=0;
int black=0;


void setup() {
  Serial.begin(9600);
  pinMode(mcuOlive, OUTPUT);
  pinMode(mcuWhite, OUTPUT);
  pinMode(mcuRed, OUTPUT);
  pinMode(mcuBlack, OUTPUT);
  //setup_mpu6050();
}

void loop() {

   rawOliveA0= analogRead(analogPinOliveA0);
   rawWhiteA1= analogRead(analogPinWhiteA1);
   rawRedA2= analogRead(analogPinRedA2);
   rawBlackA3= analogRead(analogPinBlackA3);

   
   if(rawOliveA0 && rawWhiteA1 && rawRedA2 && rawBlackA3) 
    {
      bufferOlive= rawOliveA0 * Vin;
      bufferWhite= rawWhiteA1 * Vin;
      bufferRed= rawRedA2 * Vin;
      bufferBlack= rawBlackA3 * Vin;

      VoutOliveA0= (bufferOlive)/1024.0;
      VoutWhiteA1= (bufferWhite)/1024.0;
      VoutRedA2= (bufferRed)/1024.0;
      VoutBlackA3= (bufferBlack)/1024.0;
       if(VoutOliveA0<=1.45)
            olive=0;
          else
            olive=1;
    
          if(VoutWhiteA1<=1.35)
            white=0;
          else
            white=1;
    
          if(VoutRedA2<=1.93)
            red=0;
          else
            red=1;
    
          if(VoutBlackA3<=2.36)
            black=0;
          else
            black=1;

      bufferOlive= (Vin/VoutOliveA0) -1;
      bufferWhite= (Vin/VoutWhiteA1) -1;
      bufferRed= (Vin/VoutRedA2) -1;
      bufferBlack= (Vin/VoutBlackA3) -1;

      ROlive= R1 * bufferOlive;
      RWhite= R1 * bufferWhite;
      RRed= R1 * bufferRed;
      RBlack= R1 * bufferBlack;

     /* Serial.println("Olive: ");
      Serial.println(olive);
      Serial.println("  White: ");
      Serial.println(white);
      Serial.println("  Red: ");
      Serial.println(red);
      Serial.println("  Black: ");
      Serial.println(black);
    */
   if(olive==0 && white==0 && red==0 && black==0)
      {
        delay(2000);
        Serial.println("Device Control Mode");
        Serial.println("Conditions :");
        Serial.println("1. Light 1 ON: Index finger open, others closed\n2. Light 1 OFF: Index and middle finger open, others closed\n");
        Serial.println("3. Fan 1 ON: Little finger closed, others open\n4. Fan 1 OFF: Index finger closed, others open\n");
        Serial.println("5. Window 1 OPEN: Ring finger closed, others open\n6. Window 1 CLOSE: IMiddle finger closed, others open\n");
        Serial.println("7. Fan 1 speed HIGH: Move hand to the right\n8. Fan 1 speed LOW: Move hand downwards\n");
        for(;;)
        {
          delay(3000);
           
           rawOliveA0= analogRead(analogPinOliveA0);
           rawWhiteA1= analogRead(analogPinWhiteA1);
           rawRedA2= analogRead(analogPinRedA2);
           rawBlackA3= analogRead(analogPinBlackA3);
          
          bufferOlive= rawOliveA0 * Vin;
          bufferWhite= rawWhiteA1 * Vin;
          bufferRed= rawRedA2 * Vin;
          bufferBlack= rawBlackA3 * Vin;
    
          VoutOliveA0= (bufferOlive)/1024.0;
          VoutWhiteA1= (bufferWhite)/1024.0;
          VoutRedA2= (bufferRed)/1024.0;
          VoutBlackA3= (bufferBlack)/1024.0;
    
         if(VoutOliveA0<=1.45)
            olive=0;
          else
            olive=1;
    
          if(VoutWhiteA1<=1.35)
            white=0;
          else
            white=1;
    
          if(VoutRedA2<=1.93)
            red=0;
          else
            red=1;
    
          if(VoutBlackA3<=2.36)
            black=0;
          else
            black=1;



          Serial.print("Olive: ");
          Serial.print(olive);
          Serial.print("  White: ");
          Serial.print(white);
          Serial.print("  Red: ");
          Serial.print(red);
          Serial.print("  Black: ");
          Serial.println(black);
             
          if(olive==1 && white==0 && red==0 && black==0){
            Serial.println("Light ON");
            digitalWrite(mcuOlive,HIGH);digitalWrite(mcuWhite,LOW);digitalWrite(mcuRed,LOW);digitalWrite(mcuBlack,LOW);
            Serial.print(digitalRead(mcuOlive));Serial.print(digitalRead(mcuWhite));Serial.print(digitalRead(mcuRed));Serial.println(digitalRead(mcuBlack));
            delay(5000);
            }
          if(olive==1 && white==1 && red==0 && black==0){
            Serial.println("Light OFF");
            digitalWrite(mcuOlive,HIGH);digitalWrite(mcuWhite,HIGH);digitalWrite(mcuRed,LOW);digitalWrite(mcuBlack,LOW);
            Serial.print(digitalRead(mcuOlive));Serial.print(digitalRead(mcuWhite));Serial.print(digitalRead(mcuRed));Serial.println(digitalRead(mcuBlack));
            delay(5000);
            }
          if(olive==1 && white==1 && red==1 && black==0){
            Serial.println("Fan ON");
            digitalWrite(mcuOlive,HIGH);digitalWrite(mcuWhite,HIGH);digitalWrite(mcuRed,HIGH);digitalWrite(mcuBlack,LOW);
            Serial.print(digitalRead(mcuOlive));Serial.print(digitalRead(mcuWhite));Serial.print(digitalRead(mcuRed));Serial.println(digitalRead(mcuBlack));
            delay(5000);
            }
          if(olive==0 && white==1 && red==1 && black==1){
            Serial.println("Fan OFF");
            digitalWrite(mcuOlive,LOW);digitalWrite(mcuWhite,HIGH);digitalWrite(mcuRed,HIGH);digitalWrite(mcuBlack,HIGH);
            Serial.print(digitalRead(mcuOlive));Serial.print(digitalRead(mcuWhite));Serial.print(digitalRead(mcuRed));Serial.println(digitalRead(mcuBlack));
            delay(5000);
            }
          if(olive==1 && white==1 && red==0 && black==1){
            Serial.println("Window OPEN");
            digitalWrite(mcuOlive,HIGH);digitalWrite(mcuWhite,HIGH);digitalWrite(mcuRed,LOW);digitalWrite(mcuBlack,HIGH);
            Serial.print(digitalRead(mcuOlive));Serial.print(digitalRead(mcuWhite));Serial.print(digitalRead(mcuRed));Serial.println(digitalRead(mcuBlack));
            delay(5000);
            }
          if(olive==1 && white==0 && red==1 && black==1){
            Serial.println("Window CLOSE");
            digitalWrite(mcuOlive,HIGH);digitalWrite(mcuWhite,LOW);digitalWrite(mcuRed,HIGH);digitalWrite(mcuBlack,HIGH);
            Serial.print(digitalRead(mcuOlive));Serial.print(digitalRead(mcuWhite));Serial.print(digitalRead(mcuRed));Serial.println(digitalRead(mcuBlack));
            delay(5000);
            }
          if(olive==1 && white==0 && red==0 && black==1){
            Serial.println("FAN CONTROL MODE");
            while(1){
                  
                  // if programming failed, don't try to do anything
                  if (!dmpReady) return;

                  // wait for MPU interrupt or extra packet(s) available
                  while (!mpuInterrupt && fifoCount < packetSize) {
                      // other program behavior stuff here
                      // .
                      // .
                      // .
                      // if you are really paranoid you can frequently test in between other
                      // stuff to see if mpuInterrupt is true, and if so, "break;" from the
                      // while() loop to immediately process the MPU data
                      // .
                      // .
                      // .
                  }

                  // reset interrupt flag and get INT_STATUS byte
                  mpuInterrupt = false;
                  mpuIntStatus = mpu.getIntStatus();

                  // get current FIFO count
                  fifoCount = mpu.getFIFOCount();

                  // check for overflow (this should never happen unless our code is too inefficient)
                  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
                      // reset so we can continue cleanly
                      mpu.resetFIFO();
                  //    Serial.println(F("FIFO overflow!"));

                  // otherwise, check for DMP data ready interrupt (this should happen frequently)
                  } else if (mpuIntStatus & 0x02) {
                      // wait for correct available data length, should be a VERY short wait
                      while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

                      // read a packet from FIFO
                      mpu.getFIFOBytes(fifoBuffer, packetSize);
                      
                      // track FIFO count here in case there is > 1 packet available
                      // (this lets us immediately read more without waiting for an interrupt)
                      fifoCount -= packetSize;

                      #ifdef OUTPUT_READABLE_YAWPITCHROLL
                          // display Euler angles in degrees
                          mpu.dmpGetQuaternion(&q, fifoBuffer);
                          mpu.dmpGetGravity(&gravity, &q);
                          mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
                         // Serial.print("ypr\t");
                          //Serial.print(ypr[0] * 180/M_PI);
                          //Serial.print("\t");
                         // Serial.print(ypr[1] * 180/M_PI);
                         // Serial.print("\t");
                          //Serial.println(ypr[2] * 180/M_PI);
                      #endif
                      yaw=ypr[0] * 180/M_PI;
                      pitch=ypr[1] * 180/M_PI;
                      roll=ypr[2] * 180/M_PI;
                      
                      if(pitch<0){
                        Serial.println("decrease speed");
                        digitalWrite(mcuOlive,LOW);digitalWrite(mcuWhite,HIGH);digitalWrite(mcuRed,LOW);digitalWrite(mcuBlack,LOW);
                        Serial.print(digitalRead(mcuOlive));Serial.print(digitalRead(mcuWhite));Serial.print(digitalRead(mcuRed));Serial.println(digitalRead(mcuBlack));
                        delay(3000);
                      }
                      else if(pitch>50 ){
                        digitalWrite(mcuOlive,LOW);digitalWrite(mcuWhite,LOW);digitalWrite(mcuRed,HIGH);digitalWrite(mcuBlack,LOW);
                        Serial.print(digitalRead(mcuOlive));Serial.print(digitalRead(mcuWhite));Serial.print(digitalRead(mcuRed));Serial.println(digitalRead(mcuBlack));
                        Serial.println("increase speed");
                        delay(3000);
                      }
                      
                      // blink LED to indicate activity
                      blinkState = !blinkState;
                      digitalWrite(LED_PIN, blinkState);
                    }  
                                  
                        rawOliveA0= analogRead(analogPinOliveA0);
                        rawWhiteA1= analogRead(analogPinWhiteA1);
                        rawRedA2= analogRead(analogPinRedA2);
                        rawBlackA3= analogRead(analogPinBlackA3);
                        
                        bufferOlive= rawOliveA0 * Vin;
                        bufferWhite= rawWhiteA1 * Vin;
                        bufferRed= rawRedA2 * Vin;
                        bufferBlack= rawBlackA3 * Vin;
                  
                        VoutOliveA0= (bufferOlive)/1024.0;
                        VoutWhiteA1= (bufferWhite)/1024.0;
                        VoutRedA2= (bufferRed)/1024.0;
                        VoutBlackA3= (bufferBlack)/1024.0;
                  
                      if(VoutOliveA0<=1.45)
                          olive=0;
                        else
                          olive=1;
                  
                        if(VoutWhiteA1<=1.35)
                          white=0;
                        else
                          white=1;
                  
                        if(VoutRedA2<=1.93)
                          red=0;
                        else
                          red=1;
                  
                        if(VoutBlackA3<=2.36)
                          black=0;
                        else
                          black=1;

                      if(olive==0 && white==0 && red==0 && black==0)
                        {
                          delay(2000);
                          if(olive==0 && white==0 && red==0 && black==0){
                            Serial.println("Out Of Fan Control Mode");

                            break;}
                        }
                  }
                    
              }
                        delay(2000);
                        rawOliveA0= analogRead(analogPinOliveA0);
                        rawWhiteA1= analogRead(analogPinWhiteA1);
                        rawRedA2= analogRead(analogPinRedA2);
                        rawBlackA3= analogRead(analogPinBlackA3);
                        
                        bufferOlive= rawOliveA0 * Vin;
                        bufferWhite= rawWhiteA1 * Vin;
                        bufferRed= rawRedA2 * Vin;
                        bufferBlack= rawBlackA3 * Vin;
                  
                        VoutOliveA0= (bufferOlive)/1024.0;
                        VoutWhiteA1= (bufferWhite)/1024.0;
                        VoutRedA2= (bufferRed)/1024.0;
                        VoutBlackA3= (bufferBlack)/1024.0;
                  
                      if(VoutOliveA0<=1.45)
                          olive=0;
                        else
                          olive=1;
                  
                        if(VoutWhiteA1<=1.35)
                          white=0;
                        else
                          white=1;
                  
                        if(VoutRedA2<=1.93)
                          red=0;
                        else
                          red=1;
                  
                        if(VoutBlackA3<=2.36)
                          black=0;
                        else
                          black=1;

          if(olive==0 && white==0 && red==0 && black==0)
          {
            delay(2000);
            if(olive==0 && white==0 && red==0 && black==0){
              Serial.println("Out Of Mode");
              break;}
            }
          }
        }
      delay(3000);
 }
}
