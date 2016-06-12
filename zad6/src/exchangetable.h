#ifndef _EXCHANGET_H_
#define _EXCHANGET_H_

#include "exchangerate.h"
#include "account.h"
#include <map>
#include <memory> //dla shared_pointer
#include <utility>


/*Pomocniczy typedef
  opisuje glowny element tabeli kursow walutowych czyli sama tabele
  jest to mapa o kluczach w walutach
  trzymajaca obiekty typu ExchangeRate
  czyli kursy walutowe
*/
typedef std::map<Currency, std::shared_ptr<ExchangeRate>> CurrencyTable;


/*Klasa ExchangeTable
  trzyma ona tabele kursow walutowych
  udostepnia metody wybory kursow dla danych walut
  jak i szybkiego uzyskania cen kupna i sprzedazy
  dla podanych walut
*/
class ExchangeTable {
	CurrencyTable _table;
	public:
		ExchangeTable();
		/*Move i Copy constructor sa niepotrzebne, bo nie planujemy
		  kopiowac obiektow tego typu, gdyz nie bedziemy kopiowac bankow
		*/
		ExchangeTable(ExchangeTable&) = delete;
		ExchangeTable(ExchangeTable&&) = delete;
		~ExchangeTable();
		/*Zwraca kurs walutowy odpowiadajacy walucie _curr*/
		ExchangeRate& exchangeRate(Currency _curr);
		/*Zwraca cene zakupu kursu walutowego _curr*/
		double getBuyingRate(Currency _curr) const;
		/*Zwraca cene sprzedazy kursu walutowego _curr*/
		double getSellingRate(Currency _curr) const;
};

#endif