#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./server.h"
unsigned char test_mode3 = 0;

ST_accountsDB_t accountsDB[255] = { {2000.0f, RUNNING, "8989374615436851"} , {100000.0f, BLOCKED, "5807007076043875"} ,
									{3000.0f, RUNNING, "9127465236546542"} , {200000.0f, RUNNING, "1526327951062308"} ,
									{4000.0f, RUNNING, "8512546517453232"}
								  };

ST_transaction_t transactionDB[255] = { 0 }; int trans_sequence = 0;
//-----------------------------------------------------------------------
EN_transState_t recieveTransactionData(ST_transaction_t* transData, ST_accountsDB_t** accountRefrence) {
	if (isValidAccount(&transData->cardHolderData, accountRefrence) == ACCOUNT_NOT_FOUND) return FRAUD_CARD;
	if (isAmountAvailable(&transData->terminalData, accountRefrence) == LOW_BALANCE) return DECLINED_INSUFFECIENT_FUND;
	if (isBlockedAccount(accountRefrence) == BLOCKED_ACCOUNT) return DECLINED_STOLEN_CARD;
	if (saveTransaction(transData) == SAVING_FAILED) return INTERNAL_SERVER_ERROR;

	(*accountRefrence)->balance -= transData->terminalData.transAmount;
	return APPROVED;
}

void recieveTransactionDataTest(void) {
	test_mode3 = 1;

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: recieveTransactionData\n\n");
	ST_accountsDB_t acc_ref1 = { 5000.0f, RUNNING, "6859214615451572" };

	ST_cardData_t c_data = { "Youssef", "6859214615451572", "05/25" };
	ST_terminalData_t t_data = { 10.8f, 20.3f, "04/11/2020"};
	ST_transaction_t trans_t = { c_data, t_data, APPROVED, trans_sequence };

	printf("Test Case 1:\nInput Data: trans_t, acc_ref1\nExpected Result: %d\nActual Result: %d\n\n", FRAUD_CARD, recieveTransactionData(&trans_t, &acc_ref1));
	
	strcpy(trans_t.cardHolderData.primaryAccountNumber, "5807007076043875");
	ST_accountsDB_t acc_ref2 = { 100000.0f, BLOCKED, "5807007076043875" }; // EXISTS IN DB BUT BLOCKED
	printf("Test Case 2:\nInput Data: trans_t, acc_ref2\nExpected Result: %d\nActual Result: %d\n\n", DECLINED_STOLEN_CARD, recieveTransactionData(&trans_t, &acc_ref2));
	
	strcpy(trans_t.cardHolderData.primaryAccountNumber, "1526327951062308");
	ST_accountsDB_t acc_ref3 = { 200000.0f, RUNNING, "1526327951062308" };
	printf("Test Case 3:\nInput Data: trans_t, acc_ref3\nExpected Result: %d\nActual Result: %d\n\n", APPROVED, recieveTransactionData(&trans_t, &acc_ref3));
	
	test_mode3 = 0;
}
//-----------------------------------------------------------------------
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t** accountRefrence) {
	for (int i = 0; i < 5; i++) {
		if (strcmp((char*)cardData->primaryAccountNumber, (char*)accountsDB[i].primaryAccountNumber) == 0) {
			*accountRefrence = &accountsDB[i];
			return SERVER_OK;
		}
	}
	*accountRefrence = NULL;
	return ACCOUNT_NOT_FOUND;
}

void isValidAccountTest(void) {
	test_mode3 = 1;

	ST_cardData_t test1 = { "any", "9127465236546542", "any" }; // VALID PAN EXISTS IN DB
	ST_accountsDB_t acc = {15.4f, RUNNING, "9127465236546542"};

	ST_accountsDB_t* p = &acc;

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: isValidAccount\n\n");
	printf("Test Case 1:\nInput Data: 9127465236546542\nExpected Result: %d\nActual Result: %d\n\n", SERVER_OK, isValidAccount(&test1, &p));

	strcpy(test1.primaryAccountNumber, "7127265816546560"); // NOT VALID PAN, DOESN'T EXIST IN DB
	printf("Test Case 2:\nInput Data: 7127265816546560\nExpected Result: %d\nActual Result: %d\n\n", ACCOUNT_NOT_FOUND, isValidAccount(&test1, &p));

	test_mode3 = 0;
}
//-----------------------------------------------------------------------
EN_serverError_t isBlockedAccount(ST_accountsDB_t** accountRefrence) {
	return ((*accountRefrence)->state == BLOCKED ? BLOCKED_ACCOUNT : SERVER_OK);
}

void isBlockedAccountTest(void) {
	test_mode3 = 1;

	ST_accountsDB_t acc = { 15.4f, RUNNING, "9127465236546542" };

	ST_accountsDB_t* p = &acc;

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: isBlockedAccount\n\n");
	printf("Test Case 1:\nInput Data: RUNNING\nExpected Result: %d\nActual Result: %d\n\n", SERVER_OK, isBlockedAccount(&p));

	acc.state = BLOCKED;
	printf("Test Case 2:\nInput Data: BLOCKED\nExpected Result: %d\nActual Result: %d\n\n", BLOCKED_ACCOUNT, isBlockedAccount(&p));

	test_mode3 = 0;
}
//-----------------------------------------------------------------------
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t** accountRefrence) {
	return (termData->transAmount > (*accountRefrence)->balance ? LOW_BALANCE : SERVER_OK);
}

void isAmountAvailableTest(void) {
	test_mode3 = 1;

	ST_terminalData_t t = { 14.3f, 20.5f, "05/8/2022" };
	ST_accountsDB_t my_acc = { 17.3f, RUNNING, "9127465236546542" };

	ST_accountsDB_t* p = &my_acc;

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: isAmountAvailable\n\n");
	printf("Test Case 1:\nInput Data: 17.3\nExpected Result: %d\nActual Result: %d\n\n", SERVER_OK, isAmountAvailable(&t, &p)); // VALID, 17.3(BALANCE) > 14.3(TRANS_AMOUNT)

	my_acc.balance = 10.2; // INVALID, 10.2(BALANCE) < 14.3(TRANS_AMOUNT)
	printf("Test Case 2:\nInput Data: 10.2\nExpected Result: %d\nActual Result: %d\n\n", LOW_BALANCE, isAmountAvailable(&t, &p));

	test_mode3 = 0;
}
//-----------------------------------------------------------------------
EN_serverError_t saveTransaction(ST_transaction_t* transData) {
	if (trans_sequence >= 255) return SAVING_FAILED;

	transactionDB[trans_sequence] = *transData;
	trans_sequence++;

	return SERVER_OK;
}

void saveTransactionTest(void) {
	test_mode3 = 1;

	ST_cardData_t c_data = {"Youssef", "8989374615436851", "05/25"};
	ST_terminalData_t t_data = {10.8f, 20.3f, "04/11/2020"};
	ST_transaction_t trans_t = {c_data, t_data, APPROVED, trans_sequence};

	saveTransaction(&trans_t);
	
	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: saveTransaction\n\n");
	printf("Test Case 1:\nInput Data: trans_t\nExpected Result: %s\nActual Result: %s\n\n", "Youssef", transactionDB[0].cardHolderData.cardHolderName);

	test_mode3 = 0;
}
//------------------------------------------------------------------------
void listSavedTransactions(void) {
	for (int i = 0; i < trans_sequence; i++) {
		printf("#########################\nTransaction Sequence Number: %d\nTran\
 saction Date: %s\nTransaction Amount: %f\nTransaction State: %d\nTerminal\
 Max Amount: %f\nCardholder Name: %s\nPAN: %s\nCard\
 Expiration Date: %s\n#########################\n", transactionDB[i].transactionSequenceNumber,
			transactionDB[i].terminalData.transactionDate,
			transactionDB[i].terminalData.transAmount,
			transactionDB[i].transState, 
			transactionDB[i].terminalData.maxTransAmount, 
			transactionDB[i].cardHolderData.cardHolderName, 
			transactionDB[i].cardHolderData.primaryAccountNumber, 
			transactionDB[i].cardHolderData.cardExpirationDate
		);
	}
}

void listSavedTransactionsTest(void) {
	test_mode3 = 1;

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: listSavedTransactions\n\n");
	ST_cardData_t c_data = { "Youssef", "8989374615436851", "05/25" };
	ST_terminalData_t t_data = { 10.8f, 20.3f, "04/11/2020" };
	ST_transaction_t trans_t = { c_data, t_data, APPROVED, trans_sequence };

	saveTransaction(&trans_t);
	listSavedTransactions();

	test_mode3 = 0;
}
//-----------------------------------------------------------------------