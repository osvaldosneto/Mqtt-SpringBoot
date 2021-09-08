#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define ledVerde D3       //led verde sinalização ok
#define ledVermelho D2    //led vermelho de problemas
#define ledRele D4        //led rele
#define valvula D1        //valvula


char ssid[] = "Brisas do Farol_ext";
const char *password = "bris@s9156";
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
char topic[] = "/casinhas/irrigacao";
int error = 0;


WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


void conectWifiAndMqtt(){
  Serial.println();
  Serial.print("connectando em ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Wifi Conectado");
  Serial.println(WiFi.localIP());

  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
  if (!mqttClient.connected()){
    reconnectMqtt();
  }
  statusAtivo();
}


void callback(char* topic, byte* payload, unsigned int length) {
  char msg;
  Serial.print("Callback, ");
  Serial.print("Mensagem:");
  for (int i = 0; i < length; i++) {
    msg = (char)payload[i];
  }
  Serial.println(msg);
  if(msg == '1'){
    ativaIrrigacao();
    Serial.println("Ativando Irrigação");
  } else if (msg == '0') {
    inativaIrrigacao();
    Serial.println("Inativando Irrigação");
  } else {
    Serial.print("Problemas Mqtt");
    inativaIrrigacao();
    statusInativo();
  }
}


void reconnectMqtt() {
  Serial.println("Conectando ao MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconectando MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Conectado ao MQTT Broker");
        mqttClient.subscribe(topic);
      }
  }
}


void inicializacao(){
  for(int i=0; i<3; i++){
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, HIGH);       
    digitalWrite(ledRele, HIGH);
    delay(800);
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, LOW);       
    digitalWrite(ledRele, LOW);
    delay(800);
  }
}


void ativaIrrigacao(){
  Serial.println("Função ativa irrigação");
  digitalWrite(ledRele, HIGH);
  digitalWrite(valvula, HIGH);
}


void inativaIrrigacao(){
  digitalWrite(ledRele, LOW);
  digitalWrite(valvula, LOW);
}


void statusAtivo(){
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledVermelho, LOW); 
}


void statusInativo(){
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledVermelho, HIGH);
  digitalWrite(ledRele, LOW);
  digitalWrite(valvula, LOW);
}
  

void setup(){
  Serial.begin(115200);
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledRele, OUTPUT);
  pinMode(valvula, OUTPUT);
  inicializacao();
  statusInativo();
  conectWifiAndMqtt();
}


void loop (){
  if (!mqttClient.connected())
    reconnectMqtt();
  mqttClient.loop(); 
}
