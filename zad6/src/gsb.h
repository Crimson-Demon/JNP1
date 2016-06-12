#ifndef _GSB_H_
#define _GSB_H_

/*Zawiera naglowki i implementacje rozszerzonych klas od std::exception
  sluzacych do obslugiwania bledow biznesowych i planetarnych
*/
#include "gsbexception.h"
/*Udostepnia deklaracje zegara miedzygwiezdnego*/
#include "interstellarclock.h"
/*Udostepnia klase konto konta, strukture identyfikatora konta,
  enumerator typow walut, enumerator typow kont, enumerator i klase typow operacji
*/
#include "account.h"
/*Udostepnia klase odpowiadajaca kursowi sprzedazy\kupna dla pojedynczej waluty*/
#include "exchangerate.h"
/*Udostepnia klase odpowiadajaca tabeli kursow sprzedazy/kupna dla wielu walut*/
#include "exchangetable.h"
/*Udostepnia klase trzymajaca bank*/
#include "bank.h"
/*Udostepnia klase trzymajaca builder Banku*/
#include "bankbuilder.h"
/*Udostepnia klase odpowiadajaca GKB*/
#include "gkb.h"
/*Udostepnia klasy zwiazane z mieszkancami*/
#include "citizens.h"
/*Udostepnia klase planet*/
#include "planets.h"

#endif 
/* _GSB_H */