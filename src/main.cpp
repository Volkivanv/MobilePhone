#include <iostream>
#include <string>
#include <map>
#include <fstream>

#define QUESTION "Input your request: add, call, sms, show, exit."

class Person{
    std::string name;
    std::string phoneNumber;
public:
    bool setPhoneNumber(std::string& somePhoneNumber){
        if(checkPhoneNumber(somePhoneNumber)){
            phoneNumber = somePhoneNumber;
            return true;
        }else{
         std::cout<<"Invalid phone number type" << std::endl;
            return false;
        }
    }

    std::string getPhoneNumber(){
        return phoneNumber;
    }

    bool setName(std::string& someName){
        for(int i = 0; i<someName.length();i++){
            if(!(((someName[i]>='a')&&(someName[i]<='z'))||((someName[i]>='A')&&(someName[i]<='Z'))||(someName[i]==' '))){
                std::cout<<"Name contents unacceptable symbols!"<<std::endl;
                return false;
            }
        }
        name = someName;
        return true;
    }

    std::string getName(){
        return name;
    }

    void callToNumber(){
        std::cout << "CALL " <<name <<" "<< phoneNumber << std::endl;
    }
    void smsToNumber(){
        std::string sms;
        std::cout << "Input sms to " << name << std::endl;
        std::cin.ignore();
        std::getline(std::cin,sms);
        std::cout << "Message '"<< sms <<"' have sent" << std::endl;
    }

private:
    bool checkPhoneNumber(std::string somePhoneNumber){
        if(somePhoneNumber[0]!='+') return false;
        if(somePhoneNumber[1]!='7') return false;
        for(int i = 2; i < somePhoneNumber.length(); i++ ){
            if((somePhoneNumber[i]<'0')||(somePhoneNumber[i]>'9')) return false;
        }
        return true;
    }

};

class Mobile{
    std::map<std::string,std::string>phoneBook;
    std::map<std::string,std::string>reversPhoneBook;
public:
    void readPhoneBook(std::string filePath){
        std::ifstream pB(filePath);
        if(pB){
            while (!pB.eof()){
                std::string bufString;
                std::getline(pB,bufString);
                std::string personName;
                std::string phone;
                int pos = bufString.find('+');
                personName = bufString.substr(0,pos-1);
                phone = bufString.substr(pos, bufString.size() - 1);
                auto person = new Person;
                bool nameSet = person->setName(personName);
                bool phoneSet = person->setPhoneNumber(phone);
                if((nameSet)&&(phoneSet)) {
                    phoneBook.insert(std::make_pair(person->getName(), person->getPhoneNumber()));
                    reversPhoneBook.insert(std::make_pair(person->getPhoneNumber(), person->getName()));
                }

                delete person;
               // std::cout << person << " " << phone << std::endl;
            }
        } else {
            std::cerr << "File with phonebook is not found! Please use 'add' for create new phonebook" << std::endl;
        }
        pB.close();
    }
    void showList(){
        for(auto it = phoneBook.begin(); it != phoneBook.end();it++){
            std::cout << it->first << " " << it->second << std::endl;
        }

    };
    void addPerson(std::string filePath){
        std::string bufName;
        std::string bufPhoneNumber;
        auto* person = new Person;

        std::cout << "Please input person (Name Surname Patronymic)"<< std::endl;
        std::cin.ignore();
        std::getline(std::cin,bufName);
        bool nameSet = person->setName(bufName);


        std::cout << "Please input phone number in format +7<**********>" << std::endl;
        std::cin.ignore();
        std::getline(std::cin,bufPhoneNumber);
        if(bufPhoneNumber[0]!='+') {
            bufPhoneNumber = '+' + bufPhoneNumber;
        }
       //std::cin>>bufPhoneNumber;
        bool phoneSet = person->setPhoneNumber(bufPhoneNumber);

        if((nameSet)&&(phoneSet)){
            phoneBook.insert(std::make_pair(person->getName(), person->getPhoneNumber()));
            reversPhoneBook.insert(std::make_pair(person->getPhoneNumber(), person->getName()));
            std::ofstream pB(filePath,std::ios::app|std::ios::ate);
            if(pB){
                if(pB.tellp()==0){
                    std::cout << "First!"<< pB.tellp()<<std::endl;
                    pB<< person->getName()<<" "<<person->getPhoneNumber();

                }else{
                    pB<<std::endl <<person->getName()<<" "<<person->getPhoneNumber();
                }
                std::cout << "Person entered!" << std::endl;
            }else{
                std::cerr << "File is not found!" << std::endl;
            }
            pB.close();

        } else{
            std::cout << "Person not entered!" << std::endl;
        }

        delete person;

    }
    void callMobile(){
        std::string request;
        std::cout << "Please input phone number or full name of person" << std::endl;
        std::cin.ignore();
        std::getline(std::cin,request);
        std::cout << request << std::endl;
        auto* person = new Person;
        if(isPhoneNumber(request)){
            bool phoneSet = person->setPhoneNumber(request);
            if(phoneSet) {
                auto it = reversPhoneBook.find(request);
                if(it!=reversPhoneBook.end()){
                    person->setName(it->second);
                    person->callToNumber();
                } else {
                    std::cout<<"This number is not listed!"<<std::endl;
                }
            }

        } else{
            bool nameSet = person->setName(request);
            if(nameSet){
                auto it = phoneBook.find(request);
                if(it!=phoneBook.end()){
                    person->setPhoneNumber(it->second);
                    person->callToNumber();
                } else {
                    std::cout<<"This person is not listed!"<<std::endl;
                }
            }
        }
        delete person;

    };
    void smsToMobile(){
        std::string request;
        std::cout << "Please input phone number or full name of person" << std::endl;
        std::cin.ignore();
        std::getline(std::cin,request);
        std::cout << request << std::endl;
        auto* person = new Person;
        if(isPhoneNumber(request)){
            bool phoneSet = person->setPhoneNumber(request);
            if(phoneSet) {
                auto it = reversPhoneBook.find(request);
                if(it!=reversPhoneBook.end()){
                    person->setName(it->second);
                    person->smsToNumber();
                } else {
                    std::cout<<"This number is not listed!"<<std::endl;
                }
            }

        } else{
            bool nameSet = person->setName(request);
            if(nameSet){
                auto it = phoneBook.find(request);
                if(it!=phoneBook.end()){
                    person->setPhoneNumber(it->second);
                    person->smsToNumber();
                } else {
                    std::cout<<"This person is not listed!"<<std::endl;
                }
            }
        }
        delete person;
    }
private:
    bool isPhoneNumber(std::string someRequest){
        if(someRequest[0]=='+'){
            return true;
        }else{
            return false;
        }
    }



};

int main() {
//    for(char i = 0;i<255;i++){
//        std::cout << i;
//    }
    auto* mobile = new Mobile;
    std::string request;
    mobile->readPhoneBook("PhoneBook.txt");
    mobile->showList();
    std::cout << QUESTION << std::endl;
    std::cin >> request;
    while (request!="exit"){
        if(request == "add"){
            mobile->addPerson("PhoneBook.txt");
        } else if(request == "call"){
            mobile->callMobile();
        } else if(request == "sms"){
            mobile->smsToMobile();
        } else if(request == "show"){
            mobile->showList();
        }
        std::cout << QUESTION << std::endl;
        std::cin >> request;
    }
    delete mobile;


    return 0;
}
