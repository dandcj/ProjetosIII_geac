# ProjetosIII_geac
Simulador AC inverter vs ON/OFF

## Descrição do projeto:

O projeto consiste em simular um ar condicionado inverter e um tradicional ON/OFF.

O sistema de refrigeração é composto por um compressor (utilizado no simulador pastilha peltier), um evaporador, que é simulado com um dissipador com uma ventoinha e um condensador, simulado também com um dissipador maior com duas ventoinhas.

O objetivo é explorar os conceitos do controle PID, onde variando a tensão da peltier conseguimos um controle mais estável da temperatura e também uma redução de energia, mesmo concito utilizado nos sistemas inverter quando o compressor desacelera.

A ponteH BTS7960 é utilizada para controle da peltier por conta da corrente consumida pelas pastilhas

Já  ponteH l298n é utilizada para controle das ventoinhas do “evaporador” e “condensador”



Para funcionar o programa criar um arquivo na pasta raiz:


**credentials.h**

```
char networkSSID[] = "NOME DA SUA SSID"; // nome da rede wifi

char networkPASSWORD[] = "SENHA DO WIFI"; // senha do wifi

char mqttSERVER[] = "IP DO SERVIDO MQTT"; // IP do servido mqtt

char mqttUSERNAME[] = "";

char mqttPASSWORD[] = "";
```

###### Hardwares utilizados

- RaspBerryPI 3 [link]9https://www.raspberrypi.org/products/raspberry-pi-3-model-b/0
- Arduino nano IOT 33 [link](https://store.arduino.cc/usa/nano-33-iot)
- Ponteh l298n [link](https://www.baudaeletronica.com.br/driver-motor-ponte-h-l298n.html)
- Ponteh BTS7960 [link](https://produto.mercadolivre.com.br/MLB-1165220526-modulo-driver-ponte-h-43a-bts7960-ibt2-bts7960b-arduino-_JM?quantity=1)

###### Softwares utilizados

- VSconde com extensão Arduino
- Node-red para interface web e mobile
- PowerBi para gráficos


