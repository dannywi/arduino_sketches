// MCP2515 CAN Send and Receive Test
// based on https://qiita.com/covao/items/d30fa5e36470bbee3be7

#include <SPI.h>
#include <mcp_can.h>  //https://github.com/coryjfowler/MCP_CAN_lib

unsigned long rxId;
byte len;
byte rxBuf[8];

uint32_t txId0 = 0x100;
byte txBuf0[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

uint32_t txId1 = 0x673;
byte txBuf1[] = {0xff, 0xfe, 0x04, 0x31, 0x80, 0x12, 0xf9, 0xf8};

long prev_time = 0;

MCP_CAN CAN_TX(10);  // CAN_TX CS: pin 10
MCP_CAN CAN_RX(9);   // CAN_RX CS: pin 9

void setup() {
  Serial.begin(9600);

  auto init_fn = [](MCP_CAN& can, const String& desc) {
    // CAN_1000KBPS -> MCP_20MHZ
    // CAN_500KBPS -> >= MCP_16MHZ
    // <= CAN_250KBPS -> MCP_8MHZ
    if (can.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) {
      Serial.println(desc + ": Init OK!");
      can.setMode(MCP_NORMAL);
    } else {
      Serial.println(desc + ": Init Fail!");
    }
  };

  init_fn(CAN_TX, "CAN TX");
  init_fn(CAN_RX, "CAN RX");
}

void increment(byte& val, byte max) { val == max ? val = 0 : ++val; }

void loop() {
  if (millis() - prev_time > 3000) {
    CAN_TX.sendMsgBuf(txId0, 0, 8, txBuf0);
    CAN_TX.sendMsgBuf(txId1, 0, 8, txBuf1);
    Serial.print("Send ID: 0x");
    Serial.print(txId0, HEX);
    Serial.print(", 0x");
    Serial.println(txId1, HEX);

    increment(txBuf0[6], 0x0F);
    increment(txBuf1[7], 0xFF);
    prev_time = millis();
  }

  if (CAN_RX.checkReceive() == CAN_MSGAVAIL) {
    CAN_RX.readMsgBuf(&rxId, &len, rxBuf);
    Serial.print("Receive ID: 0x");
    Serial.print(rxId, HEX);
    Serial.print(" Data:");
    for (byte i = 0; i < len; i++) {
      Serial.print(" 0x");
      Serial.print(rxBuf[i], HEX);
    }
    Serial.println();
  }
}
