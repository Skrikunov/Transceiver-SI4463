#include <Si446x.h>
#define CHANNEL 0 //канал связи
#define MAX_PACKET_SIZE 20 //максимальный размер пакета

byte power=1; //мощность передатчика
int countRX = 0;  //количество принятых пакетов
bool PKT_RXed = 0;  //флаг успешно принятого пакета
bool CRC_ERR = 0;  //флаг ошибки CRC
bool INVD_SYNC = 0; //флаг ошибки синхрослова
bool SYNC_DET = 0;  //флаг обнаружения синхрослова
bool PR_DET = 0;  //флаг обнаружения преамбулы
bool INVD_PRE = 0;  //флаг ошибки преамбулы
byte RX_SIZE; //размер принятого пакета
bool PKT_SNT = 0; //флаг успешно отправленного пакета

uint8_t data[] = {1, 2, 3, 4, 5, 6, 7, 8}; //массив данных

typedef struct {
  uint8_t ready;
  int16_t rssi;
  uint8_t length;
  uint8_t buffer[MAX_PACKET_SIZE];
} pingInfo_t;
static volatile pingInfo_t pingInfo;

//индикатор правильной преамбулы
void SI446X_CB_PREAMBLE(void) {
  PR_DET = 1;
  Serial.println("PR_DET");
}

//индикатор НЕправильной преамбулы
void SI446X_CB_RXINVALIDPREAMBLE(void) {
  //INVD_PRE = 1;
  Serial.println("INVD_PRE");
}

//индикатор правильного синхрослова
void SI446X_CB_RXBEGIN(int16_t rssi)
{
  rssi = Si446x_getRSSI();  //значение уровня сигнала в момент обнаружения синхрослова
  Serial.println("SYNC_DET");
  Serial.print("RSSI=");
  Serial.print(rssi);
  Serial.println(" dBm");
  SYNC_DET = 1; //считаем синхрослова обнаруженным
}

//индикатор НЕправильного синхрослова
void SI446X_CB_RXINVALIDSYNC(void) {
  INVD_SYNC = 1;
  Serial.println("INVD_SYNC");
}

//колбек принятого и правильного пакета
//вкл всегда
void SI446X_CB_RXCOMPLETE(uint8_t length, int16_t rssi)
{
  PKT_RXed = 1; //считаем пакет принятым
  countRX++;  //увеличиваем на 1 количество принятых пакетов
  RX_SIZE = length;
  Serial.println("PKT_RXed");
}

//идникатор ошибки CRC
void SI446X_CB_RXINVALID(int16_t rssi)
{
  CRC_ERR = 1;
  Serial.println("CRC_ERR");
  Si446x_RX(CHANNEL);
}

//колбек конца отправки пакета
//вкл через Si446x_setupCallback
void SI446X_CB_SENT(void)
{
  PKT_SNT = 1;  //считаем пакет отправленным
  Serial.println("PKT_SNT");
  Serial.println();
}

void setup()
{
  Serial.begin(115200); //последовательный порт
  Si446x_init();  //инициализация Si446x
  Si446x_setTxPower(power); //мощность передатчика

  //разрешаем колбек приема пакета
  Si446x_setupCallback(SI446X_CBS_SENT, 1);
  Si446x_setupCallback(SI446X_CBS_RXBEGIN, 1);
  Si446x_RX(CHANNEL);
  //PKT_SNT = 1;
}

void loop()
{
  //если пакет принят, то
  if (PKT_RXed) {
    PKT_RXed = 0;
    //вывести количество принятых пакетов
    Serial.print("countRX=");
    Serial.println(countRX);
    
    //записать данные в массив из FIFO буфера
    Si446x_read((uint8_t*)pingInfo.buffer, RX_SIZE);
    
    //если разммер принятого пакета больше максимального, то
    if (RX_SIZE > MAX_PACKET_SIZE) {
      RX_SIZE = MAX_PACKET_SIZE;  //ограничиваемся максимальным размером
    }
    //отправляем ответный пакет
    Si446x_TX(data, sizeof(data), CHANNEL, SI446X_STATE_RX);
  }
}
