#include <string>

namespace connection
{
    String postNewSensor(const String value);
    String postSensorData(const String apikey, int volume, int temperature, int humidity, const String MAC);
}
