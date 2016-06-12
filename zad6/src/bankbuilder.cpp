#include "account.h"
#include "bankbuilder.h"
#include "gkb.h"

BankBuilder::BankBuilder() :
	_name("Bank"), _mod_account_type(AccountType::NONE)
{
	std::vector<double> vec(3, 0.00);
	_bank_parameters[AccountType::CHECK] = vec;
	_bank_parameters[AccountType::SAVE] = vec;
	_bank_parameters[AccountType::CURR] = vec;
}

BankBuilder::~BankBuilder() { }

BankBuilder& BankBuilder::name(const std::string _s) {
	_name = _s;
	return *this;
}

BankBuilder& BankBuilder::checkingAccount() {
	_mod_account_type = AccountType::CHECK;
	return *this;
}

BankBuilder& BankBuilder::savingAccount() {
	_mod_account_type = AccountType::SAVE;
	return *this;
}

BankBuilder& BankBuilder::currencyAccount() {
	_mod_account_type = AccountType::CURR;
	return *this;
}

BankBuilder& BankBuilder::monthlyCharge(double _d) {
	if(_mod_account_type != AccountType::NONE)
		_bank_parameters[_mod_account_type][0] = _d;
	else {
		for(auto i: _bank_parameters) {
			i.second[0] = _d;
		}
	}
	return *this;
}

BankBuilder& BankBuilder::transferCharge(double _d) {
	if(_mod_account_type != AccountType::NONE)
		_bank_parameters[_mod_account_type][1] = _d;
	else {
		for(auto i: _bank_parameters) {
			i.second[1] = _d;
		}
	}
	return *this;
}

BankBuilder& BankBuilder::interestRate(double _d) {
	if(_mod_account_type != AccountType::NONE)
		_bank_parameters[_mod_account_type][2] = _d;
	else {
		for(auto i: _bank_parameters) {
			i.second[2] = _d;
		}
	}
	return *this;
}

void BankBuilder::resetParameters() {
	std::vector<double> vec(3, 0.00);
	_bank_parameters[AccountType::CHECK] = vec;
	_bank_parameters[AccountType::SAVE] = vec;
	_bank_parameters[AccountType::CURR] = vec;
	_mod_account_type = AccountType::NONE;
	_name = "Bank";
}

Bank& BankBuilder::createBank() {
	std::string _name_temp = _name;
	BankParameters _bank_parameters_temp = _bank_parameters;
	resetParameters();
	return gkb().addBank(_name_temp, _bank_parameters_temp);
}
