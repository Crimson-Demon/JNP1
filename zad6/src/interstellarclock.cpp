#include "interstellarclock.h"
#include "gkb.h"

unsigned long long InterstellarClock::hours() const {
	return _hours;
}

Time InterstellarClock::time() const {
	return _hours % HOURS_IN_DAY;
}

Date InterstellarClock::date() const {
	return _hours / HOURS_IN_DAY;
}

InterstellarClock& InterstellarClock::nextHour() {
	_hours++;
	return *this;
}

InterstellarClock& InterstellarClock::nextDay() {
	for (Time i = 0; i < HOURS_IN_DAY; i++)
		nextHour();
	return *this;
}

InterstellarClock& InterstellarClock::nextMonth() {
	for (Date i = 0; i < DAYS_IN_MONTH; i++)
		nextDay();
	return *this;
}

void InterstellarClock::checkNotify() {
	if (date() % DAYS_IN_MONTH == 0 && time() == 0)
		gkb().notifyBanks();
}

InterstellarClock & interstellarClock() {
	static InterstellarClock c;
	return c;
}
