#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "pn532.h"
#include "pn532_rpi.h"

int main(int argc, char** argv) {
    uint8_t buff[255];
    uint8_t uid[MIFARE_UID_MAX_LENGTH];
    int32_t uid_len = 0;
    printf("This's bac's app\n");
    PN532 pn532;
    PN532_I2C_Init(&pn532);
    if (PN532_GetFirmwareVersion(&pn532, buff) == PN532_STATUS_OK) {
        printf("Found PN532 with firmware version: %d.%d\r\n", buff[1], buff[2]);
    } else {
        return -1;
    }
    PN532_SamConfiguration(&pn532);
    printf("Waiting for RFID/NFC card...\r\n");
    while(1){
        // Check if a card is available to read
        uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
        if (uid_len == PN532_STATUS_ERROR) {
            printf(".");
            fflush(stdout);
        } else {
            printf("Found card with UID: ");
            for (uint8_t i = 0; i < uid_len; i++) {
                printf("%02x ", uid[i]);
            }
            printf("\r\n");
            break;
        }
    }
    printf("Reading blocks...\r\n");
    for (uint8_t block_number = 0; block_number < 64; block_number++) {
        pn532_error = PN532_MifareClassicAuthenticateBlock(&pn532, uid, uid_len,
                block_number, MIFARE_CMD_AUTH_A, key_a);
        if (pn532_error != PN532_ERROR_NONE) {
            break;
        }
        pn532_error = PN532_MifareClassicReadBlock(&pn532, buff, block_number);
        if (pn532_error != PN532_ERROR_NONE) {
            break;
        }
        printf("%d: ", block_number);
        for (uint8_t i = 0; i < 16; i++) {
            printf("%02x ", buff[i]);
        }
        printf("\r\n");
    }
    if (pn532_error) {
        printf("Error: 0x%02x\r\n", pn532_error);
    }
}