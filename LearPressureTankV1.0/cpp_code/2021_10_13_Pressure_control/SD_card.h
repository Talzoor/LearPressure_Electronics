#include <SPI.h>
#include <SD.h>

const int chipSelect = 53;

void dumpFile(char* _file) {
  Serial.println("\nOpening log file:" + (String)_file + (String)"\n----------------" );
  if (strcmp(_file, "")) {
    File dataFile = SD.open(_file);
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
      Serial.println("---------------- end" );
    } else {    // if the file isn't open, pop up an error:
      Serial.println("error opening " + (String)_file);
    }
  }
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    if (strcmp(entry.name(), "SPOTLI~1") and \
        strcmp(entry.name(), "TRASHE~1") and \
        strcmp(entry.name(), "FSEVEN~1")
       ) {
      Serial.print(entry.name());
      if (entry.isDirectory()) {
        Serial.println("/");
        printDirectory(entry, numTabs + 1);
      } else {
        // files have sizes, directories do not
        Serial.print("\t\t");
        Serial.println(entry.size(), DEC);
      }
    }
    entry.close();
  }
}

bool sd_card_setup() {
  bool ret_val = false;
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  } else {
    Serial.println("card initialized.");
    ret_val = true;
  }

  return ret_val;
}

unsigned int sd_file_no_func() {
  unsigned int tmp_val = 0;
  if (SD.exists(SD_NUMBERING_FILE)) {
    File dataFile = SD.open(SD_NUMBERING_FILE, FILE_WRITE);
    dataFile.seek(0);
    String line_str = dataFile.readStringUntil('\n');
    //Serial.println("line_str:" + (String)line_str);

    int line = line_str.toInt();
    tmp_val = line + 1;
    //Serial.println("line:" + (String)tmp_val);
    sd_write_num_file(tmp_val);
  } else {
    sd_write_num_file(0);
  }
  return tmp_val;
}

void sd_write_num_file(unsigned int _file_no, bool _del) {
  if (_del) SD.remove(SD_NUMBERING_FILE); // remove numbering file
  File dataFile = SD.open(SD_NUMBERING_FILE, FILE_WRITE); // and write it again
  if (dataFile) {
    dataFile.println((String)_file_no);
    dataFile.println("Pressure control system");
    dataFile.println("Log numbering file - DO NOT DELETE");
    dataFile.close();
  } else {     // if the file isn't open, pop up an error:
    Serial.println("error opening " + (String)SD_NUMBERING_FILE);
  }
} // sd_write_num_file

void sd_append_to_file(char* _file, char* _msg) {
  File dataFile = SD.open(_file, FILE_WRITE);
  if (dataFile) {
    dataFile.print(_msg);
    dataFile.close();
    //Serial.println("file closed");
  } else {     // if the file isn't open, pop up an error:
    Serial.println("error opening " + (String)_file);
  }
}

void sd_append_to_csv(char* _file, char* _time, float _pressure) {
  //Serial.println("opening:#" + (String)_file + "#");
  File dataFile = SD.open(_file, FILE_WRITE);
  if (dataFile) {
    char msg[100]; char bf1[10];
    dtostrf(_pressure, 2, 2, bf1);
    sprintf(msg, "%s\t\t\t,%s\n", _time, bf1);
    dataFile.print(msg);
    dataFile.close();
    //Serial.println("file closed");
  } else {     // if the file isn't open, pop up an error:
    Serial.println("error opening csv:" + (String)_file);
  }
}
