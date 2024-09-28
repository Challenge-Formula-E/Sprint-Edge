/*  Este é o código utilizado para o sensoriamento de carros de Fórmula E, bem como upload de dados através da API!
    Desenvolvido por:
      ***
      ##### NOME #####    - ##RM##
      Matheus Queiroz     - 558801
      Matheus Vinícius    - 555177
      Enzo Miletta        - 098677
      Marcela Torro       - 557658
      Lucas Son           - 554724
      ***
    
    São necessários os seguintes sensores:
      - MPU6050 (acelerômetro e gyro)
      - 4xTMP36 (temperatura: ambiente, carcaça do motor, estator e bateria)
*/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include <WiFi.h>
#include <HTTPClient.h>

// Parâmetros de rede
String server = "<URL_SERVIDOR>";
String API_KEY = "<CHAVE_API>";

String ssid = "<Nome da Rede>";
String pwrd = "<Senha da Rede>";

// Objeto MPU
Adafruit_MPU6050 mpu;

// Pinos dos TMP36
const int tempPin1 = 2;
const int tempPin2 = 3;
const int tempPin3 = 4;
const int tempPin4 = 5;

// Floats para guardar a temperatura, em ºC
float temp1;
float temp2;
float temp3;
float temp4;

float ac = 0, acx = 0, acy = 0, acz = 0, acg = 0;
float v = 0, vx = 0, vy = 0, vz = 0;

float t0 = 0, ti;

void setup(void){
  Serial.begin(115200);
  while (!Serial)
    delay(10); // Aguardando terminal abrir

  // Inicialização do MPU
  Serial.println("Inicializando MPU!");
  if (!mpu.begin()) { // mpu.begin(): método da biblioteca
    Serial.println("MPU não encontrado, tentando novamente");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Encontrado!");
  
  // Configuração do MPU, mudar caso necessário
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);  // Configura resolução do acelerômetro
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);        // Configura resolução do gyro
  Serial.println("MPU6050 configurado para: +-16G, +-250º/s");

  // Inicia conexão à internet
  WiFi_init(ssid, pwrd);

  Serial.println("Tudo certo!");
  delay(100);
  }

void loop(){
  ti = millis(); // Tempo inicial para integração
  // Integra a aceleração para achar a velocidade
  vx = vx + acx*(ti - t0);
  vy = vy + acy*(ti - t0);
  vz = vz + acz*(ti - t0);
  v = sqrt(sq(vx)+sq(vy)+sq(vz)); // Calcula o módulo da velocidade
  Serial.print("Velocidade: ");
  Serial.print(v);
  Serial.print("m/s, ");

  // Coleta dados das acelerações em x, y, z do MPU6050
  sensors_event_t a, g, temp;  // Objetos de leitura da biblioteca também
  mpu.getEvent(&a, &g, &temp); // &g e &temp podem ser utilizados caso necessário, representam o gyro e o sensor de temperatura embutido do MPU6050

  t0 = ti;                      // Segundo tempo para integração
  acx = a.acceleration.x;
  acy = a.acceleration.y;
  acz = a.acceleration.z;
  ac  = sqrt(sq(acx)+sq(acy)+sq(acz)); // Aceleração absoluta
  acg = ac/9.81;                       // Aceleração em G's
  Serial.print("Aceleração: ");
  Serial.print(ac);
  Serial.print("m/s^2, ");
  Serial.print(acg);
  Serial.println("G's");

  // Coleta e calcula temperaturas: /1023.0 por ser analog, -0.5 constante de correção do sensor segundo o datasheet, *100 corrige de % para ºC 
  temp1 = (analogRead(tempPin1)/1023.0-0.5)*100;
  temp2 = (analogRead(tempPin2)/1023.0-0.5)*100;
  temp3 = (analogRead(tempPin3)/1023.0-0.5)*100;
  temp4 = (analogRead(tempPin4)/1023.0-0.5)*100;

  Serial.print("Temperatura sensor 1: ");
  Serial.println(temp1);
  Serial.print("Temperatura sensor 2: ");
  Serial.println(temp2);
  Serial.print("Temperatura sensor 3: ");
  Serial.println(temp3);
  Serial.print("Temperatura sensor 4: ");
  Serial.println(temp4);

  // Verifica conexão Wi-Fi
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Rede OK!");
    WiFiClient client;  // Objeto WiFi de cliente na rede local
    HTTPClient http;    // Objeto HTTP de subdomínio na rede local

    Serial.println("Chamando servidor...");
    // Inicializa servidor
    http.begin(client, server);
    // Define que header receberá parâmetros em JSON
    http.addHeader("Content-Type", "application/json");
    // String a ser passada como JSON com todos dados necessários
    String JSON_Data = " {\"api_key\":\""+API_KEY+"\",\"acc_sensor\":\"MPU6050\",\"acc\":\""+ac+"\",\"spd\":\""+v+"\",\"tmp_sensor\":\"TMP36\",\"tmp1\":\""+temp1+"\",\"tmp2\":\""+temp2+"\",\"tmp3\":\""+temp3+"\",\"tmp4\":\""+temp4+"\"}";
    Serial.print("POST JSON: ");
    Serial.println(JSON_Data);
    // Posta o JSON para a API
    int httpRCODE = http.POST(JSON_Data);
    
    Serial.print("Resposta HTTP: ");
    Serial.println(httpRCODE);

    // Coleta de recursos
    http.end();
  } else {
    // Caso desconectado, tenta conectar novamente
    Serial.println("WiFi Desconectado, reconectando...");
    WiFi_init(ssid, pwrd);
  }
  delay(10);
}

// Função para estabelecer rede
void WiFi_init(String ssid, String pwrd){
  Serial.print("Tentando estabelecer conexão com a rede: ");
  Serial.println(ssid);
  // Tenta conexão
  WiFi.begin(ssid, pwrd);
  Serial.println("Conectando...");
  // Aguarda conexão
  while(WiFi.status() != WL_CONNECTED) {
    Serial.println("...");
    delay(500);
  }
  Serial.print("\nConectado à rede Wi-Fi ");
  Serial.print(ssid);
  Serial.print(" com IP Local: ");
  Serial.println(WiFi.localIP());
}
