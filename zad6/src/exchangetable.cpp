#include "gsbexception.h"
#include "exchangetable.h"

ExchangeTable::ExchangeTable() {
	_table.emplace(Currency::ENC, std::make_shared<ExchangeRate>());
	_table.emplace(Currency::BIC, std::make_shared<ExchangeRate>());
	_table.emplace(Currency::DIL, std::make_shared<ExchangeRate>());
	_table.emplace(Currency::LIT, std::make_shared<ExchangeRate>());
}

ExchangeTable::~ExchangeTable() { }

/*Ponizsze metody rzucaja wyjatek
  CurrencyNotFound
  gdy nie pyta sie o nieistniejaca walute
*/
ExchangeRate& ExchangeTable::exchangeRate(Currency _curr) {
	if(_table.find(_curr) != _table.end())
		return *_table[_curr];
	else
		throw CurrencyNotFound();
}

double ExchangeTable::getBuyingRate(Currency _curr) const {
	if(_table.find(_curr) != _table.end())
		return (*_table.at(_curr)).getBuyingRate();
	else
		throw CurrencyNotFound(); 
}

double ExchangeTable::getSellingRate(Currency _curr) const {
	if(_table.find(_curr) != _table.end())
		return (*_table.at(_curr)).getSellingRate();
	else
		throw CurrencyNotFound(); 
}
