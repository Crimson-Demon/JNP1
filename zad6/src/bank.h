#ifndef _BANK_H_
#define _BANK_H_

#include "account.h"
#include "exchangetable.h"
#include "citizens.h"
#include <map>
#include <memory>
#include <vector>
#include <utility>
#include <string>

/*Mapa BankParameters posiada klucz dla kazdego sensownego rodzaju konta
  i trzyma wektory 3ech double'i - odpowiadaja one wysokosci oplaty miesiecznej,
  wysokosci oplaty transakcyjnej i wysokoscji oprocentowania
*/
typedef std::map<AccountType, std::vector<double> > BankParameters;

/*Mapa AccountList trzyma inty - nr konta i konto jako shared_ptr
*/
typedef std::map<int, std::shared_ptr<Account>> AccountList;

/*Klasa bank przechowuje w sobie nazwe banku, jego id oraz
  parametry, liste kont (dla kazdego typu konta trzymamy osobna liste)
  i tabele kursow walutowych.
*/
class Bank {
	const std::string _name;
	const int _id;
	const BankParameters _bank_parameters;
	std::map<AccountType, AccountList> _account_lists;
	ExchangeTable _exchange_table;
	public:
		Bank(const int _id, const std::string _n);
		Bank(const int _id, const std::string _n, const BankParameters& _b_param);
		/*Nie ma move i copy constructorow
		  bo nie ma sensu kopiowac banku
		*/
		Bank(Bank& _bank) = delete;
		Bank(Bank&& _bank) = delete;
		~Bank();
		/*Zwraca tabele kursow walutowych*/
		ExchangeTable& exchangeTable();
		/*Nalicza odsetki dla wszystkich konto*/
		void capitalizeInterest();
		/*pobiera miesieczna oplate od wszystkich kont*/
		void collectMonthlyFee();
		/*Pobiera oplate transakcyjna od konta o ID _acc_id*/
		void collectTransferFee(AccountID _acc_id);
		/*Tworzy konto rozliczeniowe dla mieszkanca _citizen*/
		CheckingAccount& openCheckingAccount(const Citizen& _citizen);
		/*Tworzy konto oszczednosciowe dla mieszkanca _citizen*/
		SavingAccount& openSavingAccount(const Citizen& _citizen);
		/*Tworzy konto walutowe dla mieszkanca _citizen z waluta _currency (domyslnie ENC)*/
		CurrencyAccount& openCurrencyAccount(const Citizen& _citizen, const Currency _currency = Currency::ENC);
		/*Wyszukuje konto o podanym _AccoundID*/
		Account& findAccount(const AccountID _acc_nr) const;
		/*Wyszukuje konto typu _acc_type o numerze _acc_nr*/
		Account& findAccount(int _acc_nr, AccountType _acc_type) const;
		/*Metoda przesyla kwote _d od konta _from_id do konta _to_id z adnotacja _comment
		  Przeslanie nastepuje poprzez znalezienie odpowiedniego banku za pomoca GKB
		  i przekazanie jemu informacji nt przelewu
		*/
		bool transferFrom(const double _d, const AccountID _from_id , const AccountID _to_id, const std::string _comment = "");
		/*Metoda przyjmuje kwote _d od konta _from_id do konta _to_id z adnotacja _comment
		  Jest efektem wywolania metody transferFrom, ktora poprzed GKB wyszukuje wlasciwy bank i jemu przekazuje
		  dalsze informacje nt przelewu
		*/
		bool transferTo(const double _d, const AccountID _from_id , const AccountID _to_id, const std::string _comment = "");
		/*getter dla oplaty transakcyjnej w zaleznosci od konta*/
		double getTransferFee(AccountType _acc_t) const;
		/*getter dla oplaty miesiecznej w zaleznosci od konta*/
		double getMonthlyFee(AccountType _acc_t) const;
		/*getter dla wysokosci oprocentowania w zaleznosci od konta*/
		double getInterestRate(AccountType _acc_t) const;
};

#endif