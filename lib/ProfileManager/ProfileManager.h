#pragma once

#include <Arduino.h>
#include <iostream>
// #include <chrono>
#include <string>
#include <algorithm>
#include <fstream>

#include <thread>

/*

# Chromium Trace Event Profiling Tool

The Chromium Trace Event Profiling Tool (TEP) offers a multifaceted approach to analyzing performance within Chromium-based browsers.
TEP leverages low-overhead instrumentation at the C++ and JavaScript levels, capturing detailed event data across multiple processes and threads.
This data is rendered as a hierarchical timeline, enabling researchers and developers to visualize the temporal relationships between events,
such as method calls, task execution, and resource loading.
This granular visibility facilitates in-depth investigations of browser behavior, performance bottlenecks, and potential resource inefficiencies.
Moreover, TEP supports custom instrumentation, allowing researchers to tailor the captured data to specific research questions or debugging needs.

1. How to access it:

 - Open a Chromium browser
 - Type about::tracing in the address bar and press Enter

2. Load a trace file:
 
 - Click the "Load" button on the left sidebar ( or drag a file to the main window )
 - Select the trace file you want to analyze (in this case, 'results.json')
 - Click "Open"

3. View events and processes:

 - The timeline will display recorded events in chrological order
 - navigate between processes and threads using the dropdown menu above the timeline

4. Filter events (optional):

 - Use the filtering options:
    * Type a keyword in the search bar
    * Click categories in the "View options" panel
    * Right-click on the timeline and select "Filter"

5. Analize and visalize:

 - Zoom in and out of the timeline to inspect events in detail
 - Hover over events to see more information
 - Use the "Analyze" tab for performance metrics and analysis
 - Export visualizations as images or SVG files (right-click on the timeline)

*/

/*

# RAII (Resource Aquisition Is Initialization)



*/

/* 

# MACROS

#define PROFILING 0
#if PROFILING
#define PROFILE_SCOPE(name) ProfileTimer timer##__LINE_(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#endif

# USAGE EXAMPLE

namespace Profile{
    void Function(){

        PROFILE_FUNCTION();

        for (int i =0; i < 1000, i++)
            std::cout << i << std::endl;
    }
}
int main(){

    ProfileManager::Get().BeginSession("Profile");
    Profile::Function();
    ProfileManager::Get().EndSession("Profile");

    std::cin.get();
}

*/

/*Commented
// Represents the result of a profiled section
struct ProfileResult
{
    std::string Name;      // Name of the profiled section
    long long Start, End;  // Start and end timestamps in microseconds
    uint32_t ThreadID;     // ID of the thread where the profile was captured
};

// Represents a profiling session
struct ProfileSession
{
    std::string Name;      // Name of the profiling session
};

// Manages the profiling process
class ProfileManager
{
private:
    ProfileSession* _CurrentSession;   // Current profiling session
    std::ofstream _OutputStream;       // Output stream for writing profiling results to a file
    int _ProfileCount;                 // Count of profiled sections

public:
    // Constructor initializes the ProfileManager
    ProfileManager()
        : _CurrentSession(nullptr), _ProfileCount(0)
    {
    }

    // Begins a new profiling session
    void BeginSession(const std::string& name, const std::string& filepath = "./extra/results.json")
    {
        _OutputStream.open(filepath);
        WriteHeader();  // Writes the JSON header to the output file
        _CurrentSession = new ProfileSession{ name };
    }

    // Ends the current profiling session
    void EndSession()
    {
        WriteFooter();  // Writes the JSON footer to the output file
        _OutputStream.close();
        delete _CurrentSession;  // Frees the memory allocated for the current session
        _CurrentSession = nullptr;
        _ProfileCount = 0;
    }

    // Writes a profile result to the output file
    void WriteProfile(const ProfileResult& result)
    {
        if (_ProfileCount++ > 0)
            _OutputStream << ",";  // Add a comma if it's not the first profile result

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');  // Replace double quotes with single quotes

        // Write the profile result in JSON format
        _OutputStream << "{";
        _OutputStream << "\"cat\":\"function\",";
        _OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        _OutputStream << "\"name\":\"" << name << "\",";
        _OutputStream << "\"ph\":\"X\",";
        _OutputStream << "\"pid\":0,";
        _OutputStream << "\"tid\":" << result.ThreadID << ",";
        _OutputStream << "\"ts\":" << result.Start;
        _OutputStream << "}";

        _OutputStream.flush();  // Flush the output stream
    }

    // Writes the JSON header to the output file
    void WriteHeader()
    {
        _OutputStream << "{\"otherData\": {},\"traceEvents\":[";
        _OutputStream.flush();
    }

    // Writes the JSON footer to the output file
    void WriteFooter()
    {
        _OutputStream << "]}";
        _OutputStream.flush();
    }

    // Gets the singleton instance of ProfileManager
    static ProfileManager& Get()
    {
        static ProfileManager instance;
        return instance;
    }
};

// A simple timer for profiling code sections
class ProfileTimer
{
public:
    // Constructor starts the timer
    ProfileTimer(const char* name) : _Name(name), _Stopped(false)
    {
        _StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    // Destructor stops the timer if not stopped explicitly
    ~ProfileTimer()
    {
        if (!_Stopped)
        {
            Stop();
        }
    }

    // Stops the timer and writes the profile result
    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        // Convert timestamps to microseconds
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        // Print the elapsed time to the console
        std::cout << _Name << ": " << (end - start) << "us\n";

        // Get the ID of the current thread
        uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

        // Write the profile result to the output file
        ProfileManager::Get().WriteProfile({ _Name, start, end, threadID });

        _Stopped = true;
    }

private:
    const char* _Name;  // Name of the profiled section
    bool _Stopped;      // Flag indicating whether the timer has been stopped
    std::chrono::time_point<std::chrono::high_resolution_clock> _StartTimepoint;  // Start time of the timer
};


// template <typename func>
// void TestDecorator(func T){

//     std::cout << "Initiating Test\n";
//     {
//         ProfileTimer timer;
//         T();
//     }
//     std::cout << "Finalizing Test\n";
// };
*/