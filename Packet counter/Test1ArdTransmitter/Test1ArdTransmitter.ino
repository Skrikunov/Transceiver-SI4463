#include <Si446x.h>
#define CHANNEL 0 //канал связи
#define MAX_PACKET_SIZE 8 //максимальный размер пакета

byte power=1; //мощность передатчика
int numpack = 500; //количество отправляемых пакетов
int countTX = 0;  //количество отправленных пакетов
int countRX = 0;  //количество принятых пакетов
int countLoose = 0; //количество утерянных пакетов
int senttime = 0; //время отправки пакета
int rxtime = 0; //время приёма пакета
bool PKT_RXed = 0;  //флаг успешно принятого пакета
bool PKT_SNT = 0; //флаг успешно отправленного пакета
bool SYNC_DET = 0;  //флаг обнаружения синхрослова

int rssi = 0; //уровень сигнала
int starttime = 0;  //момент начала выполнение программы
uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8}; //массив данных

byte SENT_SIZE;  //размер отправляемого пакета

//колбек конца отправки пакета
//вкл через Si446x_setupCallback
void SI446X_CB_SENT(void)
{
  senttime = millis();  //записываем время отправки
  PKT_SNT = 1;  //считаем пакет отправленным
  countTX++;  //увеличиваем на 1 количество отправленных пакетов
}

//колбек принятого и правильного пакета
//вкл всегда
void SI446X_CB_RXCOMPLETE(uint8_t length, int16_t rssi)
{
  rxtime = millis();  //записываем время приёма
  PKT_RXed = 1; //считаем пакет принятым
  countRX++;  //увеличиваем на 1 количество принятых пакетов
  //RX_SIZE = length;
}

//индикатор правильного синхрослова
void SI446X_CB_RXBEGIN(int16_t rssi)
{
  rssi = Si446x_getRSSI();  //значение уровня сигнала в момент обнаружения синхрослова
  Serial.print("RSSI=");
  Serial.print(rssi);
  Serial.println(" dBm");
  SYNC_DET = 1; //считаем синхрослова обнаруженным
}

void setup()
{
  Serial.begin(115200); //последовательный порт
  Si446x_init();  //инициализация Si446x
  Si446x_setTxPower(power); //мощность передатчика

  //разрешаем колбек конца отправки пакета
  Si446x_setupCallback(SI446X_CBS_SENT, 1);

  //если разммер отправляемого пакета больше максимального, то
  if (sizeof(data) > MAX_PACKET_SIZE) {
    SENT_SIZE = MAX_PACKET_SIZE;  //ограничиваемся максимальным
    Serial.println("Длина пакета больше максимальной, пакет обрезан");
  }
  else {
    SENT_SIZE = sizeof(data);
  }
  starttime = millis();  //начало выполнения программы
  PKT_RXed = 1;
  PKT_SNT = 1;
}

void loop()
{
  if ((millis() - senttime >= 250) and !PKT_RXed and PKT_SNT) {
    PKT_RXed = 1;
    Serial.println("Таймаут ответного пакета");
    countLoose++; //увеличиваем количество утерянных пакетов
  }

  //если количество пакетов не достигло максимального, пакет был отправлен и принят, то
  if (countTX < numpack and PKT_SNT and PKT_RXed) {
    PKT_SNT = 0;  //обнуляем флаг отправленного пакета
    PKT_RXed = 0; //обнуляем флаг принятого пакета
    //отправляем очередной пакет
    Si446x_TX(data, SENT_SIZE, CHANNEL, SI446X_STATE_RX);

    //выводим текущее число отправленных пакетов
    Serial.print("countTX=");
    Serial.println(countTX+1);
    //выводим временную задержку
    Serial.print("Задержка=");
    Serial.println(rxtime - senttime);
    Serial.println();

    //если количество отправленных пакетов достигло максимального значения, то
    //выводим итоговую информацию
    if (countTX == numpack - 1) {
      Serial.print("TransmitPackets=");
      Serial.println(countTX + 1);
      Serial.print("RecievePackets=");
      Serial.println(countRX + 1);
      Serial.print("LoosePackets=");
      Serial.println(countLoose);
      Serial.print("Totaltime=");
      Serial.println(millis() - starttime);
    }
  }
}
