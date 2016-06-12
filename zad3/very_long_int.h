//Authors: Marcin Kania, Mateusz Macias

#include <iostream>

class VeryLongInt;

// Functions Zero() and Nan() return global unmodifiable objects, representing zero and NaN respectively
// giveNaN() returns a VeryLongInt representing NaN. The object is modifiable.
// giveZero() returns a VeryLongInt representing Zero. The object is modifiable.
const VeryLongInt& Zero();
const VeryLongInt& NaN();
VeryLongInt& giveNaN();
VeryLongInt& giveZero();

// Normalize adds leading zeroes to the shorter of strings passed as arguments.
// As a result both numbers are of the same length. The method returns the length after normalization.
// denoramlize() erases unnecesary leading zeroes from string passed as argument.
int normalize(std::string&, std::string&);
int denormalize(std::string&);

//Functions necesary for << and >> operators. 
VeryLongInt fastPower(const VeryLongInt, const VeryLongInt);

//Public overloaded binary arithmetic/bitwise operators.
const VeryLongInt operator +(const VeryLongInt& lhs, const VeryLongInt& rhs);
const VeryLongInt operator -(const VeryLongInt& lhs, const VeryLongInt& rhs);
const VeryLongInt operator *(const VeryLongInt& lhs, const VeryLongInt& rhs);
const VeryLongInt operator /(const VeryLongInt& lhs, const VeryLongInt& rhs);
const VeryLongInt operator %(const VeryLongInt& lhs, const VeryLongInt& rhs);
const VeryLongInt operator <<(const VeryLongInt& lhs, const VeryLongInt& rhs);
const VeryLongInt operator >>(const VeryLongInt& lhs, const VeryLongInt& rhs);

//Public overloaded bool operators
bool operator <(const VeryLongInt& lhs, const VeryLongInt& rhs);
bool operator >(const VeryLongInt& lhs, const VeryLongInt& rhs);
bool operator ==(const VeryLongInt& lhs, const VeryLongInt& rhs);
bool operator !=(const VeryLongInt& lhs, const VeryLongInt& rhs);
bool operator <=(const VeryLongInt& lhs, const VeryLongInt& rhs);
bool operator >=(const VeryLongInt& lhs, const VeryLongInt& rhs);

class VeryLongInt{
   private:
     //Attributes
     std::string val; // String representing the number.
     bool nan; // Object represents NaN <==> nan=true. 
   
     //Methods
     VeryLongInt& div2() const; // Returns result of this div 2.
     bool isEven() const;
     VeryLongInt& timesChar(const char) const; //Multiplication by digit represented by char.
     
     //List of Friend functions
     friend VeryLongInt& giveNaN();
     friend VeryLongInt& giveZero();
     friend VeryLongInt fastPower(const VeryLongInt, const VeryLongInt);

     
   public:
     
      //Constructors
      
      //No-arguments constructor returns zero object
      VeryLongInt();
      
      //Int-based constructors
      VeryLongInt(int);
      explicit VeryLongInt(long);
      explicit VeryLongInt(long long);
      explicit VeryLongInt(unsigned short);
      explicit VeryLongInt(unsigned int);
      explicit VeryLongInt(unsigned long);
      explicit VeryLongInt(unsigned long long);
      //Disabling char and bool construction
      explicit VeryLongInt(char) = delete;
      explicit VeryLongInt(unsigned char) = delete;
      explicit VeryLongInt(bool) = delete;
      
     //String constuctor
      VeryLongInt(std::string);
      VeryLongInt(const char[]);
   
      //Copy constructors
      VeryLongInt(const VeryLongInt&);

      //Move constructor
      VeryLongInt(VeryLongInt&&);

      //Destructor
      ~VeryLongInt();

      //Copy assignment operators
      VeryLongInt& operator =(const VeryLongInt&);
      VeryLongInt& operator =(const int);
     
     //Compound arithmetic/bitwise assignment operators
      VeryLongInt& operator +=(const VeryLongInt&);
      VeryLongInt& operator -=(const VeryLongInt&);
      VeryLongInt& operator *=(const VeryLongInt&);
      VeryLongInt& operator /=(const VeryLongInt&);
      VeryLongInt& operator %=(const VeryLongInt&);
      VeryLongInt& operator <<=(const VeryLongInt&);
      VeryLongInt& operator >>=(const VeryLongInt&);
     
     //Overloading friendly ostream output operator
      friend std::ostream& operator <<(std::ostream&, const VeryLongInt&);

      //Overloading bool operator and
      explicit operator bool() const;
      explicit operator short() const = delete;
      explicit operator int() const = delete;
      explicit operator long() const = delete;
      explicit operator long long() const = delete;
      explicit operator unsigned short() const = delete;
      explicit operator unsigned int() const = delete;
      explicit operator unsigned long() const = delete;
      explicit operator unsigned long long() const = delete;

      //Move assignment operator
      VeryLongInt& operator =(VeryLongInt&&);

      //Method returns the number of digits in the binary representation
      int numberOfBinaryDigits() const;
      
      //isValid method checks if were a valid not-NaN very long integer
      bool isValid() const;

      //Getters and setters
      std::string getVal() const;
      void setVal(std::string);
      bool getNan() const;
      void setNan(bool);
};