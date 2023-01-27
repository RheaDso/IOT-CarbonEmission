#include <MQUnifiedsensor.h>
#define board "NodeMCU"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  


MQUnifiedsensor MQ135(board, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

void setup() {
  
  Serial.begin(9600); //Init serial port


  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  
  MQ135.init(); 
  
   Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
 
}

void loop() {

delay(1000);
    int gasLevel= 0;
    
     gasLevel = analogRead(A0);
      Serial.print("gas level "); Serial.print(gasLevel); 

  if(gasLevel<175)
  {
    Serial.print ("\nGOOD!      " );
  }
  else if (gasLevel >175 && gasLevel<225){
       Serial.print ("\nOKAY  ");
  }
  else if (gasLevel >225 && gasLevel<300){
   Serial.print ("\nBAD");
  }
  else if (gasLevel >300)
  {
    Serial.print ("\nvery bad");
  }
    
  MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
  float CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(77.255); MQ135.setB(-3.18); // Configurate the ecuation values to get Alcohol concentration
  float Alcohol = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
  float CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(44.947); MQ135.setB(-3.445); // Configurate the ecuation values to get Tolueno concentration
  float Tolueno = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration
  float NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(34.668); MQ135.setB(-3.369); // Configurate the ecuation values to get Acetona concentration
  float Acetona = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

    Serial.print("\n\nCO level "); Serial.print(CO); 
  Serial.print("\nAlcohol level "); Serial.print(Alcohol); 
  Serial.print("\nCO2 level "); Serial.print(CO2); 
  Serial.print("\nTolueno level "); Serial.print(Tolueno); 
  Serial.print("\nNH4 level "); Serial.print(NH4); 
  Serial.print("\nAcetona "); Serial.print(Acetona);
   Serial.println("\n");
   
}
