
#include "network.h"
#include "misc.h"
#include <limits>
#include <fstream>
#include <dirent.h>

using namespace std;

Network::Network(){
    head = NULL;
    tail = NULL;
    count = 0;
}


Network::Network(string fileName){
    loadDB(fileName);
}

Network::~Network(){
    Connection* temp = head;

    while (temp != NULL) {
        Connection* ptr = temp->next;
        delete temp;
        temp = ptr;
    }

    delete tail;
    head = NULL;
    tail = NULL;
    count = 0;
}


void Network::push_front(Connection* newEntry){ 
    newEntry->prev = NULL;
    newEntry->next = head;

    if (head != NULL)
        head->prev = newEntry;
    else
        tail = newEntry;
    
    head = newEntry;
    count++;
}


void Network::push_back(Connection* newEntry){
    // Adds a new Connection (newEntry) to the back of LL
    newEntry->next = NULL;
    newEntry->prev = tail;

    if (tail != NULL)
        tail->next = newEntry;
    else 
        head = newEntry;

    tail = newEntry;
    count++;
}


Connection* Network::search(Connection* searchEntry){
    // Searches the Network for searchEntry
    // if found, returns a pointer to it, else returns NULL
    Connection* ptr = head;

    while (ptr != NULL) {
        if (*ptr == *searchEntry)
            return ptr;
        else
            ptr = ptr->next;
    }
    return NULL;
}


Connection* Network::search(string fname, string lname){
    // New == for Connection, only based on fname and lname
    // if found, returns a pointer to it, else returns NULL
    Connection* temp = new Connection(fname, lname, "1/1/1970", "(type) email@web.com", "(type) 123-456-7890");
    Connection* found = search(temp);

    if (found != NULL) {
        delete temp;
        return found;
    } else {
        delete temp;
        return NULL;
    }
}


Connection* Network::search(string code) {
    Connection* ptr = head;

    string tmp_code = "";
    while (ptr != NULL) {
        tmp_code = codeName(ptr->f_name, ptr->l_name);
        if (code == tmp_code)
            return ptr;
        else
            ptr = ptr->next;
    }

    return NULL;
}


void Network::printDB(){
    cout << "Number of connections: " << count << endl;
    cout << "------------------------------" << endl;
    Connection* ptr = head;
    while(ptr != NULL){
        ptr->print_connection();
        cout << "------------------------------" << endl;
        ptr = ptr->next;
    }
}


void Network::saveDB(string filename){
    // Saves the network in file <filename>
    std::ofstream output;
    output.open(filename.c_str());

    Connection* temp = head;
    if (output.is_open()) {
        while (temp != NULL) {
            temp->write_connection(output);
            temp = temp->next;
        }
    }

    output.close();
}


void Network::loadDB(string filename){
    // Loads the netwrok from file <filename>
    // The format of the input file is similar to saveDB
    // Look at network studentDB.txt as a template
    // Phone number can be with dashes or without them 
    Connection* ptr = head;
    
    while(head != NULL){
        ptr = head->next;
        delete head;
        head = ptr;
        count--;
    }
    
    head = NULL;
    tail = NULL;
    ifstream infile;
    infile.open(filename.c_str());
    string buff, fname, lname, bdate, email, phone;
    
    vector <string> friend_codes;
    vector < vector<string> > connection_friends;

    while(getline(infile, buff)) {
        friend_codes.clear();

        lname = buff.substr(0, buff.find(','));
        fname = buff.substr(buff.find(',')+2);
        getline(infile, bdate);
        getline(infile, email);     // contains email data 
        getline(infile, phone);     // contains phone data
        Connection* newEntry = new Connection(fname, lname, bdate, email, phone);
        this->push_back(newEntry);
        getline(infile, buff);

        while(buff.compare(0, 5, "-----") != 0) {
            friend_codes.push_back(buff);
            getline(infile, buff);
        }

        connection_friends.push_back(friend_codes);
    }

    infile.close();

    int i = 0;
    ptr = head;
    vector<string> temp;
    vector<string>::iterator iter;
    Connection* new_friend = NULL;

    while(ptr != NULL) {
        temp = connection_friends[i];

        for(iter = temp.begin(); iter != temp.end(); ++iter) {
            new_friend = search(*iter);
            if (new_friend != NULL)
                ptr->makeFriend(new_friend);            
        }

        ptr = ptr->next;
        i++;
    }
}


bool Network::remove(string fname, string lname){
    Connection* pRemoveThis = search(fname, lname);
    Connection* a_friend = NULL;
    vector<Connection*>::iterator iter;

    for (iter = pRemoveThis->friends.begin(); iter!= pRemoveThis->friends.end(); ++iter) {
        // find pointer for each friend of pRemoveThis
        a_friend = search(*iter);

        // remove pRemoveThis from all of their friends' vectors
        if (a_friend != NULL) {    
            for (int i = 0; i < a_friend->friends.size(); i++) {
                if(*(a_friend->friends[i]) == *pRemoveThis) {
                    a_friend->friends.erase(a_friend->friends.begin() + i);
                }
            }
        }
    }

    if (pRemoveThis != NULL) {
        // initialize prev/next pointers to NULL in case prev/next element is head or tail
        Connection* prev = NULL;
        Connection* next = NULL;

        // find location of prev/next elements that item being removed points to
        prev = pRemoveThis->prev;
        next = pRemoveThis->next;

        // first check if prev/next elements are NULL, then if not NULL assign 
        // respective pointers to their prev/next attributes
        if (prev == NULL)
            head = next;
        else 
            prev->next = next;

        if (next == NULL)
            tail = prev;
        else 
            next->prev = prev;

        count--;
        delete pRemoveThis;
        return true;
    }

    return false;
}


void Network::sendEmail(string addr, string subject, string content, string attachment=""){
    // Sending a command looks like:
    // echo "content" | mailx -s "subject" -a "attachment" addr;
    subject = "\"" + subject + "\"";
    content = "\"" + content + "\"";

    string command = "";

    if (attachment != "") {
        attachment = "-a \"" + attachment + "\"";
        command = "echo " + content + " | mailx -s " + subject + attachment + " " + addr;
    } else {
        command = "echo " + content + " | mailx -s " + subject + " " + addr;
    }
    system(command.c_str());
}


void Network::showMenu(){
    int opt;

    while(1){
        cout << "\033[2J\033[1;1H";
        printMe("banner"); // from misc library

        cout << "Select from below: \n";
        cout << "1. Save network database \n";
        cout << "2. Load network database \n";
        cout << "3. Add a new connection \n";
        cout << "4. Remove a connection \n";
        cout << "5. Search \n";
        cout << "6. Send an email \n";
        cout << "7. Send an iMessage \n";
        cout << "8. Print database \n";
        cout << "9. Connect \n";
        cout << "\nSelect an option ... ";
        
        if (cin >> opt) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Wrong option! " << endl;
            return;
        }
        
        string fname, lname, fileName, bdate;
        Connection* check_exists = NULL; 
        Connection* new_connection = NULL;
        Connection* person1 = NULL;
        Connection* person2 = NULL;
        bool exists;

        cout << "\033[2J\033[1;1H";

        if (opt==1){
            cout << "Saving network database \n";
            cout << "Enter the name of the save file: ";
            cin >> fileName;
            cout << "Network saved in " << fileName << endl;
            saveDB(fileName);
        }
        else if (opt==2){
            cout << "Loading network database \n"; 
            DIR *dir;
            struct dirent *ent;
            char targetFolderAddr[] = "./";
            
            if ((dir = opendir ("./")) != NULL) { 
                while ((ent = readdir (dir)) != NULL) {
                    string temp(ent->d_name);
                    if (temp.find(".db") != string::npos)
                        cout << ent->d_name << endl; 
                }
                cout << "Enter the name of the load file: ";
                cin >> fileName;

                loadDB(fileName);
                closedir (dir);
            } else {
                /* could not open directory */
                cout << "File FILENAME does not exist!" << endl;
            }

            // If file is loaded successfully, also print the count of connections in it: 
            cout << "Network loaded from " << fileName << " with " << count << " connections \n";
        }
        else if (opt == 3){
            new_connection = new Connection();
            check_exists = search(new_connection);

            if (check_exists == NULL) {
                cout << "Adding a new connection \n";
                push_front(new_connection);
            } else {
                cout << "Connection already exists \n";
                delete new_connection;
            }

        }
        else if (opt == 4){
            cout << "Removing a connection \n";
            cout << "First name: ";
            getline(cin, fname);
            cout << "Last name: ";
            getline(cin, lname);

            exists = remove(fname, lname);

            if (exists)
                cout << "Remove Successful! \n";
            else
                cout << "Connection not found! \n";
        }
        else if (opt==5){
            cout << "Searching: \n";
            cout << "First Name: ";
            getline(cin, fname);
            cout << "Last Name: ";
            getline(cin, lname);

            check_exists = search(fname, lname);

            if (check_exists != NULL)
                check_exists->print_connection();
            else
                cout << "Not found! \n";
        }
        else if (opt==6){
            cout << "Sending email to: \n";
            cout << "First Name: ";
            std::getline (std::cin, fname);
            cout << "Last Name: ";
            std::getline (std::cin, lname);
            Connection* ptr = search(fname, lname);
            if (ptr){
                string subject, content, attachment;
                cout << "Sending email to: \n";
                ptr->print_connection();
                cout << "Subject: ";
                std::getline (std::cin, subject);
                cout << "Content: ";
                std::getline (std::cin, content);
                cout << "Attachment: ";
                std::getline (std::cin, attachment);
                string addr = ptr->email->get_contact("1");
                sendEmail(addr, subject, content, attachment);
            }
            else 
                cout << "Contact not found! \n";
        }
        else if (opt==7){
            cout << "Option currently not available\n";
        }
        else if (opt==8){
            cout << "Network Database: \n";
            printDB();
        }
        else if (opt==9) {
            cout << "Make friends: \n";
            cout << "Connection 1 \n";
            cout << "First name: ";
            getline(cin, fname);
            cout << "Last name: ";
            getline(cin, lname);

            person1 = search(fname, lname);
            
            if (person1 == NULL) {
                cout << "Connection not found \n";
            } else {
                cout << "Connnection 2 \n";
                cout << "First name: ";
                getline(cin, fname);
                cout << "Last name: ";
                getline(cin, lname);

                person2 = search(fname, lname);

                if (person2 == NULL) {
                    cout << "Connection not found \n";
                } else {
                    person1->makeFriend(person2);
                    person2->makeFriend(person1);
                }
            }
        }
        else
            cout << "Nothing matched!\n";
        
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "\n\nPress Enter key to go back to main menu ... ";
        string temp;
        std::getline (std::cin, temp);
        cout << "\033[2J\033[1;1H";
    }
}


