#ifndef ACL_PERFORMANCE_PROFILER_HPP
#define ACL_PERFORMANCE_PROFILER_HPP

#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <algorithm>

namespace acl
{
    struct ProfileResult
    {
        std::string mTitle;
        long long mStart;
        long long mEnd;
        std::uint32_t mThreadID;

    };

    struct ProfilerSession
    {
        std::string mTitle;
    };

    class PerformanceProfiler
    {
    private:
        ProfilerSession* mCurrentSession;
        std::ofstream mOutputFileStream;
        int mProfileCount;

    private:
        void WriteHeader() {
            mOutputFileStream << "{\"otherData\": {}, \"traceEvents\": [";
            mOutputFileStream.flush();
        }

        void WriteFooter() {
            mOutputFileStream << "]}";
            mOutputFileStream.flush();
        }

    public:
        PerformanceProfiler()
            : mCurrentSession(nullptr), mProfileCount(0)
        {
        }

        void BeginSession(const std::string& aName, const std::string& aOutputFilePath = "./result.json") {
            mOutputFileStream.open(aOutputFilePath);
            WriteHeader();
            mCurrentSession = new ProfilerSession{aName};
        }

        void WriteProfileInfo(const ProfileResult& aProfileResult) {
            if (mProfileCount++ > 0) mOutputFileStream << ",";

            std::string title = aProfileResult.mTitle;
            std::replace(title.begin(), title.end(), '"', '\'');

            mOutputFileStream << "{";
            mOutputFileStream << "\"cat\":\"function\",";
            mOutputFileStream << "\"dur\":" << (aProfileResult.mEnd - aProfileResult.mStart) << ',';
            mOutputFileStream << "\"name\":\"" << title << "\",";
            mOutputFileStream << "\"ph\":\"X\",";
            mOutputFileStream << "\"pid\":0,";
            mOutputFileStream << "\"tid\":" << aProfileResult.mThreadID << ",";
            mOutputFileStream << "\"ts\":" << aProfileResult.mStart;
            mOutputFileStream << "}";

            mOutputFileStream.flush();
        }

        void EndSession() {
            WriteFooter();
            mOutputFileStream.close();
            delete mCurrentSession;
            mCurrentSession = nullptr;
            mProfileCount = 0;
        }

        static PerformanceProfiler& Get() {
            static PerformanceProfiler instance;
            return instance;
        }
    };

    class PerformanceTimer
    {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> mStartTimePoint;
        const char* mTitle = nullptr;
        bool mStopped;

    public:
        PerformanceTimer(const char* aTitle)
            : mTitle(aTitle), mStopped(false)
        {
            mStartTimePoint = std::chrono::high_resolution_clock::now();
        }

        ~PerformanceTimer() {
            if (!mStopped) Stop();
        }

        void Stop() {
            auto endTimePoint = std::chrono::high_resolution_clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTimePoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

            uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
            PerformanceProfiler::Get().WriteProfileInfo({ mTitle, start, end, threadID });

            mStopped = true;
        }
    };

#define BEGIN_PROFILE_SESSION(title, filename) ::acl::PerformanceProfiler::Get().BeginSession(title, filename)
#define END_PROFILE_SESSION() ::acl::PerformanceProfiler::Get().EndSession()
#define SET_PROFILE_SCOPE(title) ::acl::PerformanceTimer timer##__LINE__(title);
#define PROFILE_FUNCTION() SET_PROFILE_SCOPE(__FUNCSIG__)

}

#endif // !ACL_PERFORMANCE_PROFILER_HPP
