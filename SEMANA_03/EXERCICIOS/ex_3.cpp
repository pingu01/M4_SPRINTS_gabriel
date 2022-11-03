const int LED1 = 40;
const int LED2 = 39;
const int LED3 = 38;
const int LED4 = 37;

const int LDR = 3;

const int BOTAO_SALVA = 5;
const int BOTAO_TOCA = 18;

const int BUZZER = 4;

int valores_lidos[100];

int nota_atual = 0;

void tocarMusica(int valor){
  //instrumento, frequencia, tempo
  tone(BUZZER, 300*(valor), 500);
}

void acende(int valor){
  if(valor == 15){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  } else if (valor == 14){
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  } else if (valor == 13){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  } else if (valor == 12){
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  } else if (valor == 11){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  } else if (valor == 10){
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, HIGH);
  } else if (valor == 9){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, HIGH);
  } else if (valor == 8){
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, HIGH);
  } else if (valor == 7){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
  } else if (valor == 6){
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
  } else if (valor == 5){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
  } else if (valor == 4){
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, LOW);
  } else if (valor == 3){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  } else if (valor == 2){
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  } else if (valor == 1){
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  } else {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  }
}
void setup(){
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(BOTAO_SALVA, INPUT_PULLUP);
  pinMode(BOTAO_TOCA, INPUT_PULLUP);


  for (int i = 0; i<100; i++){
  valores_lidos[i] = -1;
  }
}

void loop(){
  // put your main code here, to run repeatedly:
  if(!digitalRead(BOTAO_SALVA)){
    int valor_luz = map(analogRead(LDR), 32, 4063, 0, 15);
    valores_lidos[nota_atual] = valor_luz;
    tocarMusica(valor_luz);
    acende(valor_luz);
    nota_atual += 1;
    delay(500);
    acende(0);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);

  }

  if(!digitalRead(BOTAO_TOCA)){
    for(int i = 0; i<nota_atual; i++){
      tocarMusica(valores_lidos[i]);
      acende(valores_lidos[i]);
      valores_lidos[i] = -1;
      delay(500);
    }
  }
}
