#include "exchangerate.h"

/*Domyslne wartosci to sa 1.00 dla zakupu
  i 1.00 dla sprzedazy
*/
ExchangeRate::ExchangeRate() {
	_buying_rate = 1.00;
	_selling_rate = 1.00;
}

ExchangeRate::~ExchangeRate() { }

ExchangeRate& ExchangeRate::buyingRate(double _buy) {
	_buying_rate = _buy;
	return *this;
}

ExchangeRate& ExchangeRate::sellingRate(double _sell) {
	_selling_rate = _sell;
	return *this;
}

double ExchangeRate::getBuyingRate() const {
	return _buying_rate;
}

double ExchangeRate::getSellingRate() const {
	return _selling_rate;
}
