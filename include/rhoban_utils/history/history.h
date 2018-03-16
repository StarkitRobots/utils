#pragma once

#include <cmath>
#include <deque>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <fstream>

namespace rhoban_utils
{

/**
 * History
 *
 * Class for queue past value and 
 * interpole them back in the past.
 */
class History
{
    public:
        enum ValueType {
            Number=0,
            AngleRad=1
        };

        typedef std::pair<double,double> TimedValue;

        /**
         * Initialization in timestamp duration
         */
        History(double window = 2.0);

        /**
         * Sets the history window size
         */
        void setWindowSize(double window);

        /**
         * Return the number of internal stored data
         */
        size_t size() const;

        /**
         * Return first and last recorded point
         */
        const TimedValue& front() const;
        const TimedValue& back() const;

        /**
         * Insert a new value in the container
         * with given timestamp and value
         */
        void pushValue(double timestamp, double value);

        /**
         * Return either the nearest value or the
         * linear interpolated value associated with
         * given timestamp
         */
        double interpolate(double timestamp, ValueType valueType=Number) const;

        /**
         * Enable to logging mode.
         */
        void startLogging();

        /**
         * Stop logging and dump all recorded 
         * values into given output stream.
         * If binary is true, log file is written in
         * binary format
         */
        void stopLogging(std::ostream& os, bool binary = false);

        /**
         * Open a log session with the given name, throws a logic_error if a
         * session with the given name is already opened
         */
        void startNamedLog(const std::string & sessionName);


        /**
         * Move the session with the given name from the active set to the
         * frozen set. Low time consumption.
         */ 
        void freezeNamedLog(const std::string & sessionName);

        /**
         * Close a log session with the given name, throws a logic_error if
         * there is no frozen session with the given name opened. High time
         * consumption.
         */
        void closeFrozenLog(const std::string & sessionName, std::ostream& os);

        /**
         * Read data from given input stream 
         * until either the stream end or the first
         * commented "#" line.
         * If binary is true, log file is read in
         * binary format
         * Optional time shift is apply on read timestamp
         */
        void loadReplay(
            std::ifstream& is, 
            bool binary = false, 
            double timeShift = 0.0);

    private:

        /**
         * Write the history to the provided stream
         */
        static void writeBinary(const std::deque<TimedValue> & values, std::ostream & os);

        /**
         * Mutex for concurent access
         */
        mutable std::mutex _mutex;

        /**
         * If true, the instance is in logging state
         * and does not erase any data
         */
        bool _isLogging;

        /**
         * When logging, save first time
         * to not write buffered data
         */
        double _startLoggingTime;

        /**
         * Rolling buffer size in timestamp
         */
        double _windowSize;

        /**
         * Values container indexed 
         * by their timestamp
         */
        std::deque<TimedValue> _values;

        /**
         * Named log sessions to which the object is actively writting
         */
        std::map<std::string,std::unique_ptr<std::deque<TimedValue>>> _activeLogs;
        /**
         * Named log sessions waiting to be written
         */
        std::map<std::string,std::unique_ptr<std::deque<TimedValue>>> _frozenLogs;
};

}
