using namespace ace_button;
AceButton button_up(BUTTON[0]);
AceButton button_down(BUTTON[1]);
AceButton button_left(BUTTON[2]);
AceButton button_enc(BUTTON[3]);

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState);

void button_setup() {
  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
}

void button_check() {
  button_up.check();
  button_down.check();
  button_left.check();
  button_enc.check();
}

bool acceptEvent(uint8_t _event) {
  bool return_val = false;
  if (_event == 0) return_val = true;
  if (_event == 4) return_val = true;
  if (_event == 5) return_val = true;
  return return_val;
}

void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {

  // Print out a message for all events, for both buttons.
  Serial.print(F("handleEvent(): pin: "));
  int button_pin = button->getPin();
  Serial.print(button_pin);
  Serial.print(F("; eventType: "));
  Serial.print(eventType);
  Serial.print(F("; buttonState: "));
  Serial.println(buttonState);

  if (acceptEvent(eventType)) {
    //    Serial.print(F("OK"));
    if (button_click == NO_CLICK) {
      button_click = button_pin;
      button_event = eventType;
    }
  }

  switch (eventType) {
    case AceButton::kEventPressed:
      if (button->getPin() == BUTTON[0]) { //up
        Serial.println(F("Button up clicked!"));
      }
      if (button->getPin() == BUTTON[1]) { //up
        Serial.println(F("Button down clicked!"));
      }
      if (button->getPin() == BUTTON[2]) { //up
        Serial.println(F("Button left clicked!"));
      }
      break;
    case AceButton::kEventReleased:
      if (button->getPin() == BUTTON[0]) { //down
        //        Serial.println(F("Button!"));
      }
      break;
    case AceButton::kEventClicked:
      if (button->getPin() == BUTTON[0]) { //left
        //        Serial.println(F("Button!"));
      }
      break;
  }
}
