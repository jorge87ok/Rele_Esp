#include <ESP8266WiFi.h> //Biblioteca para conexão esp ao wifi
//#include <WiFiClient.h> //Biblioteca para conexão do cliente wifi
#include <NTPClient.h> //Bilbioteca NTP cliente
#include <WiFiUdp.h> //Conexão WiFi UDP
//#include <WiFi.h> //Conexão WiFi

//#include <WiFiSecureClient.h> //Segurança WiFi
#define rele 0 //Define GPIO0 
//int rele = 0; //Pino digita utilizado pelo relé do módulo
int tentativas = 5;
char ssid[]= "pilha";
char senha[] = "pilha870408";
int count = 0 ;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org", -10800);

// Defina os horários para ativar o relé (hora em formato 24h)
const int schedule[][2] = {
  {8, 0},   // 08:00
  {12, 0},  // 12:00
  {18, 0},   // 18:00
  {20, 5},  // 18:48
  {120, 7}  // 18:50
};

const int scheduleSize = sizeof(schedule) / sizeof(schedule[0]);

/*
const int hour = 7, 8, 9;
const int minute = 30, 0,;
*/

void setup() {
  // put your setup code here, to run once:
  pinMode(rele, OUTPUT); //rele entrada
  digitalWrite(rele, HIGH); // O relé normalmente está desligado

  Serial.begin(9600); //Definição do Baud rate; o mesmo pode ser alterado pelo serial monitor
  Serial.println("Iniciando"); //Imprime a mensagem: Iniciando
  delay(1000); //delay 1s
  WiFi.begin(ssid, senha);

  Serial.print("Conectando...");
  while(WiFi.status() !=WL_CONNECTED){
    delay(2000);
    Serial.print(".");
    tentativas--;
    if (tentativas ==0){
      Serial.println();
      Serial.println("Time-out. Verifique as configurações e tente novamente.");
      while(true);
    }
    }
  Serial.println();
  Serial.print("Conectado em ");
  Serial.println(ssid);

  timeClient.begin();
  delay(2000); //esperar 2s
  timeClient.update();


}

void loop() {
  // put your main code here, to run repeatedly:

  delay(2000); //esperar 2s

  Serial.println(timeClient.getFormattedTime());
  //Serial.println(timeClient.getDay());


  // Verifique se o horário atual corresponde a algum horário na lista
  bool relayShouldBeOn = false;
  for (int i = 0; i < scheduleSize; i++) {
    if (schedule[i][0] == timeClient.getHours() && schedule[i][1] == timeClient.getMinutes()) {
      relayShouldBeOn = true;
      break;
    }
  }

  // Acione ou desative o relé conforme necessário
  if (relayShouldBeOn) {
    digitalWrite(rele, LOW);
    delay(2000);
    digitalWrite(rele, HIGH);
    timeClient.update();

  } 
  
    
  
  
  // Aguarde 60 segundos antes de verificar novamente
    delay(60000);


 /* if (hour == timeClient.getHours() && minute == timeClient.getMinutes()){
    digitalWrite(rele,HIGH); //ligar o rele
    Serial.println("Relé Ligado");
    delay(2000); //esperar 2s
    digitalWrite(rele,LOW); //desligar o rele
    Serial.println("Relé Desligado");
    delay(60000);

    

  }*/

  
}
