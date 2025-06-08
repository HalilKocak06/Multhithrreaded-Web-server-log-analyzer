#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

int main()
{
    std::ifstream logFile("access.log"); //Dosyayı açar 
    std::string line;

    if(!logFile.is_open()) //Dosya var mı kontrol edilir
    {
        std::cerr << "Log file do not open! \n";
        return 1;
    }

    while (std::getline(logFile, line)) //Satırlar okunur
    {
        std::istringstream iss(line); //bu sayede ilk kelime alınır diğerleri alınmaz !!.
        std::string ip;

        //IP adresi satırın başında olduğu için ilk kelime alınır.
        //iss ilk satır okunur
        //ip ikinci satırdır.
        iss >> ip;

        //Endpoint bulmak için çift tırnaklı kısmı ara
        size_t quoteStart = line.find('"'); //ilk tırnağı ara
        size_t quoteEnd = line.find('"', quoteStart + 1); //ikinci tırnağı ara 

        std::string endpoint ="N/A";

        if( quoteStart != std::string::npos && quoteEnd != std::string::npos) //Eğer satırda iki tane tırnak varsa yani böyle ise devam et "  ....   "
        {
            std::string requestPart = line.substr(quoteStart + 1, quoteEnd - quoteStart - 1); //Tırnaktan sonrasında başla ve tırnaktan öncesine kadar hepsini al .
            std::istringstream reqStream(requestPart);
            std::string method, url; //method -->Get , url-->/login kısmı
            reqStream >> method >> url;
            endpoint = url;
        }

        std::cout<< "IP:" << ip << " | Endpoint: " << endpoint << std::endl;


    }

    logFile.close();
    return 0;
}