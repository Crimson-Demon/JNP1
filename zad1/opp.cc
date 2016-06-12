#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <tuple>
#include <boost/regex.hpp>
#include <set>
#include <map>

//Authors: Marcin Kania, Mateusz Macias, Stas Morawski

using namespace std;
//Funkcja s³u¿y do wczytania standardu iso z pliku isowaluty.txt
int wczytaj(set<string>& iso){
	fstream plik;
	plik.open( "isowaluty.txt", ios::in );
	if(plik.good()){//Jeœli otworzyliœmy plik, wczytuje dane z pliku
		while(!plik.eof()){
			string s;
			getline(plik,s);
			if(boost::regex_match(s,boost::regex("[A-Z]{3}"))){
				iso.insert(s);
			}
		}
		plik.close();
	}else{//Jeœli nie uda sie otworzyæ pliku wczytuje w hard-kodowane dane aktualne na dzieñ 2015-10-18
		string arrayOfISO[178] = {"AED", "AFN", "ALL", "AMD", "ANG", "AOA", "ARS", "AUD", "AWG", "AZN", "BAM", "BBD", "BDT", "BGN", "BHD", "BIF", "BMD", "BND", "BOB", "BOV", "BRL", "BSD", "BTN", "BWP", "BYR", "BZD", "CAD", "CDF", "CHE", "CHF", "CHW", "CLF", "CLP", "CNY", "COP", "COU", "CRC", "CUC", "CUP", "CVE", "CZK", "DJF", "DKK", "DOP", "DZD", "EGP", "ERN", "ETB", "EUR", "FJD", "FKP", "GBP", "GEL", "GHS", "GIP", "GMD", "GNF", "GTQ", "GYD", "HKD", "HNL", "HRK", "HTG", "HUF", "IDR", "ILS", "INR", "IQD", "IRR", "ISK", "JMD", "JOD", "JPY", "KES", "KGS", "KHR", "KMF", "KPW", "KRW", "KWD", "KYD", "KZT", "LAK", "LBP", "LKR", "LRD", "LSL", "LYD", "MAD", "MDL", "MGA", "MKD", "MMK", "MNT", "MOP", "MRO", "MUR", "MVR", "MWK", "MXN", "MXV", "MYR", "MZN", "NAD", "NGN", "NIO", "NOK", "NPR", "NZD", "OMR", "PAB", "PEN", "PGK", "PHP", "PKR", "PLN", "PYG", "QAR", "RON", "RSD", "RUB", "RWF", "SAR", "SBD", "SCR", "SDG", "SEK", "SGD", "SHP", "SLL", "SOS", "SRD", "SSP", "STD", "SYP", "SZL", "THB", "TJS", "TMT", "TND", "TOP", "TRY", "TTD", "TWD", "TZS", "UAH", "UGX", "USD", "USN", "USS", "UYI", "UYU", "UZS", "VEF", "VND", "VUV", "WST", "XAF", "XAG", "XAU", "XBA", "XBB", "XBC", "XBD", "XCD", "XDR", "XFU", "XOF", "XPD", "XPF", "XPT", "XSU", "XTS", "XUA", "XXX", "YER", "ZAR", "ZMW"};
		set<string> iso2(arrayOfISO,arrayOfISO+178);
		iso=iso2;
	};
}

//Funkcja oceniaj¹ca czy dana linia jest poprawnym inputem kursu.
//Przyjmuje input postaci: 3 du¿e litery, dowolny ci¹g znaków bia³ych w tym przynajmniej jedna spacja, po czym liczbê w formacie z lub bez przecinka(np. 5 albo 1,23).
bool czyKurs(string doObrobki){
	return(boost::regex_match(doObrobki,boost::regex("\\s*[A-Z]{3}\\s*([0-9]{1,10},[0-9]{1,3}|[0-9]{1,10})\\s*")));
}

//Funkcja oceniaj¹ca czy dana linia jest poprawnym inputem darczyñcy.
//Przyjmuje input postaci: Imie lub imie i pierwsza litera nazwiska (ci¹g liter z byc moze spacj¹ przed ostatnia z nich), liczbê w formacie bez lub z przecinkiem,
// i ci¹g trzech du¿ych liter. Wszystkie czynniki musz¹ byæ rozdzielone ci¹giem znaków bia³ych zawieraj¹cym co najmniej jedn¹ spacje.
bool czyDarczynca(string doObrobki){
	return(boost::regex_match(doObrobki,boost::regex("\\s*(.+)\\s+([0-9]{1,10},[0-9]{1,3}|[0-9]{1,10})\\s*[A-Z]{3}\\s*")));
}

//Funkcja oceniaj¹ca czy dana linia jest poprawnym inputem zapytania.
////przyjmuje input postaci: dwie liczby w formacie bez lub z przecinkiem, rozdzielone ci¹giem znaków bia³ych zawieraj¹cym co najmniej jedn¹ spacje.
bool czyZapytanie(string doObrobki){
	return(boost::regex_match(doObrobki,boost::regex("\\s*([0-9]{1,10},[0-9]{1,3}|[0-9]{1,10})\\s*([0-9]{1,10},[0-9]{1,3}|[0-9]{1,10})\\s*")));
}

//Funkcja która dla stringa bêd¹cego poprawn¹ deklaracj¹ darczyñcy tworzy tuplê reprezentuj¹c¹ tego darczyñce
tuple<int,string, string, string, string> dajDarczynce(string doObrobki,int numerLinii){
	boost::smatch wynik;
	boost::regex regexDarczyncy("\\s*(.+)\\s+([0-9]+,[0-9]+|[0-9]+)\\s*([A-Z]{3})\\s*");
	boost::regex_search(doObrobki,wynik,regexDarczyncy);
	return tuple<int,string, string, string, string>(numerLinii, wynik[1], wynik[2], wynik[3], wynik[0]);
};
//Funkcja która dla stringa bêd¹cego poprawnym zapytaniem tworzy tuplê reprezentuj¹c¹ to zapytanie
pair<string, string> dajZapytanie(string doObrobki){
	boost::smatch wynik;
	boost::regex regexDarczyncy("\\s*([0-9]+,[0-9]+|[0-9]+)\\s*([0-9]+,[0-9]+|[0-9]+)\\s*");
	boost::regex_search(doObrobki,wynik,regexDarczyncy);
	return pair<string,string>(wynik[1], wynik[2]);
};
//Funkcja która dla stringa bêd¹cego poprawn¹ deklaracj¹ kursu tworzy tuplê reprezentuj¹c¹ ten¿e kurs.
pair<string,string> dajKurs(string doObrobki){
	boost::smatch wynik;
	boost::regex regexDarczyncy("\\s*([A-Z]{3})\\s*([0-9]+,[0-9]+|[0-9]+)\\s*");
	boost::regex_search(doObrobki,wynik,regexDarczyncy);
	return pair<string,string>(wynik[1], wynik[2]);	
};
///////////////////////////////////copy pasta

#define FOREA(i,c) for ( __typeof((c).begin()) i=(c).begin(); i!=(c).end(); i++)

//MODU£ ARYTMETYCZNY NA STRINGACH
//liczby sa reprezentowane w postaci dziesietnej jako teskt

//dodaj(x) zwraca liczbe wieksz¹ o 1 od x na najmniej znacz¹cej pozycji
//np "0,0000003" => "0,0000004"

string dodaj (string a){
	if (a.empty()) return "1";
	else if (a[a.size()-1]==','){
		a.pop_back();
		return dodaj(a)+",";
		}
	else if (a[a.size()-1]-'0'<9){
		a[a.size()-1]++;
		return a;
		}
	else{
		a.pop_back();
		return dodaj(a)+"0";
		}
	}

//ilePoPrzecinku(x) zwraca liczbe cyfr po przecinku liczby x
//np "3520,002" => 3

int ilePoPrzecinku(string a){
	int poz1=0;
	for (int i=a.size()-1;i>=0;i--)
		if(a[i]==',')
			poz1=a.size()-1-i;
	return poz1;
	}

//uzgodnij(x) wymusza zeby liczba x miala przecinek
//np "1234" => "1234,"

string uzgodnij(string a){
	bool dodaj=1;
	for (int i=0;i<a.size();i++)
		if (a[i]==',') dodaj=0;
	if (dodaj)
		a=a+",";
	return a;
	}
//mnoz(x,y) zwraca wynik mno¿enia x*y
//np "32,1"  "10," => "321,0"

string mnoz(string licz1, string licz2)
{
	licz1=uzgodnij(licz1);
	licz2=uzgodnij(licz2);
	stringstream kon;
	int poz1=ilePoPrzecinku(licz1),poz2=ilePoPrzecinku(licz2);
	for ( int i=licz1.size()-1; i>=0; i--)
		if(licz1[i]!=',')
			kon << licz1[i];
	licz1 = kon.str();
	kon.str("");
	for (int i=licz2.size()-1; i>=0; i--) 
		if (licz2[i]!=',')
			kon << licz2[i];
	licz2 = kon.str();
	int przen=licz1.size()+licz2.size();
	if(((char)licz1[0]-'0')*((char)licz2[0]-'0')>9) przen++;
	else przen--;
	int Tab[przen+1];
	for(int i=0; i<=przen; i++) Tab[i]=0;
	int i, j, suma;
	for (i = 0; i < licz1.size(); i++)
		for (j = 0; j < licz2.size(); j++){
			 suma = (licz1[i] - '0')*(licz2[j] - '0');
			 Tab[j+i+1]= Tab[j+i+1] + suma / 10;
			 Tab[j+i] =Tab[j+i] + suma % 10;
			 Tab[j+i+1] =Tab[j+i+1] + Tab[j+i] / 10;
			 Tab[j+i] = Tab[j+i]%10;
			 }
	int x=i+j-1;
	while (Tab[x]==0) x--;
	string wyn="";
	while(x>=0){
		wyn=wyn+(char)(Tab[x]+'0');
		x--;
		}
	if (wyn=="")
		wyn="0";
	while(wyn.size()<poz1+poz2)
		wyn="0"+wyn;
	wyn.insert(wyn.size()-poz1-poz2,",");
	return wyn;
	}

//zaokr(x,poz) przyjmuje liczbe x do zaokraglenia wraz z iloscia cyfr po przecinku x
//zwraca zaokraglona liczbe x postaci Liczba_calkowita + ',' + trzy_cyfry 

string zaokr(string a, int poz){
	if (poz<=3){
		while(poz<3){
			a=a+'0';
			poz++;
			}
		return a;
		}
	int znak=a.size()-poz+3;
	bool cosZaZnakiem=0;
	for (int i=a.size()-1; i>znak; i--)
		if (a[i]!='0'){
			cosZaZnakiem=1;
			break;
			}
	while(znak+1<a.size())
		a.pop_back();
	if (a[znak]-'0'>5 or (a[znak]-'0'==5 and ((a[znak-1]-'0')%2==1 or cosZaZnakiem))){
		a.pop_back();
		a=dodaj(a);
		return a;
		}
	a.pop_back();
	return a;
	}

//porownajTuple(a,b) sprawdza czy Darczynca a < Darczynca b
//Darczynce uznajemy za mniejszego jesli wplacil mniej lub jesli wplacil tyle samo ale wczesniej

bool porownajTuple(tuple<int, string, string, string, string> a, tuple<int, string, string, string, string> b){
	string k=get<2>(a), l=get<2>(b);	
	k=uzgodnij(k);
	k=mnoz(k,"1,0");
	k=zaokr(k, ilePoPrzecinku(k));
	l=uzgodnij(l);
	l=mnoz(l, "1,0");
	l=zaokr(l, ilePoPrzecinku(l));
	if (k.size()<l.size()) return true;
	if (l.size()<k.size()) return false;
	if (k!=l)
			return (k<l);
	return
			((get<0>(a))<(get<0>(b)));	
	}


//konwertuj przyjmuje liste darczyncow i liste kursów i konwertuje wszystkie wplaty do waluty lokalnej

void konwertuj(vector< tuple<int, string, string, string, string> >& listaDarczyncow, map<string, string> listaKursow){
	//cout<<"flag11";	
	FOREA(i, listaDarczyncow){
		//cout<<"flag 12"<<get<4>(*i)<<endl;		
		string wyn=mnoz(get<2>(*i),listaKursow[get<3>(*i)]);
		get<2>(*i)=zaokr(wyn, ilePoPrzecinku(wyn));
		}
}


//Procedura wypisujaca na standardowe wyjscie bledow bledy
//przyjmuje input postaci: referencja do wektora listy bledow.
void wypiszBledy(vector<pair<int,string> >& listaBledow) {
	for(auto it : listaBledow) {
		cerr << "Error in line " << get<0>(it) << ":"<< get<1>(it) << endl;
	}
}

//Funkcja sluzaca do sortowania wektoru bledow.
//Jako argumenty pobiera referencje do  par zawierajacych int-nr linii oraz string-tresc blednej linii
//Jako kryterium porownania sluzy relacja miedzy nr linii, ktore sa osiagane przez atrybut first par
bool sortujBledy(const pair<int, string>& lewy, const pair<int, string>& prawy) {
    return lewy.first < prawy.first;
}


int main(void){
	vector< tuple<int, string, string, string, string> > listaDarczyncow; //wstepna lista darczyncow, int odpowiada nr linii, string to nazwa darczyncy, float to wplacone pieniadze, string to waluta, string to cala linia
	map<string, string> listaKursow; //klucz to nazwa waluty, float to kurs wzgledem waluty lokalnej
	vector<pair<string, string> > listaPolecen; //albo moze lepiej mape, z kluczem - nr linii int?
	vector<pair<int,string> > listaBledow; //lista bledow, int to nr linii, string to komenda, ktora wywolala blad
	set<string> iso;
	//Wcyztujemy wpierw iso, albo z pliku, jest jest w katalogu, albo na brudno z tablicy wpisanej z palca w kodzie
	wczytaj(iso);
	int numerLinii = 0;
	int tryb = 0;
	string linia;
	do{
		getline(cin, linia);
		numerLinii++;
		if(czyKurs(linia) && tryb == 0){
			//cerr<<"kurs: "<<linia<<endl;
			pair<string, string> nowyKurs = dajKurs(linia);
			if(iso.find(nowyKurs.first) != iso.end() && listaKursow.find(nowyKurs.first) == listaKursow.end() && nowyKurs.second != "0") { //jesli kurs jest w liscie iso
				listaKursow[nowyKurs.first] = nowyKurs.second;
			} else { //jesli kurs nie jest w liscie iso, to blad
				pair<int,string> nowaPara = make_pair(numerLinii, linia);
				listaBledow.push_back(nowaPara);
			}
		} else if(czyDarczynca(linia) && tryb <= 1){
			//cerr<<"darczynca: "<<linia<<endl;
			tuple<int, string, string, string, string> nowyDarczynca = dajDarczynce(linia, numerLinii);
			listaDarczyncow.push_back(nowyDarczynca);
			tryb=1;
		} else if(czyZapytanie(linia)){
			//cerr<<"zapytanie: "<<linia<<endl;
			tryb = 2;
            pair<string, string> noweZapytanie = dajZapytanie(linia);
            if(atoi(noweZapytanie.first.c_str()) > atoi(noweZapytanie.second.c_str())) { //jesli pierwsza liczba jest niepoprawnie wieksza od drugiej
    			pair<int,string> nowaPara = make_pair(numerLinii, linia);
                listaBledow.push_back(nowaPara);
            } else { //jesli jest poprawnie sformatowane i pierwsza liczba jest niewieksza od drugiej
	            listaPolecen.push_back(noweZapytanie);
            }
		} else {
		if(linia.length()>0){
			//cerr<<"blad: "<<linia<<endl;
			pair<int,string> nowaPara = make_pair(numerLinii, linia);
				listaBledow.push_back(nowaPara);
			}
		}
		
	}while(!cin.eof());
	
        for(int i = 0; i < listaDarczyncow.size(); i++) { //poprawiamy lista darczyncow, wyrzucajac tych, ktorych kursy nie leza w liscie iso
            if(listaKursow.find(get<3>(listaDarczyncow.at(i))) == listaKursow.end()){ //jesli kurs darczyncy nie lezy w liscie kursow
                pair<int, string> nowaPara = make_pair(get<0>(listaDarczyncow.at(i)), get<4>(listaDarczyncow.at(i)));
                listaBledow.push_back(nowaPara);
                listaDarczyncow.erase(listaDarczyncow.begin() + i);
                i--;
            }
        }
		
	////////////////////////////////////////////tu idzie copy pasta
		//przygotowywujemy liste darczyncow do efektywnego odpowiadania na zapytania
	konwertuj(listaDarczyncow, listaKursow);
	sort(listaDarczyncow.begin(),listaDarczyncow.end(), porownajTuple);
	//if(listaDarczyncow.begin()==listaDarczyncow.end()) cerr<<"nic nie ma"; else cerr<<"cos jest"<<endl;
	FOREA(i,listaDarczyncow)
		get<2>(*i)=zaokr(get<2>(*i),ilePoPrzecinku(get<2>(*i)));
	//listaDarczyncow jest juz poprawnie sformatowana i przygotowana do odpowiadania na zapytania
	//Odpowiadanie na zapytania:
	FOREA(i,listaPolecen)
		if (atoi((*i).second.c_str())<atoi((*i).first.c_str())){
			//cerr<<"tu jest"<<(*i).second<<" "<<(*i).first<<endl;
			continue;	//jesli pocztek przedzialu jest wiekszy od koniec to przedzial jest pusty
		}else{
			//cerr <<"tu jest"<<endl;
			//Tworzymy z zapytan Tuple zeby mozna je porownywac z elementami listyDarczyncow
			tuple<int, string, string, string, string> k1,k2;
			(*i).first=uzgodnij((*i).first);
			(*i).second=uzgodnij((*i).second);
			get<2>(k1)=zaokr((*i).first,ilePoPrzecinku((*i).first));
			get<2>(k2)=zaokr((*i).second,ilePoPrzecinku((*i).second));
			get<0>(k2)=1<<30;
			//za pomoc¹ wyszukiwania binarnego znajdujemy pocz¹tek i koniec przedzia³u bêd¹cego odpowiedzi¹
			vector< tuple<int, string, string, string, string> >::iterator low, up;
			low=lower_bound(listaDarczyncow.begin(),listaDarczyncow.end(),k1,porownajTuple);
			up=upper_bound(listaDarczyncow.begin(),listaDarczyncow.end(),k2,porownajTuple);
			//if(low==up) cerr<<"ten sam"; else cerr<<"inny";
			//wypisujemy w poprawnym formacie ka¿dy element z znalezionego przedzia³u
			for (vector< tuple<int, string, string, string, string> >::iterator it=low; it!=up;it++){
				tuple<int, string, string, string, string> wyn=dajDarczynce(get<4>(*it),get<0>(*it));
				string nowy=get<2>(wyn);
				nowy=uzgodnij(nowy);
				nowy=mnoz(nowy,"1,0");
				nowy=zaokr(nowy, ilePoPrzecinku(nowy));
				if (nowy[0]==',')
						nowy="0"+nowy;
				get<2>(wyn)=nowy;
				get<1>(wyn).erase(get<1>(wyn).find_last_not_of(" \n\r\t")+1);
				cout<<"\""<<get<1>(wyn)<<"\",\""<<get<2>(wyn)<<"\","<<get<3>(wyn)<<endl;			
				}
			}
		
sort(listaBledow.begin(), listaBledow.end(), sortujBledy); //sortujemy liste bledow zgodnie z numerem linii
	wypiszBledy(listaBledow); //wypisujemy liste bledow
	
	return 0;
}