
void s_print_f(String _str1, String _val, String _str2){
  Serial.print(_str1 + _val + _str2);
}

void s_print_f(String _str1, float _float_val, String _str2){
  char buffer[10];
  dtostrf(_float_val, 4, 2, buffer);
  s_print_f(_str1, String(buffer), _str2);
}

void s_print_f(String _str1, unsigned long _ulong_val, String _str2){
  s_print_f(_str1, String(_ulong_val), _str2);
}

void s_print_f(String _str1, int _int_val, String _str2){
  s_print_f(_str1, String(_int_val), _str2);
}
