#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<thread>
#include<mutex>
#include<map>
#include<chrono>


std::mutex mtx;

void processLines(
    const std::vector<std::string>& lines,
    int start,
    int end,
    std::map<std::string,int>& ipCount,
    std::map<std::string,int>& endpointCount,
    std::map<int, int>& hourCount
) {
    for(int i = start; i < end; ++i)
    {
        const std::string& line = lines[i];

        std::istringstream iss(line);
        std::string ip;
        iss >> ip ;

        // ortak yapılar mutex ile güncelliyoruz.
        {
            std::lock_guard<std::mutex> lock(mtx);
            ipCount[ip]++;
        }

        //Endpoint
        size_t quoteStart = line.find('"');
        size_t quoteEnd = line.find('"',quoteStart+1);
        std::string endpoint = "N/A";

        if(quoteStart != std::string::npos && quoteEnd != std::string::npos)
        {
            std::string requestPart = line.substr(quoteStart+1, quoteEnd-quoteStart-1);
            std::istringstream reqStream(requestPart);
            std::string method, url;
            reqStream >> method >> url;
            endpoint = url;

            std::lock_guard<std::mutex> lock(mtx);
            endpointCount[endpoint]++;
        }

        //Saat bilgisi 
        size_t timeStart = line.find('[');
        size_t timeEnd = line.find(']');
        if(timeStart != std::string::npos && timeEnd != std::string::npos)
        {
            std::string timestamp = line.substr(timeStart +1 , timeEnd - timeStart -1);
            size_t colonPos = timestamp.find(':');
            if(colonPos != std::string::npos)
            {
                std::string hourStr = timestamp.substr(colonPos + 1 , 2);
                int hour = std::stoi(hourStr);
                std::lock_guard<std::mutex> lock(mtx);
                hourCount[hour]++;
            }
        }

    }
}

int main()
{
    std::ifstream logFile("access_generated.log");
    std::vector<std::string> logLines ;
    std::string line;

    auto startTime = std::chrono::high_resolution_clock::now();

    if(!logFile.is_open())
    {
        std::cerr << "Log file did not open ! \n" ;
        return 1 ;
    }

    while(std::getline(logFile, line)) //getLine fonksiyonu satır satır okur ve o satırı line olarak kayıt eder .
    {
        logLines.push_back(line); // Her satırı bellekteki vektöre(logLines)'a ekler.
    }

    logFile.close();

    std::map<std::string, int> ipCount;
    std::map<std::string, int> endpointCount;
    std::map<int, int> hourCount;

    int threadCount = 8;
    int totalLines = logLines.size();
    int chunkSize = totalLines / threadCount ; 

    std::vector<std::thread> threads;

    for(int i=0; i<threadCount; ++i)
    {
        int start = i * chunkSize;
        int end = (i == threadCount -1 ) ? totalLines : start + chunkSize;

        threads.emplace_back(processLines, std::ref(logLines), start, end, std::ref(ipCount), std::ref(endpointCount), std::ref(hourCount));

    }

    for(auto& t : threads)
    {
        t.join();
    }

    std::cout << "\n--- IP Count ---\n";
    for (const auto& pair : ipCount)
        std::cout << "IP: " << pair.first << " | Count: " << pair.second << std::endl;

    std::cout << "\n--- Endpoint Count ---\n";
    for (const auto& pair : endpointCount)
        std::cout << "Endpoint: " << pair.first << " | Count: " << pair.second << std::endl;

    std::cout << "\n--- Hourly Traffic ---\n";
    for (const auto& pair : hourCount)
        std::cout << "Hour: " << pair.first << ":00 | Count: " << pair.second << std::endl;


    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    std::cout << "\nTotal work time: " << elapsed.count() << " secodns\n";


    // IP CSV
    std::ofstream ipFile("ip_stats.csv");
    ipFile << "IP,Count\n";
    for(const auto& pair : ipCount)
    {
        ipFile << pair.first << "," << pair.second << "\n";

    }
    ipFile.close();

    //Endpoint CSV
    std::ofstream endpointFile("endpoint_stats.csv");
    endpointFile << "Endpoint,Count\n";
    for(const auto& pair : endpointCount)
    {
        endpointFile << pair.first << "," << pair.second <<"\n";
    }
    endpointFile.close();

    //Hour CSV
    std::ofstream hourFile("hourly_stats.csv");
    hourFile << "Hour,Count\n";
    for(const auto& pair : hourCount)
    {
        hourFile << pair.first <<","<<pair.second <<"\n";
    }
    hourFile.close();

    // std::cout<< "--- First 5 Log Line ----\n";
    // for(int i=0; i<std::min(3,(int)logLines.size()); ++i) 
    // {
    //     std::cout << logLines[i] << std::endl;
    // }

    return 0 ; 
    
}