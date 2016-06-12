#include "account.h"
#include "gkb.h"
#include "gsbexception.h"
#include <boost/regex.hpp>
#include <sstream>
#include <iomanip>

std::ostream& operator <<(std::ostream& stream, const Currency& cur) {
	switch (cur) {
		case Currency::ENC: return stream << "ENC";
		case Currency::BIC: return stream << "BIC";
		case Currency::DIL: return stream << "DIL";
		case Currency::LIT: return stream << "LIT";
	}
	return stream;
}

std::ostream& operator <<(std::ostream& stream, const OperationType& op) {
	switch (op) {
		case OperationType::DEPOSIT: return stream << "DEPOSIT";
		case OperationType::WITHDRAWAL: return stream << "WITHDRAWAL";
		case OperationType::TRANSFER: return stream << "TRANSFER";
		case OperationType::CHARGE: return stream << "CHARGE";
		case OperationType::INTEREST: return stream << "INTEREST";
	}
	return stream;
}

std::ostream& operator <<(std::ostream& stream, const AccountType& _ac_t) {
	switch(_ac_t) {
		case AccountType::CHECK: return stream << "CHK";
		case AccountType::SAVE: return stream << "SAV";
		case AccountType::CURR: return stream << "CUR";
		case AccountType::NONE: return stream << "";
	}
	return stream;
}

/*Zwraca string postaci B1-A2-XYZ, gdzie XYZ to waluta*/
AccountID::operator std::string() const {
	std::stringstream _ss;
	_ss << "B" << _bank_id << "-A" << _account_nr << "-" << _account_t;
	return _ss.str();
}

std::string AccountID::toString() const {
	return *this;
}

int AccountID::getNr() const {
	return _account_nr;
}

AccountType AccountID::getType() const {
	return _account_t;
}

int AccountID::getBankID() const {
	return _bank_id;
}

void AccountID::set(int _nr, AccountType _type, int _b_id) {
	_account_nr = _nr;
	_account_t = _type;
	_bank_id = _b_id;
}

std::ostream& operator <<(std::ostream& stream, const AccountID& _account_id) {
	stream << _account_id.toString();
	return stream;
}

Operation::Operation(Date _i, double _d, Currency _curr, OperationType _op_t) {
	_date = _i;
	_amount = _d;
	_currency = _curr;
	_operation_t = _op_t;
}

Operation::~Operation() { }

std::ostream& operator<<(std::ostream& stream, const Operation& op) {
	stream << op._date << " " << std::setprecision(2) << std::fixed << op._amount;
	stream << op._currency << " " << op._operation_t << std::endl;
	return stream;
}

double Operation::getAmount() const {
	return _amount;
}

Transfer::Transfer(Date _i, double _d, Currency _curr, OperationType _op_t, std::string _comm, std::string _fro, std::string _t ) :
	Operation(_i, _d, _curr, _op_t),
	_comment(_comm),
	_from(_fro),
	_to(_t) {}

std::ostream& operator<<(std::ostream& stream, const Transfer& op) {
	stream << op._date << " " << std::setprecision(2) << std::fixed << op._amount;
	stream << op._currency << " " << op._operation_t << "(" << op._comment << ") FROM: ";
	stream << op._from << " TO:" << op._to << std::endl;
	return stream;
}

Account::Account(int _acc_nr, int _b_id, std::string _owner, AccountType _account_type, Currency _curr) {
	_account_id.set(_acc_nr, _account_type, _b_id);
	_owner_id = _owner;
	_currency = _curr;
	_balance = 0.0;
}

Account::~Account() { };

std::string Account::balance() const {
	std::stringstream _ss;
	_ss << std::setprecision(2) << std::fixed << _balance << _currency;
	return _ss.str();
}

std::string Account::history() const {
	std::stringstream _ss;
	for(auto& i: _operation_history) {
		if(i.getAmount() != 0) {
			_ss << i;
		}
	}
	return _ss.str();
}

const AccountID& Account::id() const {
	return _account_id;
}

bool Account::transfer(double _d, std::string _to_id, std::string _comment) {
	/*Boostem sprawdzam najpierw, _to_id jest dobrym stringiem ID tzn czy jest postaci B1-A2-XYZ,
	  gdzie XYZ to typ konta
	*/
	if(boost::regex_match(_to_id, boost::regex("^[B][0-9]{1,1000}[-][A][0-9]{1,1000}[-](CUR|CHK|SAV)$"))) {
		std::string _acc_sbstr = _to_id.substr(_to_id.length() - 3, 3);
		AccountType _acc_t;
		/*Pobieram typ konta z ostatnich trzech liter*/
		if(_acc_sbstr == "CHK") {
			_acc_t = AccountType::CHECK;
		} else if(_acc_sbstr == "CUR") {
			_acc_t = AccountType::CURR;
		} else if(_acc_sbstr == "SAV") {
			_acc_t = AccountType::SAVE;
		}
		/*wyliczam pozycje numeru banku i numeru konta*/
		size_t _pos1 = _to_id.find("-");
		size_t _pos2 = _to_id.rfind("-");
		size_t _len = _to_id.length();
		int _bank_nr = std::stoi(_to_id.substr(1, _pos1 - (size_t) 1));
		int _acc_nr = std::stoi(_to_id.substr(_pos1 + (size_t) 2, _pos2 - _pos1 - (size_t) 2));
		/*Wyszukuje ID konta danego typu o danym numerze*/
		const AccountID& _temp_id = gkb().findBank(_bank_nr).findAccount(_acc_nr, _acc_t).id();
		/*Wywoluje transfer z AccountID*/
		transfer(_d, _temp_id, _comment);
	} else {
		throw AccountNotFound();
	}
}

bool Account::transfer(double _d, AccountID _to_id, std::string _comment) {
	double _fee = gkb().findBank(_account_id.getBankID()).getTransferFee(_account_id.getType());
	if(_currency != Currency::ENC) {
		/*Jesli moja waluta jest inna niz ENC, to pobieram oplate i konwertuje ja na nasza walute*/
		double _buy_rate = gkb().findBank(_account_id.getBankID()).exchangeTable().getBuyingRate(_currency);
		_fee /= _buy_rate;
	}
	if(_d + _fee > _balance)
		throw NotEnoughMoney();
	else {
		/*Teraz konwertuje kwote na ENC*/
		double _sell_currency = gkb().findBank(_account_id.getBankID()).exchangeTable().getSellingRate(_currency);
		double _d_conv = _d * _sell_currency;
		if(gkb().findBank(_account_id.getBankID()).transferFrom(_d_conv, _account_id, _to_id, _comment)) {
			/*Jesli udalo mi sie przeslac, to zmniejszam moj balans i dodaje do historii operacji dany przelew*/
			_balance -= _d;
			std::string _fro = std::to_string(_account_id.getBankID()) + "-" + std::to_string(_account_id.getNr());
			std::string _to = std::to_string(_to_id.getBankID()) + "-" + std::to_string(_to_id.getNr());
			Transfer _op(interstellarClock().date(), -_d, _currency, OperationType::TRANSFER, _comment, _fro, _to);
			_operation_history.push_back(_op);
			collectTransferFee();
			return true;
		} else
			throw TransferFailed();
	}
}

bool Account::transferTo(double _d, AccountID _from_id, std::string _comment) {
	double _old_d = _d;
	if(_currency != Currency::ENC) {
		double _buy_currency = gkb().findBank(_account_id.getBankID()).exchangeTable().getBuyingRate(_currency);
		_d /= _buy_currency;
	}
	_balance += _d;
	std::string _fro = std::to_string(_from_id.getBankID()) + "-" + std::to_string(_from_id.getNr());
	std::string _to = std::to_string(_account_id.getBankID()) + "-" + std::to_string(_account_id.getNr());
	Transfer _op(interstellarClock().date(), _old_d, Currency::ENC, OperationType::TRANSFER, _comment, _fro, _to);
	_operation_history.push_back(_op);
	return true;
}

std::ostream& operator<<(std::ostream& stream, const Account& _account) {
   stream << "ACCOUNT: : " << _account.id() << std::endl;
   stream << "balance: " << _account.balance() << std::endl;
   stream << _account.history() << std::endl;
   return stream;
}

void Account::collectMonthlyFee() {
	double _fee = gkb().findBank(_account_id.getBankID()).getMonthlyFee(_account_id.getType());
	double _old_fee = _fee;
	if(_currency != Currency::ENC) {
		double _rate = gkb().findBank(_account_id.getBankID()).exchangeTable().getBuyingRate(_currency);
		_fee /= _rate;
	}
	_balance -= _fee;
	Operation _op(interstellarClock().date(), -_old_fee, Currency::ENC, OperationType::CHARGE);
	_operation_history.push_back(_op);
}

void Account::capitalizeInterest() {
	if(_balance > 0) {
		double _interest_rate = gkb().findBank(_account_id.getBankID()).getInterestRate(_account_id.getType());
		_interest_rate /= 100;
		double _interest = _balance * _interest_rate;
		_balance += _interest;
		Operation _op(interstellarClock().date(), _interest, _currency, OperationType::INTEREST);
		_operation_history.push_back(_op);
	}
}

void Account::collectTransferFee() {
	double _fee = gkb().findBank(_account_id.getBankID()).getTransferFee(_account_id.getType());
	double _old_fee = _fee;
	if(_currency != Currency::ENC) {
		double _rate = gkb().findBank(_account_id.getBankID()).exchangeTable().getBuyingRate(_currency);
		_fee /= _rate;
	}
	_balance -= _fee;
	Operation _op(interstellarClock().date(), -_old_fee, Currency::ENC, OperationType::CHARGE);
	_operation_history.push_back(_op);
}

CheckingAccount::CheckingAccount(int _acc_nr, int _bank_id, std::string _owner, AccountType _account_type, Currency _curr): 
	Account(_acc_nr, _bank_id, _owner, _account_type, _curr) { }

bool CheckingAccount::withdraw(double _amount, Currency _curr) {
	if(_curr != _currency) {
		throw WrongCurrency();
	} else {
		if(_amount > _balance) {
			throw NotEnoughMoney();
		} else {
			_balance -= _amount;
			Operation _op(interstellarClock().date(), -_amount, _curr, OperationType::WITHDRAWAL);
			_operation_history.push_back(_op);
			return true;
		}
	}
}

bool CheckingAccount::withdraw(std::pair<double, Currency> _pair) {
	return withdraw(_pair.first, _pair.second);
}

bool CheckingAccount::deposit(double _amount, Currency _curr) {
	if(_curr != _currency) {
		throw WrongCurrency();
	} else {
		_balance += _amount;
		Operation _op(interstellarClock().date(), _amount, _curr, OperationType::DEPOSIT);
		_operation_history.push_back(_op);
		return true;
	}
}

bool CheckingAccount::deposit(std::pair<double, Currency> _pair) {
	return deposit(_pair.first, _pair.second);
}

SavingAccount::SavingAccount(int _acc_nr, int _bank_id, std::string _owner, AccountType _account_type, Currency _curr):
	Account(_acc_nr, _bank_id, _owner, _account_type, _curr) { }

CurrencyAccount::CurrencyAccount(int _acc_nr, int _bank_id, std::string _owner, AccountType _account_type, Currency _curr):
	Account(_acc_nr, _bank_id, _owner, _account_type, _curr) { }

bool CurrencyAccount::withdraw(double _amount) {
	return withdraw(_amount, _currency);
}

bool CurrencyAccount::withdraw(double _amount, Currency _curr) {
	if(_curr != _currency) {
		if(_curr != Currency::ENC) {
			throw WrongCurrency();
		} else {
			double _sell_currency =gkb().findBank(_account_id.getBankID()).exchangeTable().getSellingRate(_currency);
			Operation _op(interstellarClock().date(), -_amount, Currency::ENC, OperationType::WITHDRAWAL);
			_amount /= _sell_currency;
			_balance -= _amount;
			_operation_history.push_back(_op);
			return true;
		}
	} else {
		if(_amount > _balance) {
			throw NotEnoughMoney();
		} else {
			_balance -= _amount;
			Operation _op(interstellarClock().date(), -_amount, _curr, OperationType::WITHDRAWAL);
			_operation_history.push_back(_op);
			return true;
		}
	}
}

bool CurrencyAccount::withdraw(std::pair<double, Currency> _pair) {
	return withdraw(_pair.first, _pair.second);
}

void CurrencyAccount::deposit(double _amount) {
	deposit(_amount, _currency);
}

void CurrencyAccount::deposit(double _amount, Currency _curr) {
	if(_curr != _currency) {
		if(_curr != Currency::ENC) {
			throw WrongCurrency();
		} else {
			double _buy_currency = gkb().findBank(_account_id.getBankID()).exchangeTable().getBuyingRate(_currency);
			Operation _op(interstellarClock().date(), _amount, Currency::ENC, OperationType::DEPOSIT);
			_amount *= _buy_currency;
			_balance += _amount;
			_operation_history.push_back(_op);
		}
	} else {
		_balance += _amount;
		Operation _op(interstellarClock().date(), _amount, _curr, OperationType::DEPOSIT);
		_operation_history.push_back(_op);
	}
}

void CurrencyAccount::deposit(std::pair<double, Currency> _pair) {
	deposit(_pair.first, _pair.second);
}
