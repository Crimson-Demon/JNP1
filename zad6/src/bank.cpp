#include "gkb.h"
#include "gsbexception.h"
#include "bank.h"

Bank::Bank(const int _id, const std::string _s) :
	_name(_s), _id(_id) {}

Bank::Bank(const int _id, const std::string _s, const BankParameters& _b_param) :
	_name(_s), _id(_id), _bank_parameters(_b_param) {}

Bank::~Bank() { };

ExchangeTable& Bank::exchangeTable() {
	return _exchange_table;
}

void Bank::capitalizeInterest() {
	for(auto _acc_gr: _account_lists) {
		for(auto _acc_id: _acc_gr.second) {
			_acc_id.second->capitalizeInterest();
		}
	}
}

void Bank::collectMonthlyFee() {
	for(auto _acc_gr: _account_lists) {
		for(auto _acc_id: _acc_gr.second) {
			_acc_id.second->collectMonthlyFee();
		}
	}
}

void Bank::collectTransferFee(AccountID _acc_id) {
	findAccount(_acc_id).collectTransferFee();
}

CheckingAccount& Bank::openCheckingAccount(const Citizen& _citizen) {
	int _acc_nr = _account_lists[AccountType::CHECK].size() + 1;
	auto _account = std::make_shared<CheckingAccount>(_acc_nr, _id, _citizen.id(),
	                                                  AccountType::CHECK, Currency::ENC);
	_account_lists[AccountType::CHECK].insert(std::make_pair(_acc_nr, _account));
	return *_account;
}

SavingAccount& Bank::openSavingAccount(const Citizen& _citizen) {
	int _acc_nr = _account_lists[AccountType::SAVE].size() + 1;
	auto _account = std::make_shared<SavingAccount>(_acc_nr, _id, _citizen.id(),
	                                                AccountType::SAVE, Currency::ENC);
	_account_lists[AccountType::SAVE].insert(std::make_pair(_acc_nr, _account));
	return *_account;
}

CurrencyAccount& Bank::openCurrencyAccount(const Citizen& _citizen, Currency _curr) {
	int _acc_nr = _account_lists[AccountType::CURR].size() + 1;
	auto _account = std::make_shared<CurrencyAccount>(_acc_nr, _id, _citizen.id(),
	                                                  AccountType::CURR, _curr);
	_account_lists[AccountType::CURR].insert(std::make_pair(_acc_nr, _account));
	return *_account;
}

bool Bank::transferFrom(const double _d, const AccountID _from_id , const AccountID _to_id,
                        const std::string _comment) {
	return gkb().findBank(_to_id.getBankID()).transferTo(_d, _from_id, _to_id, _comment);
}

bool Bank::transferTo(const double _d, const AccountID _from_id , const AccountID _to_id,
                      const std::string _comment) {
	return this->findAccount(_to_id).transferTo(_d, _from_id, _comment);
}

Account& Bank::findAccount(const AccountID _acc_id) const {
	auto list = _account_lists.find(_acc_id.getType())->second;
	auto acc_it = list.find(_acc_id.getNr());
	if(acc_it != list.end()) {
		return *acc_it->second;
	} else {
		throw AccountNotFound();
	}
}

Account& Bank::findAccount(int _acc_nr, AccountType _acc_type) const {
	auto list = _account_lists.find(_acc_type)->second;
	auto acc_it = list.find(_acc_nr);
	if(acc_it != list.end()) {
		return *acc_it->second;
	} else {
		throw AccountNotFound();
	}
}

double Bank::getTransferFee(const AccountType _acc_t) const {
	return _bank_parameters.at(_acc_t)[1];
}

double Bank::getMonthlyFee(const AccountType _acc_t) const {
	return _bank_parameters.at(_acc_t)[0];
}

double Bank::getInterestRate(const AccountType _acc_t) const {
	return _bank_parameters.at(_acc_t)[2];
}
