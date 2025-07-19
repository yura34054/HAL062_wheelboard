#include "can.h"
#include "stm32f4xx_hal_can.h"
#include "leds/leds.h"

#include "motor/timers.h"
#include "motor/motor.h"

extern CAN_HandleTypeDef hcan1;
static CAN_HandleTypeDef *can_handle = &hcan1;

static CAN_TxHeaderTypeDef *can_txHeader;
static CAN_RxHeaderTypeDef *can_rxHeader;
static uint32_t *can_txMailbox;
static Command rxCommand = { 0 }; 

static void CAN_recivedCallback(CAN_HandleTypeDef *hcan);
static void CAN_errorCallback(CAN_HandleTypeDef *hcan);

void CAN_transmit(Command *command) {
	
	if (HAL_CAN_GetTxMailboxesFreeLevel(can_handle) == 0) return;

	can_txHeader->StdId = command->ID;

	HAL_CAN_AddTxMessage(
			can_handle, 
			can_txHeader, 
			command->payload, 
			can_txMailbox	
			);
}

static void CAN_recivedCallback(CAN_HandleTypeDef *hcan) {
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, can_rxHeader, rxCommand.payload);

	switch (can_rxHeader->StdId) { // TODO default case
	case 0 ... 127: // panic IDs on drive CANs
		// TODO panic(Wheelboard, ERROR_external_panic);
		break;

	case PARENT_ID:

		message_recived = 1;

		target_speed = SIDE_MULT * ((int16_t) ((float) rxCommand.payload[0]));
		break;

	default:
		break;
	}
	LED_TOGGLE(LED_3);
}

static void CAN_errorCallback(CAN_HandleTypeDef *hcan) {(void) hcan;}


void CAN_init(void) {
	// setup TX header
	can_txHeader->ExtId = 0;
	can_txHeader->IDE = CAN_ID_STD;
	can_txHeader->RTR = CAN_RTR_DATA;
	can_txHeader->DLC = 8;

	// setup filter 
	CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = 0u;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
	sFilterConfig.FilterIdHigh = (0x14 << 5); // Pierwszy filtr na ID 0x14 (przesunięcie o 5 bitów)
	sFilterConfig.FilterIdLow = (0x16 << 5); // Drugi filtr na ID 0x15 (przesunięcie o 5 bitów)
	sFilterConfig.FilterMaskIdHigh = (0xFE << 5); // Maska dla obu filtrów (ignoruje ostatni bit)
	sFilterConfig.FilterMaskIdLow = (0xFE << 5);
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 0;
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);

	HAL_CAN_RegisterCallback(can_handle, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, CAN_recivedCallback);
	HAL_CAN_RegisterCallback(can_handle, HAL_CAN_ERROR_CB_ID, CAN_errorCallback);

	HAL_CAN_ActivateNotification(
			can_handle, 
			CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR 
			);

	if ((can_handle->State != HAL_CAN_STATE_READY) && 
		(can_handle->State != HAL_CAN_STATE_LISTENING)) {
		// TODO errorHandler();	
	}
}

