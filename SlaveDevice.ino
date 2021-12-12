#define PIR_MOTION_SENSOR 8//Use pin 2 to receive the signal from the module 
#define LED  13//the Grove - LED is connected to D4 of Arduino
#define IN 1
#define OUT 0

int gas_sensor = A5; //Sensor pin
float m = -0.318; //Slope
float b = 1.133; //Y-Intercept
float R0 = 0.77; //Sensor Resistance in fresh air from previous code
double sensorValue = 0.0;
int stateBuzer = OUT;
int stateGS = OUT;
void setup() {
  // put your setup code here, to run once:
  pinMode(PIR_MOTION_SENSOR, INPUT);
  pinMode(LED,OUTPUT);
  
  delay(2000);
  Serial.begin(9600);
}

void loop() {
   sensorValue = getValuesFromSensor();
   // if gas or smoke is detected turn on buzer
  if (sensorValue > 0.10){
    digitalWrite(9, HIGH);
    digitalWrite(13, HIGH);
    if (stateGS == OUT){
      Serial.write(1);
      stateGS = IN;
    }
  }
  else{
   stateGS = OUT;
   digitalWrite(9, LOW);
   digitalWrite(13, LOW);
  }
  if(isPeopleDetected()){//if it detects the moving people?
    
    if (stateBuzer == OUT){
    Serial.write(2);
      stateBuzer = IN;
    }
  }
  else{
    stateBuzer = OUT;

  }   
}
double getValuesFromSensor(){
  float sensor_volt=0; //Define variable for sensor voltage
  float RS_gas=0; //Define variable for sensor resistance
  float ratio=0; //Define variable for ratio
  float sensorValue = analogRead(gas_sensor); //Read analog values of sensor
  sensor_volt = sensorValue * (5.0 / 1023.0); //Convert analog values to voltage
  RS_gas = ((5.0 * 1.0) / sensor_volt) - 1.0; //Get value of RS in a gas
  ratio = RS_gas / R0;   // Get ratio RS_gas/RS_air
                    
  double ppm_log = (log10(ratio) - b) / m; //Get ppm value in linear scale according to the the ratio value
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale
  double percentage = ppm / 10000; //Convert to percentage
  return percentage;
}
boolean isPeopleDetected()
{
  int sensorValue = digitalRead(PIR_MOTION_SENSOR);
  if(sensorValue == HIGH)//if the sensor value is HIGH?
  {
    return true;//yes,return ture
  }
  else
  {
    return false;//no,return false
  }
}
