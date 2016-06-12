#ifndef __gsb__interstellar_clock__
#define __gsb__interstellar_clock__

using Date = unsigned long long;
using Time = unsigned short;

// Czas mi�dzyplanetarny liczymy z dok�adno�ci� do godziny.
// Nowoutworzony zegar mi�dzyplanetarny b�dzie wskazywa� czas 0 i dzie� 0.
class InterstellarClock {
public:
	// Doba mi�dzyplanetarna ma 20 godzin (0-19).
	static const unsigned HOURS_IN_DAY = 20;

	// Miesi�c mi�dzyplanetarny ma 30 dni.
	static const unsigned DAYS_IN_MONTH = 30;

	virtual ~InterstellarClock() {}

	// Zwraca liczb� godzin od pocz�tku za�o�enia ZFP.
	unsigned long long hours() const;

	// Zwraca bie��c� godzin� (0-19).
	Time time() const;

	// Zwraca bie��cy dat�, tj. dzie� od pocz�tku za�o�enia ZFP.
	Date date() const;

	// Przesuwa zegar o godzin�.
	virtual InterstellarClock& nextHour();

	// Przesuwa zegar o dob�.
	virtual InterstellarClock& nextDay();

	// Przesuwa zegar o miesi�c (miesi�c ma 30 dni).
	virtual InterstellarClock& nextMonth();

private:
	// liczba godzin od pocz�tku za�o�enia ZFP
	unsigned long long _hours;
	// Sprawdza czy jest godzina 0 pierwszego dnia miesi�ca i je�li tak,
	// informuje o tym banki
	void checkNotify();
};

InterstellarClock & interstellarClock();

#endif /* defined(__gsb__interstellar_clock__) */
