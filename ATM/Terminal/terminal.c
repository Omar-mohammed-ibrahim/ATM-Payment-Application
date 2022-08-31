#include <stdio.h>
#include <time.h>
#include "terminal.h"
#define charToInt(x) ((x) % 48) // to convert a charachter to a number


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {


	time_t now = time(NULL);

	struct tm* cur_time = localtime(&now);

	strftime(termData->transactionDate, 11, "%d/%m/%Y", cur_time);


	return CARD_OK;


}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {

	
	uint8_t card_month = charToInt(cardData.cardExpirationDate[0]) * 10 + charToInt(cardData.cardExpirationDate[1]);
	uint8_t card_year = charToInt(cardData.cardExpirationDate[3]) * 10 + charToInt(cardData.cardExpirationDate[4]);

	uint8_t curr_month = charToInt(termData.transactionDate[3]) * 10 + charToInt(termData.transactionDate[4]);
	uint8_t curr_year = charToInt(termData.transactionDate[8]) * 10 + charToInt(termData.transactionDate[9]);
	
	if (curr_year > card_year)
	{
		return EXPIRED_CARD;
	}
	else if (curr_year == card_yeard && curr_month > card_month) 
	{
		return EXPIRED_CARD;
	}

	return TERMINAL_OK;


}
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {



	uint8_t i = 0, multiplier = 2, sum = 0, temp;

	while (cardData->primaryAccountNumber[i] != '\0')
	{
		// doubling the even placed digits from the leftmost
		temp = charToInt(cardData->primaryAccountNumber[i]) * multiplier;
		// if the doubled calue is consisted of two digits sup them up
		if (temp > 9) {
			temp = (temp / 10) + (temp % 10);

		}

		// toggling the multiplier between one and two
		if (multiplier == 2)
		{
			multiplier--;
		}
		else
		{
			multiplier++;
		}
		// summing up all the values to perform the last check
		sum += temp;

		i++;

	}

	// the final check is to see the summation is a multiplier of ten
	if (sum % 10 != 0)
	{
		return INVALID_CARD;
	}

	return TERMINAL_OK;

}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {

	printf("Please Enter Transaction Amount: ");

	scanf_s("%f", &termData->transAmount);

	if (termData->transAmount <= 0)
	{
		return INVALID_AMOUNT;
	}
	return TERMINAL_OK;

}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {

	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	return TERMINAL_OK;

}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {

	termData->maxTransAmount = 5000;

	return TERMINAL_OK;

}