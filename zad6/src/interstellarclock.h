#ifndef __gsb__interstellar_clock__
#define __gsb__interstellar_clock__

using Date = unsigned long long;
using Time = unsigned short;

// Czas miêdzyplanetarny liczymy z dok³adnoœci¹ do godziny.
// Nowoutworzony zegar miêdzyplanetarny bêdzie wskazywa³ czas 0 i dzieñ 0.
class InterstellarClock {
public:
	// Doba miêdzyplanetarna ma 20 godzin (0-19).
	static const unsigned HOURS_IN_DAY = 20;

	// Miesi¹c miêdzyplanetarny ma 30 dni.
	static const unsigned DAYS_IN_MONTH = 30;

	virtual ~InterstellarClock() {}

	// Zwraca liczbê godzin od pocz¹tku za³o¿enia ZFP.
	unsigned long long hours() const;

	// Zwraca bie¿¹c¹ godzinê (0-19).
	Time time() const;

	// Zwraca bie¿¹cy datê, tj. dzieñ od pocz¹tku za³o¿enia ZFP.
	Date date() const;

	// Przesuwa zegar o godzinê.
	virtual InterstellarClock& nextHour();

	// Przesuwa zegar o dobê.
	virtual InterstellarClock& nextDay();

	// Przesuwa zegar o miesi¹c (miesi¹c ma 30 dni).
	virtual InterstellarClock& nextMonth();

private:
	// liczba godzin od pocz¹tku za³o¿enia ZFP
	unsigned long long _hours;
	// Sprawdza czy jest godzina 0 pierwszego dnia miesi¹ca i jeœli tak,
	// informuje o tym banki
	void checkNotify();
};

InterstellarClock & interstellarClock();

#endif /* defined(__gsb__interstellar_clock__) */
