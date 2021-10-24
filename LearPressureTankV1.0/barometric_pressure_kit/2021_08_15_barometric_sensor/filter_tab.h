float EMA_function(float alpha, float latest, float stored) {
  return (alpha * latest) + ((1 - alpha) * stored);
}

float DEMA_function(float alpha, float sensor_value, float *_ema, float *_ema_ema) {
  *_ema = EMA_function(alpha, sensor_value, *_ema);
  *_ema_ema = EMA_function(alpha, *_ema, *_ema_ema);
  float DEMA = 2 * (*_ema) - (*_ema_ema);
  return DEMA;
}

void filter_setup_p_h(float _val1, float _val2) {
  //  ema_p = _val1; ema_ema_p = _val1;
  //  ema_h = _val2; ema_ema_h = _val2;

}

void filter_setup_alt(float *_data1_params, float _data) {
  _data1_params[0] = _data;
  _data1_params[1] = _data;
}

float filter_data(float *_data1_params, float _new_data) { // params = ema, dema, alpha
  float filtered_val = DEMA_function(_data1_params[2] , _new_data, &_data1_params[0], &_data1_params[1]);  // alpha, data, ema, dema
  return filtered_val;
}
