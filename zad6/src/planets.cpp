#include "planets.h"
#include "gsbexception.h"

Planet::Planet() : lastCitizenId(0) {}

Citizen & Planet::findCitizen(const std::string id) const {
	auto it = citizens.find(id);
	if (it == citizens.end())
		throw InhabitantNotFound();
	return *it->second;
}

std::string Planet::getFreeCitizenId() {
	return std::to_string(++lastCitizenId);
}

SingleBinar & Bynaus::registerCitizen(const std::string name) {
	auto id = getFreeCitizenId();
	auto c = std::make_shared<SingleBinar>(name, id);
	citizens[id] = c;
	return *c;
}

CompositeBinar & Bynaus::registerCitizen(const Binar & a, const Binar & b) {
	auto id = getFreeCitizenId();
	auto c = std::make_shared<CompositeBinar>(a, b, id);
	citizens[id] = c;
	return *c;
}

Human & Earth::registerCitizen(const std::string name) {
	auto id = getFreeCitizenId();
	auto c = std::make_shared<Human>(name, id);
	citizens[id] = c;
	return *c;
}

Klingon & Qonos::registerCitizen(const std::string name) {
	auto id = getFreeCitizenId();
	auto c = std::make_shared<Klingon>(name, id);
	citizens[id] = c;
	return *c;
}


Earth & earth() {
	static Earth e;
	return e;
}

Qonos & qonos() {
	static Qonos q;
	return q;
}

Bynaus & bynaus() {
	static Bynaus b;
	return b;
}
