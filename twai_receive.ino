#include "driver/gpio.h"
#include "driver/twai.h"

#define ACCEPT_ID                  0x036   //11 bit standard format ID;if not define any ID,accept all IDs.

#ifdef ACCEPT_ID
#define MASK_ID                  ~(0x7FF << 21)   //32 bit standard format ID,but only care for the frist 11 bits.
#define TWAI_FILTER_CONFIG() {.acceptance_code = (ACCEPT_ID << 21),.acceptance_mask = MASK_ID,.single_filter = true};
#endif


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  twai_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  twai_receive();
}



void twai_init()
{
  //Initialize configuration structures using macro initializers
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_4, GPIO_NUM_5, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
#ifdef ACCEPT_ID
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG();
#else
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
#endif

  //Install TWAI driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("Driver installed");
  } else {
    Serial.println("Failed to install driver");
    return;
  }

  //Start TWAI driver
  if (twai_start() == ESP_OK) {
    Serial.println("Driver started");
  } else {
    Serial.println("Failed to start driver");
    return;
  }
}

void twai_receive()
{
  //Wait for message to be received
  twai_message_t message;
  if (twai_receive(&message, pdMS_TO_TICKS(10000)) == ESP_OK) {
    //    Serial.println("Message received");
    Serial.println("ID DATA----------->");
    Serial.print(message.identifier, HEX);
    if (!(message.rtr))
    {
      for (int i = 0; i < message.data_length_code; i++)
      {
        Serial.print(" ");
        Serial.print(message.data[i], HEX);
      }
    }
    Serial.println(" ");
  } else {
    Serial.println("Failed to receive message");
    return;
  }

  //Process received message
  //  if (message.extd)
  //  {
  //    Serial.println("Message is in Extended Format");
  //  }
  //  else
  //  {
  //    Serial.println("Message is in Standard Format");
  //  }
}
