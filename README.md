# lorawan-atc
Envios e recebimetos de dados via lorawan atc everynet (Brasil)


- Utilizado para os testes Lora TTGO Esp32 com platformIo
- Plataforma da Everynet (ATC Brasil)
- Activation ABP
- Encription NS

#Primeiro edite o arquivo main.cpp com suas credencias:

//ABP Credentials 
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

#Altere o arquivo do platformio para seu equipamento em:
-> platformio.ini
upload_port=/dev/cu.wchusbserial1421
monitor_port=/dev/cu.wchusbserial1421

#Biblioteca editada
https://github.com/BeelanMX/Beelan-LoRaWAN

//Defido a região que opera na frequencia do Brasil
-> Config.h
#define AU_915

//Defindo os tempos de RX1 e RX2 conforme as especificições da ATC
->LoRaMAC.cpp
static const unsigned int Receive_Delay_1 = 5000;
static const unsigned int Receive_Delay_2 = 6000;

Exemplo de Downlink:
![alt text](https://github.com/geovaneferreira/lorawan-atc/blob/main/downlink-ok.png)


Exemplo das configurações:
![alt text](https://github.com/geovaneferreira/lorawan-atc/blob/main/configs.png)
