void EEPROM_setup() {
  const int maxAllowedWrites = 50;
  const int memBase          = 120;
  EEPROM.setMemPool(memBase, EEPROMSizeUno);
  EEPROM.setMaxAllowedWrites(maxAllowedWrites);
}

void WriteVar_byte(EEPROMVar<uint8_t> &_Var) {
  _Var.update();     // store EEPROMVar to EEPROM
}

void ReadVar_byte(EEPROMVar<uint8_t> &_Var) {
  _Var.restore();  // restore EEPROMVar to EEPROM
}

void WriteVar_int(EEPROMVar<uint16_t> &_Var) {
  _Var.update();       // store EEPROMVar to EEPROM
}

void ReadVar_int(EEPROMVar<uint16_t> &_Var) {
  _Var.restore();  // restore EEPROMVar to EEPROM
}
