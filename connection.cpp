
#include "connection.h"

Connection::Connection(){
    set_connection();
}


Connection::~Connection(){
    delete birthdate, email, phone;
}


Connection::Connection(string fname, string lname, string bdate, 
                       string email, string phone) {
    // phone and email strings are in full version
    this->f_name = fname;
    this->l_name = lname;
    this->birthdate = new Date(bdate);

    // parse email info
    string email_type, email_address;
    stringstream ss(email.substr(1));

    std::getline(ss, email_type, ')');
    std::getline(ss, email_address);

    email_address.erase(email_address.find(' '), 1);

    this->email = new Email(email_type, email_address);

    // clear stringstream
    string phone_type, phone_number;
    ss.str("");
    ss.clear();
    ss << phone.substr(1);

    // parse phone info
    std::getline(ss, phone_type, ')');
    std::getline(ss, phone_number);

    phone_number.erase(phone_number.find(' '), 1);

    this->phone = new Phone(phone_type, phone_number);
}


Connection::Connection(string filename){
    std::ifstream input;
    input.open(filename.c_str());

    string temp1 = "";
    string temp2 = "";
    string temp3 = "";

    if (input.is_open()) {
        std::getline(input, this->f_name);
        std::getline(input, this->l_name);
        std::getline(input, temp1);
        std::getline(input, temp2); // contains (type) email@website.com
        std::getline(input, temp3); // contains (type) XXX-XXX-XXXX
    }

    input.close();

    this->birthdate = new Date(temp1);

    // parse temp2, containing email info
    string email_type, email;
    stringstream ss(temp2.substr(1));

    std::getline(ss, email_type, ')');
    std::getline(ss, email);

    email.erase(email.find(' '), 1);

    this->email = new Email(email_type, email);

    // clear stringstream
    string phone_type, phone;
    ss.str("");
    ss.clear();
    ss << temp3.substr(1);

    // parse temp3, containing phone info
    std::getline(ss, phone_type, ')');
    std::getline(ss, phone);

    phone.erase(phone.find(' '), 1);

    this->phone = new Phone(phone_type, phone);
}


void Connection::set_connection(){
    // prompts for the information of the user
    // first/last name can have spaces!
    // date format must be "M/D/YYYY"
    // We are sure user enters info in correct format.
    string temp;
    string type;

    cout << "First Name: ";
    cin >> this->f_name;

	cout << "Last Name: ";
    cin >> this->l_name;

    cout << "Birthdate (M/D/YYYY): ";
    cin >> temp;
    this->birthdate = new Date(temp);
    
    cout << "Type of email address: ";
    cin >> type;

    cout << "Email address: ";
    cin >> temp;
    this->email = new Email(type, temp);

    cout << "Type of phone number: ";
    cin >> type;

    cout << "Phone number: ";
    cin >> temp;
    this->phone = new Phone(type, temp);
}


void Connection::set_connection(string filename){
    // Look at connection_template files as examples.     
    // Phone number in files can have '-' or not.
    std::ifstream input;
    input.open(filename.c_str());

    string temp1 = "";
    string temp2 = "";
    string temp3 = "";

    if (input.is_open()) {
        std::getline(input, this->f_name);
        std::getline(input, this->l_name);
        std::getline(input, temp1);
        std::getline(input, temp2); // contains (type) email@website.com
        std::getline(input, temp3); // contains (type) XXX-XXX-XXXX
    }

    input.close();

    this->birthdate = new Date(temp1);

    // parse temp2, containing email info
    string email_type, email;
    stringstream ss(temp2.substr(1));

    std::getline(ss, email_type, ')');
    std::getline(ss, email);

    this->email = new Email(email_type, email);

    // clear stringstream
    string phone_type, phone;
    ss.str("");
    ss.clear();
    ss << temp3.substr(1);

    // parse temp3, containing phone info
    std::getline(ss, phone_type, ')');
    std::getline(ss, phone);

    this->phone = new Phone(phone_type, phone);
}


bool Connection::operator==(const Connection& rhs){
    if ((f_name == rhs.f_name) &&
        (l_name == rhs.l_name)) 
        return true;
    else 
        return false;
}


bool Connection::operator!=(const Connection& rhs){ 
    return !(*this == rhs);
}


void Connection::print_connection(){
	cout << l_name <<", " << f_name << endl;
	birthdate->print_date("Month D, YYYY");
    email->print();
    phone->print();

    vector<Connection*>::iterator iterator; // for iterating through vector

    for (iterator = friends.begin(); iterator != friends.end(); ++iterator) {
        cout << codeName((*iterator)->f_name, (*iterator)->l_name) << endl;
    }
}


void Connection::makeFriend(Connection* newFriend) {
    this->friends.push_back(newFriend);
}


void Connection::write_connection(ofstream& outfile) {
    outfile << l_name << ", " << f_name << "\n";
    outfile << birthdate->get_date() << "\n";
    outfile << email->get_contact() << "\n";
    outfile << phone->get_contact() << "\n";

    vector<Connection*>::iterator iterator;

    for (iterator = friends.begin(); iterator != friends.end(); ++iterator) {
        outfile << codeName((*iterator)->f_name, (*iterator)->l_name) << "\n";
    }

    outfile << "--------------------" << "\n";
}
