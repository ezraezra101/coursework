#include <RH_ASK.h>

void listen(void (*callback)(packet)) {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      packet p = {
        buf[0],
        buf[1]
      };
      callback(p);
      
        // Message with a good checksum received, dump it.
//        driver.printBuffer("Got:", buf, buflen);
//        buf[buflen] = '\0';
//        Serial.println((char*)buf);
    }
}

void transmit(packet p) {
  driver.send((uint8_t *)&p, sizeof(packet));
  //driver.waitPacketSent();
}

