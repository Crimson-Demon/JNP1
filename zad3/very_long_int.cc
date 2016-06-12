//Authors: Marcin Kania, Mateusz Macias

#include <iostream>
#include "very_long_int.h"

/**********************GLOBAL OBJECTS***************************/
const VeryLongInt& NaN(){
      static const VeryLongInt result = giveNaN();
      return result;
}

const VeryLongInt& Zero(){
      static const VeryLongInt result("0");
      return result;
   
}

VeryLongInt& giveNaN(){
      VeryLongInt* result = new VeryLongInt("0");
      result->nan=true;
      return *result;
};

VeryLongInt& giveZero() {
      VeryLongInt* result = new VeryLongInt("0");
      return *result;
};

/**********************HELPING FUNCTIONS***************************/
int normalize(std::string& str1, std::string& str2){
      int n = str1.length();
      int m = str2.length();
      int how_many_append = std::max(n,m) - std::min(n,m);
      if(how_many_append!=0){
         
         std::string zeroes(how_many_append,'0');
      
         if(n == std::max(n,m)){
            str2 = zeroes.append(str2);
            return n;
         } else {
            str1 = zeroes.append(str1);
            return m;
         }
         
      }
      
      return n;
}

int denormalize(std::string& str){
   int i = 0;
   int n = str.length();
   while( str[i]=='0' && i < n-1 )i++;
   str = str.substr(i);
   return str.length();
}


VeryLongInt fastPower(const VeryLongInt lowerIndex, const VeryLongInt upperIndex){
   if(upperIndex == Zero()) return 1;
   if( upperIndex.isEven() ){
      VeryLongInt sqrt = fastPower(lowerIndex,upperIndex/2);
      return sqrt * sqrt;
   } else {
      return lowerIndex * fastPower(lowerIndex,upperIndex-1);;
   }
};

/******************************BINARY OPERATORS************************************/

const VeryLongInt operator +(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   VeryLongInt result = lhs;
   result += rhs;
   return result;
}

const VeryLongInt operator -(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   VeryLongInt result = lhs;
   result -= rhs;
   return result;
}

const VeryLongInt operator *(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   VeryLongInt result = lhs;
   result *= rhs;
   return result;
}

const VeryLongInt operator /(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   VeryLongInt result = lhs;
   result /= rhs;
   return result;
}

const VeryLongInt operator %(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   VeryLongInt result = lhs;
   result %= rhs;
   return result;
}

const VeryLongInt operator <<(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   VeryLongInt result = lhs;
   result <<= rhs;
   return result;
}

const VeryLongInt operator >>(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   VeryLongInt result = lhs;
   result >>= rhs;
   return result;
}

/***************************COMPARISON OPERATORS****************************/

bool operator <(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   
   if(lhs.getNan() || rhs.getNan()){
      return false;
   }
   
   std::string num1(lhs.getVal());
   std::string num2(rhs.getVal());
   
   int len = normalize(num1, num2);
   
   for(int i=0; i<len; i++){
      
      if(num1[i] > num2[i]) return false;
      if(num1[i] < num2[i]) return true;
   }
   return false;
}

bool operator >(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   
   if(lhs.getNan() || rhs.getNan()){
      return false;
   }
   
   std::string num1(lhs.getVal());
   std::string num2(rhs.getVal());
   
   int len = normalize(num1, num2);
   
   for(int i=0; i<len; i++){
      if(num1[i] < num2[i]) return false;
      if(num1[i] > num2[i]) return true;
   }
   return false;
}

bool operator ==(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   
   if(lhs.getNan() || rhs.getNan()){
      return false;
   }
   
   std::string num1(lhs.getVal());
   std::string num2(rhs.getVal());
   
   int len = normalize(num1, num2);
   
   for(int i=0; i<len; i++){
      if(num1[i]!=num2[i]) return false;
   }
   return true;
}

bool operator !=(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   if(lhs.getNan() || rhs.getNan()){
      return false;
   }
   return !(lhs == rhs);
}

bool operator <=(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   if(lhs.getNan() || rhs.getNan()){
      return false;
   }
   return ( lhs < rhs ) || ( lhs == rhs );
}

bool operator >=(const VeryLongInt& lhs, const VeryLongInt& rhs) {
   if(lhs.getNan() || rhs.getNan()){
      return false;
   }
   return ( lhs > rhs ) || ( lhs == rhs );
}

/************************PRIVATE METHODS************************************/

//Here we don't check if arguments are NaN
VeryLongInt& VeryLongInt::div2() const{
   
   //We create variable with result
   int len = (this->val).length();
   std::string resultStr(len,0);
   
   int transfer = 0;
   
   //We divide and if necesary do the transfer
   for(int i=0; i<len; i++){
      int digit = (this->val)[i] - '0';
      resultStr[i] = ( (digit / 2) + transfer ) + '0';
      transfer = 5 * ( digit % 2 );
   }
   
   denormalize(resultStr);
   
   return *new VeryLongInt(resultStr);
};

bool VeryLongInt::isEven() const{
   int len = (this->val).length();
   int lastNumber = (this->val)[len-1];
   return (lastNumber % 2) == 0;
}

//Multiplying by digit from char
VeryLongInt& VeryLongInt::timesChar(const char digit) const{
   
   //Creating strings to operate on them
   std::string num1(this->val);
   int len = num1.length();
   int transfer = 0;
   
   //create variable for result
   std::string resultStr(len+1,'0');
   
   //Algorithm like in school
   for(int i=len;i>0;i--){
      int digitFromString = num1[i-1] - '0';
      int digitFromChar = digit - '0';
      int multi = digitFromString * digitFromChar + transfer;
      resultStr[i] = (multi % 10) + '0';
      transfer = multi/10;
   }
   
   //Adding transfer that could be left after last leap
   if(transfer>0){
      resultStr[0] = transfer + '0';
   }
   
   denormalize(resultStr);
   
   VeryLongInt* result = new VeryLongInt(resultStr);
   
   return *result;

}

/************************************CONSTRUCTORS AND DESTRUCTORS**************************************/

VeryLongInt::VeryLongInt() { //must return global zero object?
   val = "0";
   nan = false;
}

VeryLongInt::VeryLongInt(int n) {
   val = std::to_string(n);
   nan = n < 0;
}

VeryLongInt::VeryLongInt(long n) {
   val = std::to_string(n);
   nan = n < 0;
}

VeryLongInt::VeryLongInt(long long n) {
   val = std::to_string(n);
   nan = n < 0;
}

VeryLongInt::VeryLongInt(unsigned short n) {
   val = std::to_string(n);
   nan = false;
}

VeryLongInt::VeryLongInt(unsigned int n) {
   val = std::to_string(n);
   nan = false;
}

VeryLongInt::VeryLongInt(unsigned long n) {
   val = std::to_string(n);
   nan = false;
}

VeryLongInt::VeryLongInt(unsigned long long n) {
   val = std::to_string(n);
   nan = false;
}

VeryLongInt::VeryLongInt(std::string number) {
   val = number; //TODO, trzeba sprawdzic, czy ten string ma sens
   nan = false; //TODO, trzeba sprawdzic, czy to nie jest zla liczba
}

VeryLongInt::VeryLongInt(const char str[]) {
   //int size = sizeof(str)/sizeof(char);
   std::string s(str);
   val = s;
   nan = false;
}

VeryLongInt::VeryLongInt(const VeryLongInt& vlint) {
    val = vlint.val;
    nan = vlint.nan;
}

VeryLongInt::VeryLongInt(VeryLongInt&& rref) {
   val = std::move(rref.val); //moving contents to new container
   nan = std::move(rref.nan);
}

VeryLongInt::~VeryLongInt() { } //no destructing needed effectively

VeryLongInt& VeryLongInt::operator =(const VeryLongInt& vlint) {
   if(this != &vlint) { //checking if we are not self referencing ourselves
      val = vlint.val;
      nan = vlint.nan;
   }
   return *this;
}

VeryLongInt& VeryLongInt::operator =(const int n) {
   val = std::to_string(n);
   nan = n <= 0;
   return *this;
}

VeryLongInt& VeryLongInt::operator =(VeryLongInt&& rref) {
   val = std::move(rref.val);
   nan = std::move(rref.nan);
   return *this;
}

/******************************COMPOUND ARITHMETIC***********************************/

VeryLongInt& VeryLongInt::operator +=(const VeryLongInt& anotherVeryLongInt){
   
   //Checking if NaN is an argument
   if(this->nan || anotherVeryLongInt.nan){
      this->nan=true;
      return giveNaN();
   };
   
   //Creating strings to operate on them
   std::string num1(this->val);
   std::string num2(anotherVeryLongInt.val);
   
   //Normalize and create string for result
   int len = normalize(num1, num2);
   std::string resultStr(len+1,'0');
   
   //Adding algorithm like in school
   int transfer = 0;
   for(int i=len;i>0;i--){
      int digit1 = num1[i-1] - '0';
      int digit2 = num2[i-1] - '0';
      int sum = digit1 + digit2 + transfer;
      resultStr[i] = (sum % 10) + '0';
      transfer = sum/10;
   }
   
   //Adding remaining transfer, that could be left after the whole proces
   if(transfer>0){
      resultStr[0] = transfer + '0';
   }
   
   denormalize(resultStr);
   
   this->val = resultStr;
   return *this;

};


VeryLongInt& VeryLongInt::operator -=(const VeryLongInt& anotherVeryLongInt){
   
   //Checking if NaN is an argument + whether the result will be negative
   if(*this<anotherVeryLongInt) {
      this->nan=true;
      return giveNaN();
   };
   
   if(this->nan || anotherVeryLongInt.nan){
      this->nan=true;
      return giveNaN();
   };
   
   //Creating strings to operate on them  
   std::string num1(this->val);
   std::string num2(anotherVeryLongInt.val);

   //Normalize and create string for result  
   int len = normalize(num1, num2);
   std::string resultStr(len,'0');
   
   int transfer = 0;
   
   //Substracting algorithm like in school   
   for(int i=len-1;i>=0;i--){
      int digit1 = num1[i] - '0';
      int digit2 = num2[i] - '0';
      int difference = digit1+transfer-digit2;
      
      if(difference >= 0){
         transfer = 0;
         resultStr[i] = difference + '0';
      } else {
         transfer = -1;
         resultStr[i] = difference + 10 + '0';
      }
   }
   
   denormalize(resultStr);
   this->val=resultStr;
   
   return *this;
};

VeryLongInt& VeryLongInt::operator *=(const VeryLongInt& anotherVeryLongInt) {

   //Checking if NaN is an argument
   if(this->nan || anotherVeryLongInt.nan){
      this->nan=true;
      return giveNaN();
   };
   
   //Creating string to operate on them   
   std::string num1(this->val);
   int len = num1.length();

   //Create string for result and helping string zeroes
   VeryLongInt* result = new VeryLongInt("0");
   std::string zeroes("");
   
   //Substracting algorithm like in school      
   for(int i=len;i>0;i--){
       VeryLongInt tempInt = (anotherVeryLongInt.timesChar(num1[i-1]));
       tempInt.val = tempInt.val.append(zeroes);
       zeroes.append("0");
       *result += tempInt;
   }
      
   denormalize(result->val);
   
   this->val = result->val;
   free(result);
   return *this;
   
};


//Binary search for the result. We know that the result of division is between
//0 and *this. We binary search for result inbetwen these numbers.
//The cost is O(log_2 *this)^3 = O(log_10 *this)^3 = O( len(*this) )^3

VeryLongInt& VeryLongInt::operator/=(const VeryLongInt& anotherVeryLongInt){
   //Checking if NaN is an argument + whether anotherVeryLongInt is nonzero   
   if(this->nan || anotherVeryLongInt.nan){
      this->nan = true;
      return giveNaN();
   }
   
   if(anotherVeryLongInt.val == "0"){
      this->nan = true;
      return giveNaN();
   }
   //We create lower and upper bound for binsearch, variable for result and const one.
   VeryLongInt lowerBound("0");
   VeryLongInt upperBound(this->val);
   std::string(resultStr);
   VeryLongInt one("1");
   
   //Just like in binsearch algorithm, we set middle algorithm
   //Then we count residuum which is the difference betwen our result and *this
   VeryLongInt sum = lowerBound + upperBound;
   VeryLongInt middle = sum.div2();
   VeryLongInt residuum = *this - middle*anotherVeryLongInt;
   bool shouldContinue = true;
   
   while(lowerBound < upperBound && shouldContinue){
      if(residuum.isValid() && ( residuum < anotherVeryLongInt ) ){
         shouldContinue = false;
         resultStr = middle.val;
      } else if( residuum.isValid() ){//if residuum is valid number then *this > middle*anotherVeryLongInt 
         lowerBound = middle+one;
         resultStr = lowerBound.val;
      } else{
         upperBound = middle-one;
         resultStr = upperBound.val;
      }
      middle = (lowerBound+upperBound).div2();
      residuum = *this - middle * anotherVeryLongInt;
   }
   
   this->val=resultStr;
   return *this;
};

VeryLongInt& VeryLongInt::operator %=(const VeryLongInt& anotherVeryLongInt){
   
   //Checking if NaN is an argument + whether anotherVeryLongInt is nonzero   
   if(this->nan || anotherVeryLongInt.nan){
      this->nan = true;
      return giveNaN();
   }
   
   //Counting result
   VeryLongInt result = *this - ( anotherVeryLongInt * (*this/anotherVeryLongInt) );
   this->val=result.val;
   return *this;
};


//Multiplycatioon by 2^anotherVeryLongInt
VeryLongInt& VeryLongInt::operator <<=(const VeryLongInt& anotherVeryLongInt){
   
   //Checking if NaN is an argument + whether anotherVeryLongInt is nonzero   
   if(this->nan || anotherVeryLongInt.nan){
      this->nan = true;
      return giveNaN();
   }
   
   //Counting result
   VeryLongInt result = *this * fastPower(2,anotherVeryLongInt);
   this->val = result.val;
   return *this;
};

//Division by 2^anotherVeryLongInt
VeryLongInt& VeryLongInt::operator >>=(const VeryLongInt& anotherVeryLongInt){

   //Checking if NaN is an argument + whether anotherVeryLongInt is nonzero   
   if(this->nan || anotherVeryLongInt.nan){
      this->nan = true;
      return giveNaN();
   }
   //Counting result 
   VeryLongInt result = *this / fastPower(2,anotherVeryLongInt);
   this->val = result.val;
   return *this;
};

/*************************OSTREAM OUTPUT OPERATOR***************************/

std::ostream& operator <<(std::ostream& stream, const VeryLongInt& toBeWriten) {
   if( toBeWriten.isValid() ){
      stream << toBeWriten.val;
   } else{
      stream << "NaN";
   }
   return stream;
};

/***************************OTHER METHODS**********************************/

VeryLongInt::operator bool() const {
   return this->isValid();
}

int VeryLongInt::numberOfBinaryDigits() const {
   if(this == &NaN()) {
      return 0;
   }
   if(this == &Zero()) {
      return 1;
   }
   VeryLongInt temp(*this);
   int count = 0;
   std::string binaryRepresentation = "";
   while(temp.val != "0") {
      temp = temp.div2();
      count++;
      char c = temp.val[0];
   if(static_cast<int>(c) % 2 == 0) {
         binaryRepresentation = "0" + binaryRepresentation;
      } else {
         binaryRepresentation = "1" + binaryRepresentation;
      }
   }
   int i = 0;
   while(binaryRepresentation[i] != '1') {
      count--;
      i++;
   }
   return count + 1;
}


bool VeryLongInt::isValid() const{
   return !this->nan;
}

std::string VeryLongInt::getVal() const {
   return val;
}

void VeryLongInt::setVal(std::string s) {
   bool argAssert = true;
   for(auto i : s) {
      if(!isdigit(i)) {
         std::cerr << "setVal error, argument is not numerical" << std::endl;
         argAssert = false;
         break;
      }
   }
   if(argAssert) {
      val = s;
   }
}

bool VeryLongInt::getNan() const {
   return nan;
}

void VeryLongInt::setNan(bool b) {
   nan = b;
}