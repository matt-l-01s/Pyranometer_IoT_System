// add code here

void setup() {
  // put your setup code here, to run once:
int sensor = A0;
pinMode(sensor,INPUT);
analogReference(INTERNAL1V1);
Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
float sensor_voltage;
float sensitivity = 0.0002021;
float solar_irradiance;
float reference_voltage = 1.1;

int analog_value = analogRead(A0);
sensor_voltage = (analog_value * reference_voltage) / 1024.0; 
solar_irradiance = (sensor_voltage / sensitivity);
Serial.println(solar_irradiance);
delay(1000); 
}
