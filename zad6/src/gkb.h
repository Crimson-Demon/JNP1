#ifndef _GKB_H_
#define _GKB_H_

#include "account.h"
#include "bank.h"
#include "bankbuilder.h"
#include <map>
#include <memory>
#include <utility>

/*Klasa GKB sluzy do trzymania obiektu typu komitet bankowy
  przechowuje w sobie liste bankow i zapamietuje ich aktualna liczbe
  Banki sa trzymane za pomoca shared_ptr, aby zewnetrzne referencje odnosily sie
  do tych samuych obiektow, ktore trzyma GKB
  GKB faktycznie i fizycznie tworzy bank - BankBuilder jedynie w tym pomaga.
*/
/*W polaczeniu z Interstellarclock realizuje wzorzec Observer
  Interstellarclock na poczatku miesiaca o godzinie 0
  budzi gkb i gkb potem budzi wszystkie banki po kolei
*/
class GKB {
	int _bank_count;
	std::map<int, std::shared_ptr<Bank>> _bank_list;
	public:
		GKB();
		GKB(GKB&) = delete;
		GKB(GKB&&) = delete;
		~GKB();
		/*Zwraca statyczny obiekt typu BankBuilder - budowniczy bankow z fluent interfacem*/
		BankBuilder& bankApplication();
		/*Metoda budzi kazdy bank po kolei, aby pobrac miesieczna oplate i naliczyc odsetki*/
		void notifyBanks();
		/*Dodaje bank o danej nazwie i danych parametrach*/
		Bank& addBank(const std::string _name, const BankParameters _bank_parameters);
		/*Wyszukuje bank po podanym id*/
		Bank& findBank(int _id) const;
};

/*Metoda inicjalizuje, jesli nie istnieje jeszcze
  i zwraca statyczny obiekt typu GKB
  W koncu istnieje tylko jeden komitet bankowy
*/
GKB & gkb();

#endif