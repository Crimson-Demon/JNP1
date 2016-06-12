#ifndef __gsb__citizens__
#define __gsb__citizens__

#include <string>

class Citizen {
public:
	// Obywatele nie powinni byæ kopiowani ani przenoszeni
	Citizen(Citizen&) = delete;
	Citizen(Citizen&&) = delete;
	// Tworzy nowego obywatela
	Citizen(const std::string name, const std::string id);
	virtual ~Citizen() {};
	// Zwraca identyfikator obywatela
	virtual std::string id() const;
	// Zwraca imiê obywatela
	virtual std::string name() const;

private:
	const std::string id_, name_;
};

class Human : public Citizen {
public:
	using Citizen::Citizen;
};

class Klingon : public Citizen {
public:
	using Citizen::Citizen;
};

class Binar : public Citizen {
public:
	using Citizen::Citizen;
	virtual std::string name() const = 0;
};

class SingleBinar : public Binar {
public:
	using Binar::Binar;
	virtual std::string name() const;
};

class CompositeBinar : public Binar {
public:
	// Tworzy nowego podwójnego Binara z dwóch innych Binarów
	CompositeBinar(const Binar & a, const Binar & b, const std::string id);
	virtual std::string name() const;
private:
	const Binar & a_, & b_;
};

#endif
