#ifndef __gsb__planets__
#define __gsb__planets__

#include "citizens.h"
#include <map>
#include <string>
#include <memory>

class Planet {
public:
	Planet();
	// Nowe planety powinny byæ tworzone tylko przy u¿yciu konstruktora Planet()
	Planet(Planet&) = delete;
	Planet(Planet&&) = delete;
	virtual ~Planet() {};
	// Rejestruje nowego obywatela
	virtual Citizen & registerCitizen(const std::string name) = 0;
	// Wyszukuje i zwraca obywatela na podstawie identyfikatora
	virtual Citizen & findCitizen(const std::string id) const;
protected:
	// Zwraca wolny identyfikator
	std::string getFreeCitizenId();
	// Liczba u¿yta do utworzenia ostatniego identyfikatora
	unsigned long long lastCitizenId;
	// Mapa ID -> obywatel
	std::map<std::string, std::shared_ptr<Citizen>> citizens;
};


class Earth : public Planet {
public:
	virtual Human & registerCitizen(const std::string name);
};

class Qonos : public Planet {
public:
	virtual Klingon & registerCitizen(const std::string name);
};

class Bynaus : public Planet {
public:
	// Rejestruje pojedynczego Binara
	virtual SingleBinar & registerCitizen(const std::string name);
	// Rejestruje podwójnego Binara
	virtual CompositeBinar & registerCitizen(const Binar & a, const Binar & b);
};

Earth & earth();
Qonos & qonos();
Bynaus & bynaus();

#endif
