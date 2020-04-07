#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "credentials.h"
#include <math.h>
#include <PID_v1.h>

//PID
double Setpoint, Input, Output;

double consKp = 2, consKi = 0.8, consKd = 1;

PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
//PID

//PonteH l298
int ENA = 5;
int IN1A = 4;
int IN2A = 3;

int ENB = 6;
int IN1B = 7;
int IN2B = 8;
//PonteH l298

//PonteH ponthBTS7960B
int pwmBt = 9;
int lBt = 10;
int rBt = 11;
//PonteH ponthBTS7960B

//Sensores NTC
unsigned int Rs = 150000;
double Vcc = 3.26;
int filtro = 500;
//Sensores NTC

//Variables
double vSetpointDouble = 40;
double vDiferencialDouble = 2;
int statusRef = 0;
//Variables

//Conversões publish
String H1_str;
char H1_value[50];
double H1;

String H2_str;
char H2_value[50];
double H2;

String H3_str;
char H3_value[50];
double H3;

String H4_str;
char H4_value[50];
double H4;

String H5_str;
char H5_value[50];
double H5;
//Conversões publish

//WIFI - MQTT
const char *ssid = networkSSID;
const char *password = networkPASSWORD;
const char *mqttServer = mqttSERVER;
const char *mqttUsername = mqttUSERNAME;
const char *mqttPassword = mqttPASSWORD;

WiFiClient wifiClient;
PubSubClient client(wifiClient);
//WIFI - MQTT

void setup_wifi()
{

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);

  //PID
  myPID.SetOutputLimits(0, 255);
  myPID.SetMode(AUTOMATIC);
  myPID.SetControllerDirection(REVERSE);
  //PID

  //ponteh l395
  pinMode(ENA, OUTPUT);
  pinMode(IN1A, OUTPUT);
  pinMode(IN2A, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN1B, OUTPUT);
  pinMode(IN2B, OUTPUT);
  //ponteh l395

  //PonteH ponthBTS7960B
  pinMode(pwmBt, OUTPUT);
  pinMode(lBt, OUTPUT);
  pinMode(rBt, OUTPUT);
  //PonteH ponthBTS7960B
}

void callback(char *topico, byte *mensagem, unsigned int tamanho)
{
  mensagem[tamanho] = '\0';
  String strMensagem = String((char *)mensagem);
  String srtTopico = String((char *)topico);

  if (srtTopico == "setpoint")
  {
    String vSetpoint = strMensagem;
    vSetpointDouble = vSetpoint.toDouble();
    Setpoint = vSetpoint.toDouble();
    Serial.println(vSetpointDouble);
  }

  if (srtTopico == "diferencial")
  {
    String vDiferencial = strMensagem;
    vDiferencialDouble = vDiferencial.toDouble();
    Serial.println(vDiferencialDouble);
  }

  if (strMensagem == "trigger")
  {
    publica();
  }

  else if (strMensagem == "refOn")
  {
    refOn();
  }

  else if (strMensagem == "refOff")
  {
    refOff();
  }

  else if (strMensagem == "trigger")
  {
    publica();
    
  }

  else if (strMensagem == "fanEvaporadorOn")
  {
    fanEvaporadorOn();
    Serial.println("Evap ON");
  }

  else if (strMensagem == "fanEvaporadorOff")
  {
    fanEvaporadorOff();
    Serial.println("Evap Off");
  }

  else if (strMensagem == "fanCondensadorOn")
  {
    fanCondensadorOn();
    Serial.println("Cond ON");
  }

  else if (strMensagem == "fanCondensadorOff")
  {
    fanCondensadorOff();
    Serial.println("Cond Off");
  }
}

void fanEvaporadorOn()
{
  analogWrite(ENA, 255);
  digitalWrite(IN1A, HIGH);
  digitalWrite(IN2A, LOW);
}

void fanEvaporadorOff()
{
  analogWrite(ENA, 0);
  digitalWrite(IN1A, LOW);
  digitalWrite(IN2A, LOW);
}

void fanCondensadorOn()
{
  analogWrite(ENB, 255);
  digitalWrite(IN1B, HIGH);
  digitalWrite(IN2B, LOW);
}

void fanCondensadorOff()
{
  analogWrite(ENB, 0);
  digitalWrite(IN1B, LOW);
  digitalWrite(IN2B, LOW);
}

void refOn()
{
  analogWrite(pwmBt, 255);
  digitalWrite(lBt, HIGH);
  digitalWrite(rBt, LOW);
  statusRef = 1;
  }

void refOff()
{
  analogWrite(pwmBt, 0);
  digitalWrite(lBt, LOW);
  digitalWrite(rBt, LOW);
  statusRef = 0;
  
}

void publica()
{
  H1 = Thermister(AnalogRead(A1));
  H1_str = String(H1);
  H1_str.toCharArray(H1_value, H1_str.length() + 1);
  client.publish(/*topic*/ "S1", H1_value);

  H2 = Thermister(AnalogRead(A2));
  H2_str = String(H2);
  H2_str.toCharArray(H2_value, H2_str.length() + 1);
  client.publish(/*topic*/ "S2", H2_value);

  H3 = Thermister(AnalogRead(A3));
  H3_str = String(H3);
  H3_str.toCharArray(H3_value, H3_str.length() + 1);
  client.publish(/*topic*/ "S3", H3_value);
/*
  H4 = Thermister(AnalogRead(A4));
  H4_str = String(H4);
  H4_str.toCharArray(H4_value, H4_str.length() + 1);
  client.publish( "S4", H4_value);*/

  //H5 = Thermister(AnalogRead(A5));
  H5_str = String(statusRef);
  H5_str.toCharArray(H5_value, H5_str.length() + 1);
  client.publish(/*topic*/ "sRefrigeracao", H5_value);

}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ArduinoClient-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqttUsername, mqttPassword))
    {
      Serial.println("connected");
      // ... and resubscribe
      client.subscribe("inTopic");
      client.subscribe("setpoint");
      client.subscribe("diferencial");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  
  client.loop();
/*
  Input = H3;
  myPID.SetTunings(consKp, consKi, consKd);
  myPID.Compute();
  refOn();

 

  Serial.print("Output: ");
  Serial.print(Output);
  Serial.print("   Input: ");
  Serial.print(Input);
  Serial.print("   Setpoint: ");
  Serial.println(Setpoint);*/

 if (H3 >= vSetpointDouble + vDiferencialDouble)
  {
    refOn();
  }
  else if (H3 <= vSetpointDouble)
  {
    refOff();
  }

  delay(10);
}

int AnalogRead(int pin)
{
  int val = 0;

  for (int i = 0; i < filtro; i++)
  {
    val += analogRead(pin);
    delay(1);
  }

  val = val / filtro;

  return val;
}

double Thermister(int val)
{
  double V_NTC = (double)val / 1024;
  double R_NTC = (Rs * V_NTC) / (Vcc - V_NTC);
  R_NTC = log(R_NTC);
  double Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * R_NTC * R_NTC)) * R_NTC);
  Temp = Temp - 303.00; //273.15
  return Temp;
}