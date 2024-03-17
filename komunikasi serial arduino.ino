void setup() {
  Serial.begin(115200);
}

void loop() {

  int MQ4 = analogRead(A0);
  //int MQ6 = analogRead(A1);
  //int MQ135 = analogRead(A2);
  //int MQ138 = analogRead(A3);
  String msg = "*" + String(MQ4) + "#";

  //kirim ke ESP menggunakan komunikasi serial RXTX
  Serial.println(msg);

  if (Serial.available())  //apabila serial available, akan ngeread msg yg udh dikirim
  {
    String Data = "";  // Baca Data yang dikirim dari ESP
    while (Serial.available() > 0) {
      Data += char(Serial.read());
      delay(50);
    }
    Serial.println(Data);
  }
  delay(3000);
}