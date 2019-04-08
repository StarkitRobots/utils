#pragma once

#include <cmath>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <fstream>
#include "rhoban_utils/util.h"

namespace rhoban_utils
{
class HistoryBase
{
public:
  virtual void setWindowSize(double window) = 0;
  virtual size_t size() const = 0;
  virtual void startLogging() = 0;
  virtual void stopLogging(std::ostream& os, bool binary = false) = 0;
  virtual void startNamedLog(const std::string& sessionName) = 0;
  virtual void freezeNamedLog(const std::string& sessionName) = 0;
  virtual void closeFrozenLog(const std::string& sessionName, std::ostream& os) = 0;
  virtual void loadReplay(std::istream& is, bool binary = false, double timeShift = 0.0) = 0;
  virtual void clear() = 0;
};

/**
 * History
 *
 * Class for queue past value and
 * interpole them back in the past.
 */
template <typename T>
class History : public HistoryBase
{
public:
  typedef std::pair<double, T> TimedValue;

  /**
   * Initialization in timestamp duration
   */
  History(double window) : _mutex(), _isLogging(false), _startLoggingTime(-1.0), _windowSize(window), _values()
  {
  }

  /**
   * This is used to abstract writing/reading data from and to a log stream
   */
  virtual TimedValue readValueFromStream(std::istream& is, bool binary = false) = 0;
  virtual void writeValueToStream(const TimedValue& value, std::ostream& os, bool binary = false) = 0;

  /**
   * Sets the history window size
   */
  void setWindowSize(double window)
  {
    _windowSize = window;
  }

  /**
   * Return the number of internal stored data
   */
  size_t size() const
  {
    return _values.size();
  }

  /**
   * Return first and last recorded point
   */
  const TimedValue& front() const
  {
    if (_values.size() == 0)
    {
      static std::pair<double, double> zero = { 0.0, 0.0 };
      return zero;
    }
    else
    {
      return _values.front();
    }
  }

  const TimedValue& back() const
  {
    if (_values.size() == 0)
    {
      static std::pair<double, double> zero = { 0.0, 0.0 };
      return zero;
    }
    else
    {
      return _values.back();
    }
  }

  /**
   * Insert a new value in the container
   * with given timestamp and value
   */
  void pushValue(double timestamp, T value)
  {
    // Lock
    _mutex.lock();
    // Check that timestamp is increasing
    if (_values.size() > 0 && timestamp < _values.back().first)
    {
      _mutex.unlock();
      throw std::logic_error("History invalid timestamp");
    }
    if (_values.size() > 0 && timestamp == _values.back().first)
    {
      _mutex.unlock();
      return;
    }
    std::pair<double, T> entry(timestamp, value);
    // Insert the value
    _values.push_back(entry);
    // Shrink the queue if not in logging mode
    while (!_isLogging && !_values.empty() && (_values.back().first - _values.front().first > _windowSize))
    {
      _values.pop_front();
    }
    // Set the startLoggingTime to the first
    // data timestampt pushed after startLogging() is called
    if (_isLogging && _startLoggingTime < 0.0)
    {
      _startLoggingTime = timestamp;
    }
    // Write new entries for all named sessions
    for (auto& namedLog : _activeLogs)
    {
      namedLog.second->push_back(entry);
    }
    // Unlock
    _mutex.unlock();
  }

  /**
   * Return either the nearest value or the
   * linear interpolated value associated with
   * given timestamp
   */
  virtual T interpolate(double timestamp) const
  {
    // Lock
    _mutex.lock();

    // Degererate failback cases
    if (_values.size() == 0)
    {
      _mutex.unlock();
      return 0.0;
    }
    else if (_values.size() == 1)
    {
      _mutex.unlock();
      return _values.front().second;
    }
    else if (timestamp <= _values.front().first)
    {
      _mutex.unlock();
      return _values.front().second;
    }
    else if (timestamp >= _values.back().first)
    {
      _mutex.unlock();
      return _values.back().second;
    }

    // Bijection search
    size_t indexLow = 0;
    size_t indexUp = _values.size() - 1;
    while (indexUp - indexLow > 1)
    {
      size_t indexMiddle = (indexLow + indexUp) / 2;
      if (_values[indexMiddle].first <= timestamp)
      {
        indexLow = indexMiddle;
      }
      else
      {
        indexUp = indexMiddle;
      }
    }

    // Retrieve lower and upper bound values
    double tsLow = _values[indexLow].first;
    double valLow = _values[indexLow].second;
    double tsUp = _values[indexUp].first;
    double valUp = _values[indexUp].second;

    // Unlock
    _mutex.unlock();

    // Weights
    double wLow = (tsUp - timestamp) / (tsUp - tsLow);
    double wUp = (timestamp - tsLow) / (tsUp - tsLow);

    return doInterpolate(valLow, wLow, valUp, wUp);
  }

  /**
   * Do the actual interpolation between valLow and valHigh
   */
  virtual T doInterpolate(T valLow, double wLow, T valUp, double wUp) const
  {
    return wLow * valLow + wUp * valUp;
  }

  /**
   * Enable to logging mode.
   */
  void startLogging()
  {
    _mutex.lock();
    _isLogging = true;
    _startLoggingTime = -1.0;
    _mutex.unlock();
  }

  /**
   * Stop logging and dump all recorded
   * values into given output stream.
   * If binary is true, log file is written in
   * binary format
   */
  void stopLogging(std::ostream& os, bool binary = false)
  {
    _mutex.lock();
    _isLogging = false;
    if (!binary)
    {
      for (const auto& it : _values)
      {
        // Skip data in buffer before logging start
        if (_startLoggingTime >= 0.0 && it.first >= _startLoggingTime)
        {
          // Write ascii data
          writeValueToStream(it, os, false);
        }
      }
    }
    else
    {
      writeBinary(_values, os);
    }
    _mutex.unlock();
  }

  /**
   * Open a log session with the given name, throws a logic_error if a
   * session with the given name is already opened
   */
  void startNamedLog(const std::string& sessionName)
  {
    _mutex.lock();
    if (_activeLogs.count(sessionName) > 0)
    {
      _mutex.unlock();
      throw std::logic_error(DEBUG_INFO + " there is already a session with name '" + sessionName + "'");
    }
    _activeLogs[sessionName] = std::unique_ptr<std::deque<TimedValue>>(new std::deque<TimedValue>());
    _mutex.unlock();
  }

  /**
   * Move the session with the given name from the active set to the
   * frozen set. Low time consumption.
   */
  void freezeNamedLog(const std::string& sessionName)
  {
    _mutex.lock();
    if (_activeLogs.count(sessionName) == 0)
    {
      _mutex.unlock();
      throw std::logic_error(DEBUG_INFO + " there is no active session with name '" + sessionName + "'");
    }
    if (_frozenLogs.count(sessionName) > 0)
    {
      _mutex.unlock();
      throw std::logic_error(DEBUG_INFO + " there is already a frozen session with name '" + sessionName + "'");
    }
    _frozenLogs[sessionName] = std::move(_activeLogs[sessionName]);
    _activeLogs.erase(sessionName);
    _mutex.unlock();
  }

  /**
   * Close a log session with the given name, throws a logic_error if
   * there is no frozen session with the given name opened. High time
   * consumption.
   */
  void closeFrozenLog(const std::string& sessionName, std::ostream& os)
  {
    _mutex.lock();
    if (_frozenLogs.count(sessionName) == 0)
    {
      _mutex.unlock();
      throw std::logic_error(DEBUG_INFO + " there is no open session with name '" + sessionName + "'");
    }
    std::unique_ptr<std::deque<TimedValue>> values = std::move(_frozenLogs[sessionName]);
    _frozenLogs.erase(sessionName);
    _mutex.unlock();

    writeBinary(*values, os);
  }

  /**
   * Read data from given input stream
   * until either the stream end or the first
   * commented "#" line.
   * If binary is true, log file is read in
   * binary format
   * Optional time shift is apply on read timestamp
   */
  void loadReplay(std::istream& is, bool binary = false, double timeShift = 0.0)
  {
    _mutex.lock();
    // Clean the container
    _values.clear();
    // Read the number of data
    size_t size = 0;
    if (binary)
    {
      is.read((char*)&size, sizeof(size_t));
    }
    // Read the input stream
    while (true)
    {
      // Extract one data point
      TimedValue newValue;

      if (binary)
      {
        if (size == 0)
        {
          _mutex.unlock();
          return;
        }

        newValue = readValueFromStream(is, true);
        size--;
      }
      else
      {
        while (is.peek() == ' ' || is.peek() == '\n')
        {
          is.ignore();
        }
        if (is.peek() == '#' || is.peek() == EOF)
        {
          _mutex.unlock();
          return;
        }
        newValue = readValueFromStream(is, false);
      }
      // Apply time shift
      newValue.first += timeShift;

      // Check that timestamp is increasing
      if (_values.size() > 0 && newValue.first <= _values.back().first)
      {
        _mutex.unlock();
        throw std::runtime_error("History invalid timestamp");
      }
      // Insert the value
      _values.push_back(newValue);
    }
  }

  /**
   * Getting all values
   */
  std::deque<TimedValue> getValues()
  {
    return _values;
  }

  /**
   * Clearing history values
   */
  void clear()
  {
    _values.clear();
  }

private:
  /**
   * Write the history to the provided stream
   */
  void writeBinary(const std::deque<TimedValue>& values, std::ostream& os)
  {
    // When writing in binary, use all values
    size_t size = values.size();
    os.write((const char*)&size, sizeof(size_t));
    for (const auto& it : values)
    {
      // Write binary data
      writeValueToStream(it, os, true);
    }
  }

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
  std::map<std::string, std::unique_ptr<std::deque<TimedValue>>> _activeLogs;
  /**
   * Named log sessions waiting to be written
   */
  std::map<std::string, std::unique_ptr<std::deque<TimedValue>>> _frozenLogs;
};

class HistoryDouble : public History<double>
{
public:
  HistoryDouble(double window = 2.0);

  TimedValue readValueFromStream(std::istream& is, bool binary = false);
  void writeValueToStream(const TimedValue& value, std::ostream& os, bool binary = false);
};

class HistoryAngle : public HistoryDouble
{
public:
  HistoryAngle(double window = 2.0);

  double doInterpolate(double valLow, double wLow, double valHigh, double wHigh) const;
};

class HistoryCollection : public std::map<std::string, HistoryBase*>
{
public:
  template <typename T>
  T* get(std::string name)
  {
    if (!_histories.count(name))
    {
      T* h = new T();
      _histories[name] = h;
    }

    T* h = dynamic_cast<T*>(_histories[name]);
    if (h == nullptr)
    {
      std::ostringstream os;
      os << "Asking for history of bad type (" << name << ")";
      throw std::logic_error(os.str());
    }

    return h;
  }

  HistoryDouble* getDouble(std::string name)
  {
    return get<HistoryDouble>(name);
  }

  HistoryAngle* getAngle(std::string name)
  {
    return get<HistoryAngle>(name);
  }

  /**
   * Start and stop (save) a named log session
   */
  void startNamedLog(const std::string& filePath);
  void stopNamedLog(const std::string& filePath);

  /**
   *  Load the replays from a given file stream
   */
  void loadReplays(const std::string& filePath);

protected:
  std::map<std::string, HistoryBase *> _histories;
};

}  // namespace rhoban_utils