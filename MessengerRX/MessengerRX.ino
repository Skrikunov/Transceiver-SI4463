//NL новая строка (режим порта)
#include <Si446x.h> //бииблиотека для Si4463

float temp;
float bat;
float RSsi;

#define CHANNEL 0 //номер канала
#define MAX_PACKET_SIZE 8 //максимальный размер пакета данных

#define PACKET_NONE    0
#define PACKET_OK   1
#define PACKET_INVALID  2
int inf = 0;
//ПРИЁМНИК
typedef struct {
  uint8_t ready;
  int16_t rssi;
  uint8_t length;
  uint8_t buffer[MAX_PACKET_SIZE];
} pingInfo_t;

static volatile pingInfo_t pingInfo;
bool SYNC_DET = 0;
bool PKT_RXed = 0; //флаг приёма пакета
byte RX_SIZE; //размер принятого пакета
int RXtime = 0; //момент получения пакета


//ПЕРЕДАТЧИК
byte power = 127; //мощность передатчика
uint8_t data[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool PKT_SNT = 0; //флаг отправки пакета
byte SENT_SIZE; //размер отправленного пакета
int TXtime = 0; //момент отправки пакета

bool me = 1;
bool ends = 0;

//колбек конца отправки пакета
//вкл через Si446x_setupCallback
void SI446X_CB_SENT(void)
{
  TXtime = millis();
  PKT_SNT = 1;
}

//индикатор правильного синхрослова
void SI446X_CB_RXBEGIN(int16_t rssi)
{
  RSsi = Si446x_getRSSI();
  //SYNC_DET = 1;
}

//колбек принятого и правильного пакета
//вкл всегда
void SI446X_CB_RXCOMPLETE(uint8_t length, int16_t rssi)
{
  RXtime = millis();
  PKT_RXed = 1;
  RX_SIZE = length;
}

void setup()
{
  Serial.begin(115200); //последовательный порт
  Si446x_init();  //инициализация Si446x
  Si446x_setTxPower(power); //установить мощность передатчика

  //разрешаем колбек конца отправки пакета
  Si446x_setupCallback(SI446X_CBS_SENT, 1);

  //разрешаем колбек начала принятого пакета
  Si446x_setupCallback(SI446X_CBS_RXBEGIN, 1);

  Si446x_RX(CHANNEL); //включаем приём по каналу
  //info();
  Serial.println("ПЕРЕДАТЧИК 2");
}

void loop() {
  if(SYNC_DET){
  SYNC_DET=!SYNC_DET;
  Serial.println();
  Serial.print("rssi=");
  Serial.println(RSsi);
}
  
  readSerial(); //проверка буфера последовательного порта
  verifyRX(); //проверка принятого пакета
}

void verifyRX() {
  //если пакет был успешно принят, то
  if (PKT_RXed) {
    PKT_RXed = 0; //обнуляем флаг приёма пакета
    //записываем данные из FIFO в буфер контроллера
    Si446x_read((uint8_t*)pingInfo.buffer, RX_SIZE);
    //если размер пакета ревышает предельный, то
    if (RX_SIZE > MAX_PACKET_SIZE) {
      RX_SIZE = MAX_PACKET_SIZE;  //ограничиваемся предельным размером пакета
    }

    //выводпринятого сообщения в последовательный порт
    for (int i = 0; i < RX_SIZE; i++) {
      /*Serial.print("buffer[");
        Serial.print(i);
        Serial.print("]=");*/
      //Serial.println(pingInfo.buffer[i],DEC);
      Serial.write(pingInfo.buffer[i]);
      //Serial.println();
      Serial.flush(); //ожидаем конца передачи данных
    }
    //подтверждаем получение пакета противоположной стороне
    Si446x_TX(data, sizeof(data), CHANNEL, SI446X_STATE_RX);
  }
}

void readSerial() {
  //если в буфере фарта есть данные
  if (Serial.available() > 0) {
    delay(50);
    byte AV_SIZE=Serial.available();
    //Serial.print("SA=");
    //Serial.println(AV_SIZE);
    


    if (AV_SIZE / MAX_PACKET_SIZE > 0) {
      SENT_SIZE = MAX_PACKET_SIZE;
      //Serial.print("M=");
    }
    else {
      SENT_SIZE = AV_SIZE % MAX_PACKET_SIZE;
      //Serial.print("NM=");
    }
    ends = 1; //флаг выхода из условия
    //Serial.println(SENT_SIZE);
    if (Serial.available()){
    Serial.readBytes((uint8_t*)data, SENT_SIZE);
    }
  }

  //если буфер порта пуст или количество элементов делятся нацело на 8 и вышли из условия, то
  if (ends) {
    ends = 0; //обнуляем флаг выхода из условия
    if (me) {
      //Serial.println();
      Serial.print("Me:");
      me = 0;
    }
    //выводим все отправленные байты в порт
    /*for (int i = 0; i < SENT_SIZE; i++ ) {
      Serial.print("dataS");
      Serial.print("[");
      Serial.print(i);
      Serial.print("]=");
      Serial.write(data[i]); //Serial.print(data[i]);
      Serial.println();
      Serial.flush();
      }
      //выводим число отправленных байт
      Serial.print("SENT_SIZE=");
      Serial.println(SENT_SIZE);*/

    sendMes(data, SENT_SIZE); //отправляем массив data размером SENT_SIZE
    Si446x_RX(CHANNEL); //включаем приём по каналу
  }
}

//функция отправки сообщения
void sendMes(byte *dataLocal, int sizeDataLocal) {
  //обнуляем флаг приёма пакета (не может быть принят раньше, чем отправлен)
  PKT_RXed = 0;
  //отправляем сообщение
  Si446x_TX(dataLocal, sizeDataLocal, CHANNEL, SI446X_STATE_RX);
  getResponse(1000);  //ожидаем пока не получили ответ (мс)

  //выводим отправленные данные в порт
  for (byte i = 0; i < sizeDataLocal; i++) {
    /*Serial.print("dataLocal[");
      Serial.print(i);
      Serial.print("]=");*/
    //Serial.println(dataLocal[i], DEC);
    Serial.write(dataLocal[i]);
    //Serial.println();
    Serial.flush();
    if (dataLocal[i] == 10) {
      me = 1;
      //Serial.println();
    }
  }
}

//функция ожидания ответного пакета
void getResponse(int delayPKT) {
  //находимся в цикле пока не получили ответ
  while (!PKT_RXed) {
    //если пакет был отправлен но не принят и тайм-аут истек, то
    if ((millis() - TXtime >= delayPKT) and !PKT_RXed and PKT_SNT) {
      //обнуляем флаг отправки пакета (чтобы не заходить в цикл в рамках 1 пакета)
      PKT_SNT = 0;

      //Serial.println("Таймаут ответного пакета");
      //Serial.print("Me:");
      //отправляем пакет повторно
      Si446x_TX(data, SENT_SIZE, CHANNEL, SI446X_STATE_RX);
      me = 0;
    }

    //если пакет был отправлен и успешно принят, то
    if (PKT_SNT and PKT_RXed) {
      //обнуляем флаг отправки пакета
      PKT_SNT = 0;
    }
  }
  //обнуляем флаг приёма пакета
  PKT_RXed = 0;
}

void info() {
  si446x_info_t info;
  Si446x_getInfo(&info);
  Serial.print("info.chipRev=");
  Serial.println(info.chipRev);

  Serial.print("info.part=");
  Serial.println(info.part);

  Serial.print("info.partBuild=");
  Serial.println(info.partBuild);

  Serial.print("info.id=");
  Serial.println(info.id);

  Serial.print("info.customer=");
  Serial.println(info.customer);

  Serial.print("info.romId=");
  Serial.println(info.romId);

  Serial.print("info.revExternal=");
  Serial.println(info.revExternal);

  Serial.print("info.revBranch=");
  Serial.println(info.revBranch);

  Serial.print("info.revInternal=");
  Serial.println(info.revInternal);

  Serial.print("info.patch=");
  Serial.println(info.patch);

  Serial.print("info.func=");
  Serial.println(info.func);

  temp = Si446x_adc_temperature();
  Serial.print("temp=");
  Serial.print(temp);
  Serial.println(" C");

  bat = Si446x_adc_battery();
  Serial.print("bat=");
  Serial.print(bat);
  Serial.println(" mV");

  RSsi = Si446x_getRSSI();
  Serial.print("rssi=");
  Serial.print(RSsi);
  Serial.println(" dBm");
}
