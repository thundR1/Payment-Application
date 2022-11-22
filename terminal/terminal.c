#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "./terminal.h"
unsigned char test_mode2 = 0;
//----------------------------------------------------------------
int is_wrong_format(char* s) {
	size_t len = strlen(s);
	if (len < 10) return 1;
	for (size_t i = 0; i < len; i++) {
		switch (i) {
			case 0:
				if (s[i] < '0' || s[i] > '3') return 1;
				break;
			case 1:
				if (s[0] == '0') {
					if (s[i] == '0') return 1;
				}
				else {
					if (s[i] < '0' || s[i] > '9') return 1;
				}
				break;
			case 2:
				if (s[i] != '/') return 1;
				break;
			case 3:
				if (s[i] != '0' && s[i] != '1') return 1;
				break;
			case 4:
				if (s[3] == '0') {
					if (s[i] < '0' || s[i] > '9') return 1;
				}
				else {
					if (s[i] < '0' || s[i] > '2') return 1;
				}
				break;
			case 5:
				if (s[i] != '/') return 1;
				break;
			case 6:
				if (s[i] == '0' || !isdigit(s[i])) return 1;
				break;
			case 7:
				if (s[i] < '0' || s[i] > '9') return 1;
				break;
			case 8:
				if (s[i] < '0' || s[i] > '9') return 1;
				break;
			case 9:
				if (s[i] < '0' || s[i] > '9') return 1;
				break;
		}
	}
	return 0;
}

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	size_t len = strlen(termData->transactionDate);
	if (!test_mode2) {
		printf("Enter Transaction Date : ");
		assert(scanf("%s", termData->transactionDate) > 0);
		len = strlen(termData->transactionDate);
	}
	return ((termData->transactionDate == NULL) || is_wrong_format(termData->transactionDate) ? WRONG_DATE : TERMINAL_OK);
}

void getTransactionDateTest(void) {
	test_mode2 = 1;

	ST_terminalData_t test1 = { 0.0f, 0.0f, NULL };

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: getTransactionDate\n\n"); // NOT VALID, NULL
	printf("Test Case 1:\nInput Data: NULL\nExpected Result: %d\nActual Result: %d\n\n", WRONG_DATE, getTransactionDate(&test1));

	strcpy(test1.transactionDate, "12/09/2"); // NOT VALID, LESS THAN 10 CHARS
	printf("Test Case 2:\nInput Data: 12/09/2\nExpected Result: %d\nActual Result: %d\n\n", WRONG_DATE, getTransactionDate(&test1));

	strcpy(test1.transactionDate, "10/11$2019"); //NOT VALID, WRONG FORMAT
	printf("Test Case 3:\nInput Data: 10/11$2019\nExpected Result: %d\nActual Result: %d\n\n", WRONG_DATE, getTransactionDate(&test1));

	strcpy(test1.transactionDate, "02/09/2M08"); // NOT VALID, WRONG FORMAT
	printf("Test Case 4:\nInput Data: 02/09/2M02\nExpected Result: %d\nActual Result: %d\n\n", WRONG_DATE, getTransactionDate(&test1));

	strcpy(test1.transactionDate, "09/05/2022"); // VALID
	printf("Test Case 5:\nInput Data: 09/05/2022\nExpected Result: %d\nActual Result: %d\n\n", TERMINAL_OK, getTransactionDate(&test1));

	strcpy(test1.transactionDate, "12/11/2018"); // VALID
	printf("Test Case 6:\nInput Data: 12/11/2018\nExpected Result: %d\nActual Result: %d\n\n", TERMINAL_OK, getTransactionDate(&test1));

	test_mode2 = 0;
}
//----------------------------------------------------------------
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {
	char* s1 = cardData->cardExpirationDate, *s2 = termData->transactionDate;
	size_t len1 = strlen(s1), len2 = strlen(s2);
	char str1[3] = "", str2[3] = "", str3[3] = "", str4[3] = "";
	str1[0] = s1[len1 - 2]; str1[1] = s1[len1 - 1];
	str2[0] = s2[len2 - 2]; str2[1] = s2[len2 - 1];
	str3[0] = s1[0]; str3[1] = s1[1];
	str4[0] = s2[len2 - 7]; str4[1] = s2[len2 - 6];
	int year_exp = atoi(str1), year_trans = atoi(str2);
	int month_exp = atoi(str3), month_trans = atoi(str4);
	if (year_exp < year_trans)
		return EXPIRED_CARD;
	if (year_exp == year_trans) {
		if (month_exp < month_trans) return EXPIRED_CARD;
	}
	return TERMINAL_OK;
}

void isCardExpriedTest(void) {
	test_mode2 = 1;

	ST_terminalData_t test2 = { 0.0f, 0.0f, "05/10/2021"};
	ST_cardData_t test3 = { "any", "any", "05/20" };

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: isCardExpired\n\n");
	printf("Test Case 1:\nInput Data: 05/10/2021 , 05/20\nExpected Result: %d\nActual Result: %d\n\n", EXPIRED_CARD, isCardExpired(&test3, &test2));

	strcpy(test2.transactionDate, "09/12/2022");
	strcpy(test3.cardExpirationDate, "10/22");
	printf("Test Case 2:\nInput Data: 09/12/2022 , 10/22\nExpected Result: %d\nActual Result: %d\n\n", EXPIRED_CARD, isCardExpired(&test3, &test2));

	strcpy(test2.transactionDate, "24/10/2022");
	strcpy(test3.cardExpirationDate, "11/22");
	printf("Test Case 3:\nInput Data: 24/10/2022 , 11/2022\nExpected Result: %d\nActual Result: %d\n\n", TERMINAL_OK, isCardExpired(&test3, &test2));
	
	strcpy(test2.transactionDate, "18/12/2021");
	strcpy(test3.cardExpirationDate, "10/22");
	printf("Test Case 4:\nInput Data: 18/12/2021 , 10/22\nExpected Result: %d\nActual Result: %d\n\n", TERMINAL_OK, isCardExpired(&test3, &test2));

	test_mode2 = 0;
}
//----------------------------------------------------------------
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	if (!test_mode2) {
		printf("Enter the transaction amount: ");
		assert(scanf("%f", &termData->transAmount) > 0);
	}
	return (termData->transAmount <= 0 ? INVALID_AMOUNT : TERMINAL_OK);
}

void getTransactionAmountTest(void) {
	test_mode2 = 1;

	ST_terminalData_t test4 = { -12.4f, 0.0f, "any"};

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: getTransactionAmount\n\n"); // -12.4 SMALLER THAN 0 INVALID AMOUNT
	printf("Test Case 1:\nInput Data: -12.4\nExpected Result: %d\nActual Result: %d\n\n", INVALID_AMOUNT, getTransactionAmount(&test4));

	test4.transAmount = 9.3f; // VALID
	printf("Test Case 2:\nInput Data: 9.3\nExpected Result: %d\nActual Result: %d\n\n", TERMINAL_OK, getTransactionAmount(&test4));

	test_mode2 = 0;
}
//----------------------------------------------------------------
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {
	return (termData->transAmount > termData->maxTransAmount ? EXCEED_MAX_AMOUNT : TERMINAL_OK);
}

void isBelowMaxAmountTest(void) {
	test_mode2 = 1;

	ST_terminalData_t test5 = { 15.3f, 10.2f, "any" };

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: isBelowMaxAmount\n\n"); // 15.3 IS INVALID MAX IS 10.2
	printf("Test Case 1:\nInput Data: 15.3\nExpected Result: %d\nActual Result: %d\n\n", EXCEED_MAX_AMOUNT, isBelowMaxAmount(&test5));

	test5.transAmount = 8.4; // VALID
	printf("Test Case 2:\nInput Data: 9.3\nExpected Result: %d\nActual Result: %d\n\n", TERMINAL_OK, isBelowMaxAmount(&test5));

	test_mode2 = 0;
}
//----------------------------------------------------------------
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {
	if (maxAmount <= 0) return INVALID_MAX_AMOUNT;
	termData->maxTransAmount = maxAmount;
	return TERMINAL_OK;
}

void setMaxAmountTest(void) {
	test_mode2 = 1;

	ST_terminalData_t test6 = { 8.0f, 15.0f, "any" }; // Initialized with random values

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: setMaxAmount\n\n"); // -5.4 IS INVALID MAX AMOUNT
	printf("Test Case 1:\nInput Data: -5.4\nExpected Result: %d\nActual Result: %d\n\n", INVALID_MAX_AMOUNT, setMaxAmount(&test6, -5.4f));

	test6.maxTransAmount = 4.8f; // VALID MAX AMOUNT
	printf("Test Case 2:\nInput Data: 4.8\nExpected Result: %d\nActual Result: %d\n\n", TERMINAL_OK, setMaxAmount(&test6, 4.8f));

	test_mode2 = 0;
}
//----------------------------------------------------------------