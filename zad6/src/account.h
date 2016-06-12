#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include "interstellarclock.h"
#include <string>
#include <iostream>
#include <vector>
#include <utility>

/*Enumerator typow walut*/
enum class Currency {ENC, BIC, DIL, LIT};
/*Overloadowany operator << dla typow walut*/
std::ostream& operator <<(std::ostream& stream, const Currency& cur);

/*Enumerator typow operacji*/
enum class OperationType {DEPOSIT, WITHDRAWAL, TRANSFER, CHARGE, INTEREST};
/*Overloadowany operator << dla typow operacji*/
std::ostream& operator <<(std::ostream& stream, const OperationType& op);

/*Enumerator typow kont*/
enum class AccountType {CHECK, SAVE, CURR, NONE};
/*Overloadowany operator << dla typow kont*/
std::ostream& operator <<(std::ostream& stream, const AccountType& _ac_t);

/*Strutktura AccountID trzyma najwazniejsze informacje nt konta:
  jego numer, jego typ oraz identyfikator banku
*/
struct AccountID {
	private:
		int _account_nr;
		AccountType _account_t;
		int _bank_id;
	public:
		/*Overloadowany operator <<*/
		friend std::ostream& operator <<(std::ostream& stream, const AccountID& _a_id);
		/*konwersja do stringa*/
		operator std::string() const;
		/*zwraca wynik konwersji do stringa*/
		std::string toString() const;
		/*getter numeru konta*/
		int getNr() const;
		/*getter typu konta*/
		AccountType getType() const;
		/*getter identyfikatora banku*/
		int getBankID() const;
		/*ustawia parametry ID*/
		void set(int _nr, AccountType _type, int _b_id);
};

/*Klasa Operation odpowiada operacjom:
  Deposit, withdrawal, charge, interest
  Trzyma date operacji, wysokosc, walute oraz typ
*/
class Operation {
	protected:
		Date _date;
		double _amount;
		Currency _currency;
		OperationType _operation_t;
	public:
		Operation(Date _i, double _d, Currency _curr, OperationType _op_t);
		virtual ~Operation();
		friend std::ostream& operator <<(std::ostream& stream, const Operation& op);
		double getAmount() const;
};

/*Klasa transfer rozszerza Operation
  Wynika to z tego, ze trzyma dodatkowo informacje:
  komentarz, od kogo i do kogo przelew
  Przez to ma troche inny operator <<
*/
class Transfer: public Operation {
	public:
		Transfer(Date _i, double _d, Currency _curr, OperationType _op_t,
				 std::string _comm, std::string _fro, std::string _t );
		friend std::ostream& operator <<(std::ostream& stream, const Transfer& op);
private:
		std::string _comment;
		std::string _from;
		std::string _to;
};

/*Glowna klasa Account
  trzyma obiekt AccountID, string identyfikujacy wlasciciela,
  aktualny balans, vector zawierajacy operacje (historia operacji)
  i walute konta
*/
class Account {
	protected:
		AccountID _account_id;
		std::string _owner_id;
		double _balance;
		std::vector<Operation> _operation_history;
		Currency _currency;
	public:
		Account(int _acc_nr, int _bank_id, std::string _citizen_id, AccountType _account_type,
		        Currency _curr);
		Account(Account&) = delete;
		Account(Account&&) = delete;
		virtual ~Account();
		/*zwraca const AcountID konta*/
		const AccountID& id() const;
		/*zwraca informacje nt balansu w postaci stringu*/
		std::string balance() const;
		/*zwraca string z historia operacji jedna pod druga*/
		std::string history() const;
		/*transfer dla id_konta podanego jako string*/
		bool transfer(double _d, std::string _to_id, std::string _comment = "");
		/*transfer kwoty _d z obecnego konto do _to_id z komentarzem _comment*/
		bool transfer(double _d, AccountID _to_id, std::string _comment = "");
		/*Sluzy odbierania kwoty przelewanej nam*/
		bool transferTo(double _d, AccountID _from_id, std::string _comment = "");
		/*Overloadowany operator <<*/
		friend std::ostream& operator <<(std::ostream& stream, const Account& acc);
		/*Pobiera miesieczne oplaty*/
		void collectMonthlyFee();
		/*Nalicza odsetki*/
		void capitalizeInterest();
		/*Pobiera oplate za przelew*/
		void collectTransferFee();
};

/*Klasa CheckingAccount odpowiada
  kontu rozliczeniowemu
  jest zawsze w ENC
*/
class CheckingAccount: public Account {
	public:
		CheckingAccount(int _acc_nr, int _bank_id, std::string _citizen_id, AccountType _account_type,
		                Currency _curr = Currency::ENC);
		/*Dwa prototypy dla withdraw i deposit pozwalaja na
		  withdraw({kwota, waluta}) i  withdraw(kwota, waluta)
		*/
		bool withdraw(double _amount, Currency _curr = Currency::ENC);
		bool withdraw(std::pair<double, Currency> _pair);
		bool deposit(double _amount, Currency _curr = Currency::ENC);
		bool deposit(std::pair<double, Currency> _pair);
};

/*Klasa SavingAccount odpowiada
  kontu oszczednosciowemu
  nie zezwala na wplaty i wyplaty, tylko na przelewy
  jest zawsze w ENC
*/
class SavingAccount: public Account {
	public:
		SavingAccount(int _acc_nr, int _bank_id, std::string _citizen_id, AccountType _account_type,
		              Currency _curr = Currency::ENC);
};

/*Klasa CurrencyAccount odpowiada
  kontu walutowemu
  zezwala na wplaty i wyplaty w roznych walutach,
  przelewy w jednej,
  moze byc w roznych walutach prowadzona
*/
class CurrencyAccount: public Account {
	public:
		CurrencyAccount(int _acc_nr, int _bank_id, std::string _citizen_id, AccountType _account_type,
		                Currency _curr);
		/*Dwa prototypy dla withdraw i deposit pozwalaja na
		  withdraw({kwota, waluta}) i  withdraw(kwota, waluta)
		*/
		bool withdraw(double _amount, Currency _curr);
		bool withdraw(std::pair<double, Currency> _pair);
		bool withdraw( double _amount);
		void deposit(double _amount, Currency _curr);
		void deposit(std::pair<double, Currency> _pair);
		void deposit(double _amount);
};

#endif