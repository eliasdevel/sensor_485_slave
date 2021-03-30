 String inputString = "";
 String message[10];
 boolean cameMessage = false;
 int pos = 0;

#include <EEPROM.h>
// Inclusao das bibliotecas
#include <OneWire.h>
#include <DallasTemperature.h>
float temperature =0;

const int PINO_ONEWIRE = 8; // Define pino do sensor
OneWire oneWire(PINO_ONEWIRE); // Cria um objeto OneWire
DallasTemperature sensor(&oneWire); // Informa a referencia da biblioteca dallas temperature para Biblioteca onewire
DeviceAddress endereco_temp; 

void setup() {
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(4, LOW);
  Serial.begin(9600);
  sensor.begin();
}

void serialEvent() {
  while (Serial.available()) {
    digitalWrite(13, HIGH);
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:

    //separador
    if (inChar == '|')
    {
      pos++;
    }
    else
    {
      message[pos] += inChar;
    }

    if (inChar == '\n') {
      cameMessage = true;
      pos = 0;
    }
  }
  
  digitalWrite(13, LOW);
}


void loop() {

  sensor.requestTemperatures(); // Envia comando para realizar a conversão de temperatura
  if (!sensor.getAddress(endereco_temp,0)) { // Encontra o endereco do sensor no barramento
    Serial.println("SENSOR NAO CONECTADO"); // Sensor conectado, imprime mensagem de erro
  } else {
    temperature =  sensor.getTempC(endereco_temp);
    
  }
  
  //configuração do sensor
  if (cameMessage && message[0] == "AA")
  {
    sensorNumberConfig();
    clearMessage();
  }

  //se está requisitando sensor
  else if (cameMessage && message[0].toInt() == EEPROM.read(0))
  {
    //Configura 485 para escrita
    digitalWrite(4, HIGH);
    //manda dado com temperatura devolta
    Serial.println(message[0]+"|"+temperature+"|");
    //Espera até q último byte ssaia do buffer
    while ((UCSR0A & _BV (TXC0)) == 0){}
    //volta para leitura
    digitalWrite(4, LOW);
    
    clearMessage();
  } else if (cameMessage)
  {
    clearMessage();
  }

}

void clearMessage()
{
  for (int i =0; i< 8; i++) message[i] = "";
  cameMessage = false;
}

void sensorNumberConfig() {
  //Configura 485 para escrita
  digitalWrite(4, HIGH);

  Serial.println("Configurando numeração do sensor");

  int sensorNumber = message[1].toInt();

  if (sensorNumber > 0 && sensorNumber < 100)
  {
    //Endereço da eeprom que define o numero do sensor
    EEPROM.write(0, sensorNumber);
    Serial.println("Sensor Configurado com sucesso!");
  }
  else
  {
    Serial.println("Configuração do sensor inválida");
  }

  cameMessage = false;
  //volta para leitura
  digitalWrite(4, LOW);
}
