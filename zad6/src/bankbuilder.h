#ifndef _BANKBUILDER_H_
#define _BANKBUILDER_H_

#include "bank.h"
#include <vector>
#include <utility>
#include <string>

/*Klasa BankBuilder sluzy do budowania banku i
  upraszcza to poprzez realizacje interfejsu fluent
*/
/*W polaczeniu z klasa Bank i GKB realizuje wzorzec builder:
  GKB tworzy obiekt typu BankBuilder, ktory nastepnie
  przyjmuje ustawienia banku i jego parametry oraz
  przesylajac zebrane informacje, przesyla zadanie tworzenia banku
  do GKB.
*/
class BankBuilder {
	std::string _name;
	BankParameters _bank_parameters;
	/*Sluzy by wiedziec, jaki jest typ obecnie modyfikowanego konta
	jesli jest NONE to wtedy parametry sa ustawiane globalne*/
	AccountType _mod_account_type;

	public:
		BankBuilder();
		BankBuilder(BankBuilder& _bank_builder) = delete;
		BankBuilder(BankBuilder&& _bank_builder) = delete;
		~BankBuilder();
		/*ustawia nazwe aktualnie budowanego banku na _s*/
		BankBuilder& name(const std::string _s);
		/*ustawia aktualnie modyfikowany typ konta na konto rozliczeniowe*/
		BankBuilder& checkingAccount();
		/*ustawia aktualnie modyfikowany typ konta na konto oszczednosciowe*/
		BankBuilder& savingAccount();
		/*ustawia aktualnie modyfikowany typ konta na konto walutowe*/
		BankBuilder& currencyAccount();
		/*ustawia miesieczna oplate w wysokosci _d dla aktualnego typu konta*/
		BankBuilder& monthlyCharge(double _d);
		/*Ustawia oplate transakcyjna w wysokosci _d dla aktualnego typu konta*/
		BankBuilder& transferCharge(double _d);
		/*Ustawia oprocentowanie dla danego typu konta na wysokosc _d*/
		BankBuilder& interestRate(double _d);
		/*Tworzy bank*/
		Bank& createBank();
		/*Resetuje parametry BankBuildera, by nowy bank nie dziedziczyl parametrow innego*/
		void resetParameters();
};

#endif