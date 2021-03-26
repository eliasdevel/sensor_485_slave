 String inputString = "";
 String message[10];
 boolean cameMessage = false;
 int pos = 0;

#include <EEPROM.h>

void setup() {
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(4, LOW);
  Serial.begin(9600);
  
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
    //TODO colocar temperatura do sensor
    Serial.println(message[0]+"|10");
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
