#ifndef _EXCEPT_H_
#define _EXCEPT_H_

#include <exception>

class BusinessException: public std::exception {
    public:

        virtual char const* what() const throw() {
                return "Buisiness error encountered!";
        };
};

class CurrencyNotFound: public BusinessException {
    public:

        virtual char const* what() const throw() {
                return "Non-existant currency!";
        };
};

class WrongCurrency: public BusinessException {
    public:

        virtual char const* what() const throw() {
                return "Transaction not allowed in this currency!";
        };
};

class BankNotFound: public BusinessException {
public:

        virtual char const* what() const throw() {
                return "Bank not found!";
        };
};

class TransferFailed: public BusinessException {
    public:

        virtual char const* what() const throw() {
                return "Transfer failed due to suspicious circumstances!";
        };
};

class AccountNotFound: public BusinessException {
    public:

        virtual char const* what() const throw() {
                return "Unknown account number!";
        };
};

class NotEnoughMoney: public BusinessException {
    public:

        virtual char const* what() const throw() {
                return "Not enough money!";
        };
};

class PlanetaryException: public std::exception {
    public:

        virtual char const* what() const throw() {
                return "Planetary error encountered!";
        };
};

class InhabitantNotFound: public PlanetaryException {
    public:

        virtual char const* what() const throw() {
                return "Inhabitant not found!";
        };
};

#endif 
/* _EXCEPT_H */