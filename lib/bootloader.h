#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

/* Configuration and Constants */
#define PUBLIC_KEY "pubkey"
#define SIGNATURE_ADDRESS (uint32_t)0x080FFFF0  // Example address
#define APP_ADDRESS (uint32_t)0x08008000
#define END_ADDRESS (uint32_t)0x080FFFFB
#define FIRMWARE_SIZE (END_ADDRESS - APP_ADDRESS + 1)



/* Function Declarations */
static uint8_t verify_signature(void);
void Bootloader_HandleError(BLState error_code);

typedef void (*pFunction)(void);
typedef enum bl_state_t{
    BL_OK = 0,
    BL_Sync,
    BL_WaitingForUpdateReq,
    BL_FWLengthReq,
    BL_FWLengthRes,
    BL_ReceiveFW,
    BL_CheckSignature,
    BL_UpdateFW,
    BL_UpdateTimeout,
    BL_UpdateSuccess,
    BL_UpdateFailure,
    BL_SwitchToApplication,
    BL_VerifyApplicationSuccess,
    BL_VerifyApplicationFail,
    BL_ERR_FLASH_WRITE,
    BL_ERR_HW_FAILURE
}BLState;

#endif