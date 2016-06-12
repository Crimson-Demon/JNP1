#include "gkb.h"
#include "gsbexception.h"
#include <memory>

GKB::GKB() {
	_bank_count = 0;
}

GKB::~GKB() { }

BankBuilder& GKB::bankApplication() {
	static BankBuilder _bank_app;
	return _bank_app;
}

void GKB::notifyBanks() {
	for(auto i: _bank_list) {
		(*i.second).capitalizeInterest();
		(*i.second).collectMonthlyFee();
	}
}

Bank& GKB::addBank(const std::string _name, const BankParameters _bank_parameters) {
	int _bank_id = ++_bank_count;
	_bank_list[_bank_id] = std::make_shared<Bank>(_bank_id, _name, _bank_parameters);
	return *_bank_list[_bank_id];
}

Bank& GKB::findBank(int _id) const {
	if(_bank_list.find(_id) != _bank_list.end())
		return *_bank_list.at(_id);
	else
		throw BankNotFound();
}

GKB & gkb() {
	static GKB _gkb;
	return _gkb;
}
