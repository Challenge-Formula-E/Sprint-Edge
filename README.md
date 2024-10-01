
# Projeto IoT: ESP32 com Sensores de Temperatura e MPU6050

Este projeto demonstra como usar um microcontrolador ESP32 para ler dados de vários sensores de temperatura (TMP36) e um acelerômetro (MPU6050). O ESP32 conecta-se a uma rede Wi-Fi e envia os dados para um servidor usando HTTP em formato JSON. Isso é útil para o monitoramento em tempo real em várias aplicações de IoT.

## Sumário

- [Visão Geral do Projeto](#visão-geral-do-projeto)
- [Componentes de Hardware](#componentes-de-hardware)
- [Requisitos de Software](#requisitos-de-software)
- [Diagrama de Circuito](#diagrama-de-circuito)
- [Explicação do Código](#explicação-do-código)
- [Instruções de Configuração](#instruções-de-configuração)
- [Como Usar](#como-usar)
- [Licença](#licença)

## Visão Geral do Projeto

Este projeto coleta dados de quatro sensores de temperatura TMP36 e de um acelerômetro MPU6050. O ESP32 lê os dados, processa-os e os envia para um servidor web. Os dados enviados incluem:

- Leituras de temperatura dos sensores TMP36
- Aceleração e velocidade calculadas a partir do sensor MPU6050

O ESP32 usa a rede Wi-Fi para enviar os dados em uma requisição HTTP POST no formato JSON para um servidor. Este projeto é útil para sistemas de monitoramento baseados em IoT, como monitoramento ambiental, aplicações industriais ou monitoramento remoto de sensores.

## Componentes de Hardware

- **ESP32** microcontrolador
- **4 x Sensores de Temperatura TMP36** (conectados aos pinos analógicos)
- **Acelerômetro MPU6050** (conectado via I2C)
- Protoboard, cabos jumper e fonte de alimentação

### Conexões dos Pinos

| Sensor/Dispositivo       | Pino do ESP32   |
|--------------------------|-----------------|
| TMP36 (Sensor de Temp 1)  | GPIO 36         |
| TMP36 (Sensor de Temp 2)  | GPIO 39         |
| TMP36 (Sensor de Temp 3)  | GPIO 34         |
| TMP36 (Sensor de Temp 4)  | GPIO 35         |
| MPU6050 SCL               | GPIO 17         |
| MPU6050 SDA               | GPIO 14         |
| VCC (TMP36 & MPU6050)     | 3.3V/5V         |
| GND (TMP36 & MPU6050)     | GND             |

## Requisitos de Software

- **IDE Arduino** com a placa ESP32 configurada
- Bibliotecas:
  - **Adafruit MPU6050**
  - **Adafruit Unified Sensor**
  - **WiFi** para ESP32

## Explicação do Código

O código realiza as seguintes tarefas:

1. **Conexão Wi-Fi**: O ESP32 conecta-se a uma rede Wi-Fi usando o nome (SSID) e a senha fornecidos.
2. **Leitura de Sensores**: O ESP32 lê dados de quatro sensores de temperatura TMP36 conectados aos pinos analógicos e do acelerômetro MPU6050, que mede a aceleração em três eixos.
3. **Processamento dos Dados**: O código processa os dados para calcular a aceleração total e a velocidade.
4. **Envio para o Servidor**: Os dados são enviados para um servidor via HTTP POST, em formato JSON, contendo as leituras de temperatura e aceleração.

### Exemplo de Estrutura JSON Enviada:

\`\`\`json
{
  "api_key": "SUA_CHAVE_API",
  "acc": "3.25",
  "spd": "1.15",
  "tmp1": "22.5",
  "tmp2": "23.0",
  "tmp3": "24.1",
  "tmp4": "22.8"
}
\`\`\`

## Instruções de Configuração

1. **Instalar as Bibliotecas Necessárias**:
   - Na IDE Arduino, vá em **Sketch > Incluir Biblioteca > Gerenciar Bibliotecas** e instale as seguintes bibliotecas:
     - **Adafruit MPU6050**
     - **Adafruit Unified Sensor**
     - **WiFi para ESP32**

2. **Configurar o Código**:
   - Abra o arquivo `.ino` no Arduino IDE.
   - Atualize os parâmetros de rede com o nome da sua rede Wi-Fi (SSID) e a senha:
     \`\`\`cpp
     String ssid = "Seu_SSID";
     String pwrd = "Sua_Senha";
     \`\`\`
   - Insira a URL do seu servidor e a chave da API:
     \`\`\`cpp
     String server = "URL_DO_SEU_SERVIDOR";
     String API_KEY = "SUA_CHAVE_API";
     \`\`\`

3. **Montar o Circuito**:
   - Siga o diagrama de conexões para conectar os sensores de temperatura TMP36 aos pinos analógicos do ESP32 e o MPU6050 aos pinos I2C (SDA e SCL).

4. **Upload do Código**:
   - Conecte o ESP32 ao seu computador e faça o upload do código usando a IDE Arduino.

## Como Usar

1. Após carregar o código no ESP32, ele tentará conectar à rede Wi-Fi configurada.
2. O ESP32 começará a ler os dados dos sensores de temperatura e do acelerômetro.
3. Os dados serão enviados para o servidor configurado via HTTP POST.
4. Verifique no console serial da IDE Arduino para monitorar as leituras dos sensores e as respostas do servidor.

## Licença

Este projeto é distribuído sob a licença MIT. Sinta-se à vontade para modificar e usar conforme necessário.
