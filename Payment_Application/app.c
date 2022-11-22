#include <stdio.h>
#include <assert.h>
#include "app.h"


void appStart(void) {
	ST_transaction_t transactionData;
	ST_cardData_t* cardData = &transactionData.cardHolderData;
	ST_terminalData_t* termData = &transactionData.terminalData;
	ST_accountsDB_t* acc = NULL;

	if (getCardHolderName(cardData)) {
		printf("WRONG_NAME\n");
		return;
	}

	if (getCardExpiryDate(cardData)) {
		printf("WRONG_EXP_DATE\n");
		return;
	}

	if (getCardPAN(cardData)) {
		printf("WRONG_PAN\n");
		return;
	}

	if (getTransactionDate(termData)) {
		printf("WRONG_DATE\n");
		return;
	}

	if (isCardExpired(cardData, termData)) {
		printf("EXPIRED_CARD\n");
		return;
	}

	if (getTransactionAmount(termData)) {
		printf("INVALID_AMOUNT\n");
		return;
	}

	if (setMaxAmount(termData, 3000.0f)) {
		printf("INVALID_MAX_AMOUNT\n");
		return;
	}

	if (isBelowMaxAmount(termData)) {
		printf("EXCEED_MAX_AMOUNT\n");
		return;
	}

	switch (recieveTransactionData(&transactionData, &acc)) {
	case DECLINED_STOLEN_CARD:
		printf("\nDECLINED_STOLEN_CARD\n");
		break;

	case DECLINED_INSUFFECIENT_FUND:
		printf("\nDECLINED_INSUFFECIENT_FUND\n");
		break;

	case ACCOUNT_NOT_FOUND:
		printf("\nACCOUNT_NOT_FOUND\n");
		break;

	case INTERNAL_SERVER_ERROR:
		printf("\nINTERNAL_SERVER_ERROR\n");
		break;

	case APPROVED:
		printf("\nTnransaction is APPROVED\n");
		printf("\nBalance After Transaction is approved: %f\n\n", acc->balance);
		break;
	}
}

int main(void) {
	appStart();
	return 0;
}
