#include <stdint.h>
#include <stdio.h>

#include "bootloader.h"
#include "crypto.h" // Acryptographic functionalities



/* Bootloader Main Code */
int main(void)
{
    uint8_t status;

    // Initialize system peripherals necessary for the bootloader ot work properly
    Bootloader_Init();

    // Verify the firmware's signature
    status = verify_signature();
    if(status != BL_VerifyApplicationSuccess)
    {
        // Handle the error, perhaps by entering a safe state or attempting recovery
        Bootloader_HandleError(status);
    }

    // If the signature is valid, proceed to application
    Bootloader_JumpToApplication();

    // This point should never be reached
    while(1);
}

// Initialse the bootloader
uint8_t Bootloader_Init(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_FLASH_CLK_ENABLE();

    /* Clear flash flags */
    HAL_FLASH_Unlock();
    // __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);
    HAL_FLASH_Lock();

    return BL_OK;
}

/* Verify the digital signature of the firmware */
static uint8_t verify_signature(void)
{
    uint8_t result = NULL;
    uint8_t *signature = (uint8_t*)SIGNATURE_ADDRESS;  // Pointer to the signature in flash
    uint8_t *firmware_data = (uint8_t*)APP_ADDRESS;

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
    __set_MSP(*(volatile uint32_t*) APP_ADDRESS);

    // Get the reset handler address from the application's vector table
    uint32_t app_reset_handler_address = *(volatile uint32_t*)(APP_ADDRESS + 4);
    pFunction app_reset_handler = (pFunction) app_reset_handler_address;

    // Deinitialize all peripherals to a safe state before jumping to the application
    Bootloader_DeInit();

    // Jump to application
    app_reset_handler();
}

// Error logging
void log_error(BLState error_code) {
    // Show the bootloader error
    printf("Bootloader error: %d\n", error_code);
}


// error handling function
void Bootloader_HandleError(BLState error_code) {
    log_error(error_code);

    switch (error_code) {
        case BL_ERR_FLASH_WRITE:
        case BL_VerifyApplicationFail:
            attempt_recovery();
            break;
        case BL_UpdateFailure:
            revert_to_last_good_firmware();
            break;
        case BL_ERR_HW_FAILURE:
            enter_safe_mode();
            break;
        default:
            reset_device();
            break;
    }    
}