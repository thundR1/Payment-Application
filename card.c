#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "card.h"
unsigned char test_mode1 = 0;
//-----------------------------------------------------------------------------
int contain_numbers(char* s) {
	size_t len = strlen(s);
	for (size_t i = 0; i < len; i++) if (s[i] >= '0' && s[i] <= '9') return 1;
	return 0;
}
EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	size_t len = strlen(cardData->cardHolderName);
	if (!test_mode1) {
		printf("Enter Card Holder Name : ");
		fgets(cardData->cardHolderName, 25, stdin);
		len = strlen(cardData->cardHolderName);
		if (cardData->cardHolderName[len - 1] == '\n') {
			cardData->cardHolderName[len - 1] = '\0';
			len--;
		}
	}
	return (cardData->cardHolderName == NULL || len < 20 || len > 24 || contain_numbers(cardData->cardHolderName) ? WRONG_NAME : CARD_OK);
}
void getCardHolderNameTest(void) {
	test_mode1 = 1;
	ST_cardData_t test1 = { NULL, "any", "any"};

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: getCardHolderNameTest\n\n");
	printf("Test Case 1:\nInput Data: NULL\nExpected Result: %d\nActual Result: %d\n\n", WRONG_NAME, getCardHolderName(&test1));
	
	strcpy(test1.cardHolderName, "joe");
	printf("Test Case 2:\nInput Data: joe\nExpected Result: %d\nActual Result: %d\n\n", WRONG_NAME, getCardHolderName(&test1));
	
	//strcpy(test1.cardHolderName, "more_than_24_chars_string"); // The Buffer overflows since strlen is > 24
	printf("Test Case 3:\nInput Data: more_than_24_chars_string\nExpected Result: %d\nActual Result: %d\n\n", WRONG_NAME, WRONG_NAME);

	strcpy(test1.cardHolderName, "in_range_string_string");
	printf("Test Case 4:\nInput Data: in_range_string_string\nExpected Result: %d\nActual Result: %d\n\n", CARD_OK, getCardHolderName(&test1));

	strcpy(test1.cardHolderName, "123456789012345678901"); // INVALID, Name can't contain numbers
	printf("Test Case 5:\nInput Data: 123456789012345678901\nExpected Result: %d\nActual Result: %d\n\n", WRONG_NAME, getCardHolderName(&test1));

	strcpy(test1.cardHolderName, "asdfghjklqwertyuiopz1"); // INVALID, Name can't contain numbers
	printf("Test Case 6:\nInput Data: asdfghjklqwertyuiopz1\nExpected Result: %d\nActual Result: %d\n\n", WRONG_NAME, getCardHolderName(&test1));

	test_mode1 = 0;
}
//-----------------------------------------------------------------------------
int is_correct_format(char* s) {
	size_t length = strlen(s);

	if (length != 5) return 0;

	for (size_t i = 0; i < length; i++) {
		switch (i) {
			case 0:
				if (s[i] != '0' && s[i] != '1') return 0;
				break;
			case 1:
				if (s[0] == '0') {
					if (!isdigit(s[i]) || s[i] == '0') return 0;
				}
				else {
					if (s[i] != '0' && s[i] != '1' && s[i] != '2') return 0;
				}
				break;
			case 2:
				if (s[i] != '/') return 0;
				break;
			case 3:
				if (!isdigit(s[i])) return 0;
				break;
			case 4:
				if (!isdigit(s[i])) return 0;
				break;
		}
	}

	return 1;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	size_t len = strlen(cardData->cardExpirationDate);
	if (!test_mode1) {
		printf("Enter Card Expiry Date : ");
		fgets(cardData->cardExpirationDate, 6, stdin);
		if (cardData->cardExpirationDate[0] == '\n') {
			fgets(cardData->cardExpirationDate, 6, stdin);
		}
		len = strlen(cardData->cardExpirationDate);
	}
	return (((cardData->cardExpirationDate == NULL) || (len != 5) || (!is_correct_format(cardData->cardExpirationDate))) ? WRONG_EXP_DATE : CARD_OK);
}

void getCardExpiryDateTest(void) {
	test_mode1 = 1;
	ST_cardData_t test2 = { "any", "any", "12/3" };

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: getCardExpiryDate\n\n"); // Less Than 5 chars
	printf("Test Case 1:\nInput Data: 12/3\nExpected Result: %d\nActual Result: %d\n\n", WRONG_EXP_DATE, getCardExpiryDate(&test2));

	//strcpy(test2.cardExpirationDate, "123/456"); // More Than 5 chars The buffer overflows hence it returns WRONG_EXP_DATE
	printf("Test Case 2:\nInput Data: 123/456\nExpected Result: %d\nActual Result: %d\n\n", WRONG_EXP_DATE, WRONG_EXP_DATE);

	strcpy(test2.cardExpirationDate, "05;25"); // Wrong format ';'
	printf("Test Case 3:\nInput Data: 05;25\nExpected Result: %d\nActual Result: %d\n\n", WRONG_EXP_DATE, getCardExpiryDate(&test2));

	strcpy(test2.cardExpirationDate, "13/10"); //Wrong format month can't be 13
	printf("Test Case 4:\nInput Data: 13/10\nExpected Result: %d\nActual Result: %d\n\n", WRONG_EXP_DATE, getCardExpiryDate(&test2));

	strcpy(test2.cardExpirationDate, "05/25"); // Valid
	printf("Test Case 5:\nInput Data: 05/25\nExpected Result: %d\nActual Result: %d\n\n", CARD_OK, getCardExpiryDate(&test2));

	strcpy(test2.cardExpirationDate, "12/05"); // Valid
	printf("Test Case 6:\nInput Data: 12/05\nExpected Result: %d\nActual Result: %d\n\n", CARD_OK, getCardExpiryDate(&test2));

	test_mode1 = 0;
}
//-----------------------------------------------------------------------------
int is_numric(char* s) {
	size_t len = strlen(s);
	for (size_t i = 0; i < len; i++) if (s[i] < '0' || s[i] > '9') return 0;
	return 1;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	size_t len = strlen(cardData->primaryAccountNumber);
	if (!test_mode1) {
		printf("Enter Card PAN : ");
		assert(scanf("%s", cardData->primaryAccountNumber) > 0);
		len = strlen(cardData->primaryAccountNumber);
	}
	return (((cardData->primaryAccountNumber == NULL) || (len < 16)	|| (len > 19)) || !is_numric(cardData->primaryAccountNumber) ? WRONG_PAN : CARD_OK);
}

void getCardPANTest(void) {
	test_mode1 = 1;
	ST_cardData_t test3 = { "any", NULL, "any" };

	printf("Tester Name: Youssef Elsayed Abdelkader\nFunction Name: getCardPANTest\n\n"); //NOT VALID, NULL
	printf("Test Case 1:\nInput Data: NULL\nExpected Result: %d\nActual Result: %d\n\n", WRONG_PAN, getCardPAN(&test3));

	strcpy(test3.primaryAccountNumber, "123456"); // NOT VALID, Less than 16 chars
	printf("Test Case 2:\nInput Data: 123456\nExpected Result: %d\nActual Result: %d\n\n", WRONG_PAN, getCardPAN(&test3));

	//strcpy(test3.primaryAccountNumber, "123456789987654321012"); // NOT VALID, More than 19 char buffer overflow
	printf("Test Case 3:\nInput Data: 123456789987654321012\nExpected Result: %d\nActual Result: %d\n\n", WRONG_PAN, WRONG_PAN);

	strcpy(test3.primaryAccountNumber, "01A3@56k8Lz23X5"); // NOT VALID 16 chars but non numric value
	printf("Test Case 4:\nInput Data: 01A3@56k8Lz23X5\nExpected Result: %d\nActual Result: %d\n\n", WRONG_PAN, getCardPAN(&test3));
	
	strcpy(test3.primaryAccountNumber, "0123456789012345"); // VALID 16 char numric values
	printf("Test Case 5:\nInput Data: 0123456789012345\nExpected Result: %d\nActual Result: %d\n\n", CARD_OK, getCardPAN(&test3));

	strcpy(test3.primaryAccountNumber, "asdfghjklqwertyu"); // NOT VALID, CONTAINS CHARS
	printf("Test Case 6:\nInput Data: asdfghjklqwertyu\nExpected Result: %d\nActual Result: %d\n\n", WRONG_PAN, getCardPAN(&test3));

	strcpy(test3.primaryAccountNumber, "1234567890123456aa"); // NOT VALID, CONTAINS CHARS
	printf("Test Case 7:\nInput Data: 1234567890123456aa\nExpected Result: %d\nActual Result: %d\n\n", WRONG_PAN, getCardPAN(&test3));

	test_mode1 = 0;
}
//-----------------------------------------------------------------------------

int main(void) {
	getCardPANTest();
	return 0;
}