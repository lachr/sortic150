#include <Arduino.h>
#include <SPI.h>

#include <SorticMachine.h>
#include <Chassis.h>
#include <Placer.h>
#include <MachineAPI.h>
#include <Component.h>
#include <Config.h>
#include <Debug.h>

Adafruit_MotorShield currentMotorShield{};
Adafruit_DCMotor *driverMotor = currentMotorShield.getMotor(MOTOR_NR);
MFRC522 partDetector{RFIDDETECTOR_SELECT, RFIDDETECTOR_POWEROFF};
SoftwareSerial bluetooth{BLUETOOTH_TX, BLUETOOTH_RX};

static Component<PlacerPosition> *placer = new Placer{bluetooth};
static Component<byte *> *rfidDetector = new RfidDetector{partDetector};
static Component<int> *chassis = new Chassis{driverMotor, DISTANCE_SENSOR};
static Component<Config> *machineAPI = new MachineAPI{};
static Component<Config> *sorticMachine = new SorticMachine{placer, rfidDetector, chassis, machineAPI};

void setup()
{
  Serial.begin(9600);
  currentMotorShield.begin();
  SPI.begin();
  bluetooth.begin(57600);
  partDetector.PCD_Init();
  machineAPI->on();
  debugLn("Setup Ready");
}

void loop()
{
  machineAPI->executeOneStep();
  machineAPI->getData().powerOn ? sorticMachine->on() : sorticMachine->off();

  sorticMachine->setAction(machineAPI->getData());
  sorticMachine->executeOneStep();
}
