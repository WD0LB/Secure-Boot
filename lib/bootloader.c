#include <stdint.h>
#include <stdio.h>

#include "bootloader.h"
#include "crypto.h" // Assume this header provides cryptographic functionalities



/* Bootloader Main Code */
int main(void)
{
    uint8_t status;

    // Initialize system peripherals necessary for the bootloader
    Bootloader_Init();

    // Verify the firmware's signature
    status = verify_signature();
    if(status != BL_VerifyApplicationSuccess)
    {
        // Handle the error, perhaps by entering a safe state or attempting recovery
        Bootloader_HandleError();
    }

    // If the signature is valid, proceed to application
    Bootloader_JumpToApplication();

    // This point should never be reached
    while(1);
}

/* Verify the digital signature of the firmware */
static uint8_t verify_signature(void)
{
    uint8_t result = NULL;
    uint8_t *signature = (uint8_t*)SIGNATURE_ADDRESS;  // Pointer to the signature in flash
    uint8_t *firmware_data = (uint8_t*)FIRMWARE_START_ADDRESS;

    // Assuming 'Crypto_VerifySignature' is a function that verifies the signature of given data
    // This function needs to be implemented as part of your cryptographic operations
    if(!Crypto_VerifySignature(firmware_data, FIRMWARE_SIZE, signature, PUBLIC_KEY))
    {
        result = BL_VerifyApplicationFail;  // Error code for signature verification failure
    }

    return result;
}

/* Jump to application */
void Bootloader_JumpToApplication(void)
{
    // Set main stack pointer
    __set_MSP(*(volatile uint32_t*) FIRMWARE_START_ADDRESS);

    // Get the reset handler address from the application's vector table
    uint32_t app_reset_handler_address = *(volatile uint32_t*)(FIRMWARE_START_ADDRESS + 4);
    pFunction app_reset_handler = (pFunction) app_reset_handler_address;

    // Deinitialize all peripherals to a safe state before jumping to the application
    Bootloader_DeInit();

    // Jump to application
    app_reset_handler();
}
