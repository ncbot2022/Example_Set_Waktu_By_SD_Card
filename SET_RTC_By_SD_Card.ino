#include <Wire.h>
#include <SD.h>
#include <EEPROM.h>


// macro key debug
#define serial_mon true
#if serial_mon
  #define  debug(x) Serial.print(x)
  #define  debugln(x) Serial.println(x)
  #define debug_begin(x) Serial.begin(x)
#else
  #define print(x) 
  #define println(x) 
  #define debug_begin(x) 
#endif


const int chipSelect = 53;
File myFile;

unsigned int jam, menit, detik;
unsigned int tanggal, bulan, tahun;

void readConfigFile(const char * path) {
  File file = SD.open(path);
  if (!file) {
    Serial.println("Failed to open config file!");
    return;
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    line.trim(); 
    if (line.length() == 0 || line.indexOf('=') == -1) continue;
    int sep = line.indexOf('=');
    String key = line.substring(0, sep);
    String value = line.substring(sep + 1);
    key.trim();
    value.trim();

    if (key.equalsIgnoreCase("JAM")) {
      jam = value.toInt();
    } else if (key.equalsIgnoreCase("MENIT")) {
      menit = value.toInt();
    } else if (key.equalsIgnoreCase("DETIK")) {
      detik = value.toInt();
    } else if (key.equalsIgnoreCase("TANGGAL")) {
      tanggal = value.toInt();
    } else if (key.equalsIgnoreCase("BULAN")) {
      bulan = value.toInt();
    }else if (key.equalsIgnoreCase("TAHUN")) {
      tahun = value.toInt();
    }
  }
  
  debug("Set JAM     : ");debugln(jam); 
  debug("Set MENIT   : ");debugln(menit); 
  debug("Set DETIK   : ");debugln(detik); 

  debug("Set TANGGAL : ");debugln(tanggal); 
  debug("Set BULAN   : ");debugln(bulan); 
  debug("Set TAHUN   : ");debugln(tahun); 
  file.close();

  byte alamat = 0;
  byte storedTanggal = EEPROM.read(alamat);
  byte storedBulan = EEPROM.read(alamat + 1);
  int storedTahun = EEPROM.read(alamat + 2) * 256 + EEPROM.read(alamat + 3);
  byte storedJam = EEPROM.read(alamat + 4);
  byte storedMenit = EEPROM.read(alamat + 5);
  byte storedDetik = EEPROM.read(alamat + 6);

  if (tanggal != storedTanggal || bulan != storedBulan || tahun != storedTahun || 
      jam != storedJam || menit != storedMenit || detik != storedDetik) {

    EEPROM.write(alamat, tanggal);
    EEPROM.write(alamat + 1, bulan);
    EEPROM.write(alamat + 2, (tahun >> 8) & 0xFF);  
    EEPROM.write(alamat + 3, tahun & 0xFF);         
    EEPROM.write(alamat + 4, jam);
    EEPROM.write(alamat + 5, menit);
    EEPROM.write(alamat + 6, detik);
    debugln("CONFIG BERUBAH...., SET WAKTU RTC SEKARANG JUKA!!!");
    delay(1000);
  } else {
     debugln("CONFIG TIDAK BERUBAH...., LANJUTKAN WAKTU RTC SEKARANG JUKA!!!");
  }
}


void setup(){
  debug_begin(9600);

  if (!SD.begin(chipSelect)) {
      while (true){
        if (SD.begin(chipSelect)) {  // Sesuaikan nomor pin CS sesuai dengan wiring Anda     
          break;  // Keluar dari fungsi agar tidak mencoba menulis ke SD
        }
      }
  }
  Serial.println("SD card berhasil terdeteksi");
  readConfigFile("CONFIG/Config.txt");
}

void loop(){

}

