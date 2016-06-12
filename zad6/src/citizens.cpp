#include "citizens.h"

Citizen::Citizen(std::string name, std::string id) : id_(id), name_(name) {};

std::string Citizen::id() const {
	return id_;
}

std::string Citizen::name() const {
	return name_;
}

CompositeBinar::CompositeBinar(const Binar & a, const Binar & b, const std::string id) :
	Binar(a.name() + "&" + b.name(), id),
	a_(a), b_(b) {}

std::string CompositeBinar::name() const {
	return a_.name() + "&" + b_.name();
}

std::string SingleBinar::name() const {
	return Citizen::name();
}
