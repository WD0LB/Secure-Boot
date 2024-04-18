

/* Configuration and Constants */
#define PUBLIC_KEY "pubkey"
#define SIGNATURE_ADDRESS (uint32_t)0x080FFFF0  // Example address
#define FIRMWARE_START_ADDRESS APP_ADDRESS
#define FIRMWARE_SIZE (END_ADDRESS - APP_ADDRESS + 1)

/* Function Declarations */
static uint8_t verify_signature(void);
typedef void (*pFunction)(void);
typedef enum bl_state_t{
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
}BLState;