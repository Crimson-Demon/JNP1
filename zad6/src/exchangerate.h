#ifndef _EXCHANGER_H_
#define _EXCHANGER_H_

#include <utility>

/*Klasa sluzaca do przechowywania kursow
  Trzyma kursy zakupu i kursy sprzedazy
  dla pewnej waluty
  Udostepnia metody ustawiania cen zakupu i sprzedazy
  jak i ich pobierania
*/

class ExchangeRate {
	double _buying_rate;
	double _selling_rate;
	public:
		ExchangeRate();
		ExchangeRate(ExchangeRate&) = default;
		ExchangeRate(ExchangeRate&&) = default;
		~ExchangeRate();
		/*Metoda zwraca referencje do obiektu typu kurs
		  i ustawia jego kurs zakupu na _buy
		*/
		ExchangeRate& buyingRate(double _buy);
		/*Metoda zwraca referencje do obiektu typu kurs
		  i ustawia jego kurs sprzedazy na _sell
		*/
		ExchangeRate& sellingRate(double _sell);
		/*Getter kursu zakupu*/
		double getBuyingRate() const;
		/*Getter kursu sprzedazy*/
		double getSellingRate() const;
};

#endif