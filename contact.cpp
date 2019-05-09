
#include "contact.h"

#include <sstream>
#include <stdlib.h>

Email::Email(string type, string email_addr){
    this->type = type;
    this->email_addr = email_addr;
}


void Email::set_contact(){
    cout << "Enter the type of email address: ";
    cin >> this->type;

    cout << "Enter email address: ";
    cin >> this->email_addr;
}


string Email::get_contact(string style){
    if (style=="full")
	    return "(" + type + ") " + email_addr;
    else 
        return email_addr;
}


void Email::print(){
	cout << get_contact() << endl;
}


Phone::Phone(string type, string num){
    this->type = type;

    // parse "-" chars if they exist with stringstream
    string temp = "";
    string parsed_str = "";
    istringstream ss(num);

    while (getline(ss, temp, '-')) {
        parsed_str += temp;
    }

    this->phone_num = atol(parsed_str.c_str());
}


void Phone::set_contact(){
	cout <<"Enter the type of phone number: ";
    cin >> this->type;

    string temp = "";
	cout << "Enter the phone number: ";
    cin >> temp;

    // use stringstream to convert string to long int
    string parsed_str = "";
    istringstream ss(temp);

    while (getline(ss, temp, '-')) {
        parsed_str += temp;
    }

    this->phone_num = atol(parsed_str.c_str());
}


string Phone::get_contact(string style){
    stringstream ss;
    ss << phone_num;
    string phone_str = ss.str();

    if (style=="full") {
        return "(" + type + ") " + phone_str.substr(0,3) + "-" + 
                                   phone_str.substr(3,3) + "-" + 
                                   phone_str.substr(6);
    } else {
        return phone_str.substr(0,3) + "-" + 
               phone_str.substr(3,3) + "-" + 
               phone_str.substr(6);
    }
}


void Phone::print(){
	cout << get_contact() << endl;
}


