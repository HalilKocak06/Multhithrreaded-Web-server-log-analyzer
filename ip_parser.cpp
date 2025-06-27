#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<map>


//CTRL + ALT + N İLE ÇALIŞTIR(CODE_RUNNER)
int main()
{
    std::ifstream logFile("access.log"); //Dosyayı açar 
    std::string line;
    std::map<std::string, int> ipCount;
    std::map<std::string, int> endpointCount;
    std::map<int , int > hourCount;

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

        ipCount[ip]++; //IP sayacını arttırır.

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
            endpointCount[endpoint]++;
        }

        std::cout<< "IP:" << ip << " | Endpoint: " << endpoint << std::endl;

        size_t timeStart=line.find('['); //Burada bunu bul diyor ilk
        size_t timeEnd= line.find(']'); //Ardından bunu bul diyor son.

        if(timeStart != std::string::npos && timeEnd != std::string::npos)
        {
            std::string timestamp = line.substr(timeStart + 1 , timeEnd - timeStart - 1);
            size_t colonPos = timestamp.find(':');

            if(colonPos != std::string::npos)
            {
                std::string hourStr = timestamp.substr(colonPos + 1 , 2); //saaat kısmı
                int hour = std::stoi(hourStr); //string to int 
                hourCount[hour]++;
            }

        }

    }

    logFile.close();

    std::cout << " \n--- IP Address and Request Counts ---\n";
    for(const auto& pair : ipCount)
    {
        std::cout << "IP: " << pair.first << " | Count: " << pair.second << std::endl;
    }

    std::cout<< "\n--- Endpoint and Request Counts ---\n";
    for(const auto& pair : endpointCount)
    {
        std::cout << "EndPoint:" << pair.first <<  " | Count:" << pair.second << std::endl;
    }

    std::cout<<"\n--- Traffic Hours ---\n";

    for(const auto& pair : hourCount)
    {
        std::cout << "Hours: " <<pair.first << ":00 | Count:" << pair.second << std::endl;
    }

    return 0;
}