#include <rhoban_utils/angle.h>
#include "rhoban_utils/history/history.h"
#include "rhoban_utils/util.h"

namespace rhoban_utils
{
HistoryDouble::HistoryDouble(double window) : History(window)
{
}

HistoryDouble::TimedValue HistoryDouble::readValueFromStream(std::istream& is, bool binary)
{
  TimedValue value;

  if (binary)
  {
    is.read((char*)&value.first, sizeof(double));
    is.read((char*)&value.second, sizeof(double));
  }
  else
  {
    is >> value.first >> value.second;
  }

  return value;
}

void HistoryDouble::writeValueToStream(const TimedValue& value, std::ostream& os, bool binary)
{
  if (binary)
  {
    os.write((const char*)&(value.first), sizeof(double));
    os.write((const char*)&(value.second), sizeof(double));
  }
  else
  {
    os << std::setprecision(15) << value.first << " " << std::setprecision(15) << value.second << std::endl;
  }
}

HistoryAngle::HistoryAngle(double window) : HistoryDouble(window)
{
}

double HistoryAngle::doInterpolate(double valLow, double wLow, double valUp, double wUp) const
{
  Angle angleLow(rad2deg(valLow));
  Angle angleUp(rad2deg(valUp));
  auto result = Angle::weightedAverage(angleLow, wLow, angleUp, wUp);

  return deg2rad(result.getSignedValue());
}

void HistoryCollection::loadReplays(const std::string& filePath)
{
  std::ifstream file(filePath.c_str());
  // Check file
  if (!file.is_open())
  {
    throw std::runtime_error("HistoryCollection unable to read file: '" + filePath + "'");
  }

  // Check file format
  if (file.peek() == '#')
  {
    // Textual format
    while (true)
    {
      while (file.peek() == ' ' || file.peek() == '\n')
      {
        file.ignore();
      }
      if (!file.good() || file.peek() == EOF)
      {
        break;
      }
      // Extract key name
      std::string name;
      if (file.peek() == '#')
      {
        file.ignore();
        file >> name;
      }
      else
      {
        throw std::runtime_error("HistoryCollection malformed file: '" + filePath + "'");
      }
      // Retrieve all data for current key
      _histories[name]->loadReplay(file);
      std::cout << "Loading " << name << " with " << _histories[name]->size() << " points" << std::endl;
    }
  }
  else
  {
    // Binary format
    while (true)
    {
      if (!file.good() || file.peek() == EOF)
      {
        break;
      }
      size_t length = 0;
      char buffer[256];
      file.read((char*)&length, sizeof(size_t));
      file.read(buffer, length);
      buffer[length] = '\0';
      std::string name(buffer);
      // Retrieve all data for current key
      _histories[name]->loadReplay(file, true, 0.0);
      std::cout << "Loading " << name << " with " << _histories[name]->size() << " points" << std::endl;
    }
  }
  // Start replay
  // XXX ???
  // _replayTimestamp = _histories["read:head_pitch"].front().first;

  // Close read file
  file.close();
}

void HistoryCollection::startNamedLog(const std::string& filePath)
{
  for (auto& it : _histories)
  {
    it.second->startNamedLog(filePath);
  }
}

void HistoryCollection::stopNamedLog(const std::string& filePath)
{
  /// First, freeze all histories for the session name
  for (auto& it : _histories)
  {
    it.second->freezeNamedLog(filePath);
  }
  // Open log file
  std::ofstream file(filePath.c_str());
  // Check file
  if (!file.is_open())
  {
    throw std::runtime_error(DEBUG_INFO + "unable to write to file '" + filePath + "'");
  }
  /// Second write logs
  for (auto& it : _histories)
  {
    size_t length = it.first.length();
    file.write((const char*)(&length), sizeof(size_t));
    file.write((const char*)(it.first.c_str()), length);
    it.second->closeFrozenLog(filePath, file);
  }
}

}  // namespace rhoban_utils
