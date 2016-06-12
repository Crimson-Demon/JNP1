//Author: Marcin Kania, Katarzyna Herba

#include <iostream>
#include <unordered_map>
#include <cstring> // for many string functions
#include <sstream> // for converting address to string
#include <algorithm> // for all_of
#include <vector>
#include <cassert>
#include <utility> // for pair and make_pair
#include "cmaptel"

#ifndef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif

using namespace std;

/** Defining shorthand for the type containing all dictionaries. */
typedef unordered_map <unsigned long, unordered_map<string, string>> dict_type;

/** Enum for all possible operations on a dictionary. */
enum command_t {CREATE, DELETE, INSERT, ERASE, TRANSFORM};

/** Keeps id that will be assigned to the next created dictionary. */
unsigned long dict_id = 0;

/** Auxiliary functions to avoid static fiasco intialization { */

dict_type& dictionaries() {
  static dict_type* resp = new dict_type();
  return *resp;
}

string command_names(command_t i) {
   static string command_names [] = {
      "create", "delete", "insert", "erase", "transform"};
   return command_names[i];
}

/** Checks if there is a dictionary with given id. */
bool is_id_ok(unsigned long id) {
   return dictionaries().count(id) > 0;
}

/** Checks if given string is a valid telephone number. */
bool is_phone_ok(char const* tel_nr, const size_t num ) {
   if(tel_nr == NULL) {
      return false;
   } else {
      string tel_nr_str(tel_nr);
      if(tel_nr_str.length() > num) {
         return false;
      } else {
         return all_of(tel_nr_str.begin(), tel_nr_str.end(), ::isdigit);
      }
   }
}

/** Checks if the phone numbers length is in acceptable range. */
bool is_len_ok(size_t len, const size_t num) {
   return ((len <= num + 1) && (len > 0));
}

/** Writes to the stderr message about a function being called,
 * and with what arguments.
 */
void write_start_message(command_t command, vector<string> arguments) {
   ios_base::Init();
   cerr << "maptel: maptel_";
   cerr << command_names(command) << "(";
   for (vector<string>::iterator it = arguments.begin();
        it != arguments.end(); it++) {
      if(it != arguments.begin()) {
         cerr << ", ";
      }
      cerr << (*it);
   }
   cerr << ")" << endl;
}

/** Writes to stderr message about function's result. */
void write_end_message(command_t command, string message) {
   cerr << "maptel: maptel_";
   cerr << command_names(command) << ": ";
   cerr << message << endl;
}

/** } */

/** Descriptons of each functions can be found in maptel.h
 *Please refer to it for basic information on maptel functions
 */

unsigned long maptel_create() {
   unordered_map <string, string> dictionary;
   dictionaries()[dict_id] = dictionary;
   if(debug) {
      // Contains the arguments with which the function was called.
      vector<string> arguments;
      write_start_message(CREATE, arguments);
      string message = "new map id = ";
      message += to_string(dict_id);
      write_end_message(CREATE, message);
   }
   // Increasing dict_id so the next created dictionary
   // has a unique id.
   dict_id++;
   return dict_id - 1;
}
   
void maptel_delete(unsigned long id) {
   if(debug) {
      assert(is_id_ok(id));
      // Contains the arguments with which the function was called.
      vector<string> arguments;
      arguments.push_back(to_string(id));
      write_start_message(DELETE, arguments);
      string message = "map " + to_string(id)  + " deleted";
      write_end_message(DELETE, message);
   }
   dictionaries().erase(id);
}
   
void maptel_insert(unsigned long id, char const *tel_src,
   char const *tel_dst) {
   if(debug) {
      assert(is_id_ok(id));
      assert(is_phone_ok(tel_src, TEL_NUM_MAX_LEN));
      assert(is_phone_ok(tel_dst, TEL_NUM_MAX_LEN));
   }
   string tel_src_str (tel_src);
   string tel_dst_str (tel_dst);
   if(debug) {
      // Contains the arguments with which the function was called.
      vector<string> arguments;
      arguments.push_back(to_string(id));
      arguments.push_back(tel_src_str);
      arguments.push_back(tel_dst_str);
      write_start_message(INSERT, arguments);
      write_end_message(INSERT, "inserted");
   }
   dictionaries()[id][tel_src_str] = tel_dst_str;
}

void maptel_erase(unsigned long id, char const *tel_src) {
   if(debug) {
      assert(is_id_ok(id));
      assert(is_phone_ok(tel_src, TEL_NUM_MAX_LEN));
   }
   string tel_src_str (tel_src);
   if(debug) {
      // Contains the arguments with which the function was called.
      vector<string> arguments;
      arguments.push_back(to_string(id));
      arguments.push_back(tel_src_str);
      write_start_message(ERASE, arguments);
   }
   // We erase the dictionary if it exists.
   if(dictionaries()[id].count(tel_src_str)) {
      dictionaries()[id].erase(tel_src_str);
      if(debug) {
         write_end_message(ERASE, "erased");
      }
   } else {
      if(debug) {
         write_end_message(ERASE, "nothing to erase");
      }
   }
}

void maptel_transform(unsigned long id, char const *tel_src, char *tel_dst,
   size_t len) {
   if(debug) { // initial assert
      assert(::is_id_ok(id));
      assert(::is_phone_ok(tel_src, TEL_NUM_MAX_LEN));
      assert(::is_len_ok(len, TEL_NUM_MAX_LEN));
   }
   // 
   string tel_src_str (tel_src);
   string tel_dst_str (tel_dst);
   bool isCycle = false;
   pair<string, string> error;

   if(debug) {
   // Contains the arguments with which the function was called.
   vector<string> arguments;
   arguments.push_back(to_string(id));
   arguments.push_back(tel_src_str);
   // stringstream used to obtain a string of the address of the tel_dst
   stringstream ss;
   ss << &tel_dst;
   arguments.push_back(ss.str());
   arguments.push_back(to_string(len));
   write_start_message(TRANSFORM, arguments);
   }
   // checking if tel_src is a valid number in the dictionary
   if(dictionaries()[id].find(tel_src_str) != dictionaries()[id].end()) {
      string s1 = dictionaries()[id][tel_src_str];
      string s2 = "";
      if(dictionaries()[id].find(s1) != dictionaries()[id].end()) {
         s2 = dictionaries()[id][s1];
      } else {
         s2 = "";
      }
      string previousS2 = s1; // remembers the key before s2
      // seeking cycle
      while((dictionaries()[id].find(s2) != dictionaries()[id].end()) && 
      (s1.compare(s2) != 0)) {
         s1 = dictionaries()[id][s1]; // moves one step forward
         string tmpStr = dictionaries()[id][s2];
         if(dictionaries()[id].find(tmpStr) != dictionaries()[id].end()) {
            previousS2 = dictionaries()[id][s2];
            if(dictionaries()[id].find(previousS2) != dictionaries()[id].end()) {
               s2 = dictionaries()[id][previousS2]; // two steps forward
            } else {
               s2 = "";
            }
         } else {
            s2 = "";
         }
      }
      if(dictionaries()[id].find(s2) == dictionaries()[id].end()) { // if no cycle
         if(s2 == "") { // there were no more elements to check
            error = make_pair(tel_src_str, previousS2);
            const char* tmp_tel_src = previousS2.c_str();
            strncpy(tel_dst, tmp_tel_src, len);
         } else {
            error = make_pair(tel_src_str, s2);
            const char* tmp_tel_src = s2.c_str();
            strncpy(tel_dst, tmp_tel_src, len);
         }
      } else { // if there is a cycle
         if(debug) {
            isCycle = true;
            error = make_pair(tel_src_str, tel_src_str);
         }
         strncpy(tel_dst, tel_src, len);
      }
   } else { // if there is no tel_src
      error = make_pair(tel_src_str, tel_src_str);
      strncpy(tel_dst, tel_src, len);
   }

   if(debug) { // debug: printing to stderr
      string message = "";
      if(isCycle) {
         message = "cycle detected";
         write_end_message(TRANSFORM, message);
      }
      message = error.first+" -> "+error.second+", ";
      write_end_message(TRANSFORM, message);
   }
}
