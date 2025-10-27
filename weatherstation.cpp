// CIS 476 Presentation
// Weather Station Observer Pattern
// Gunnar Bergstrom

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Observers
// Temperature observer receives both Celsius and Farenheit
class TemperatureObserver {
public:
    virtual void onTemperatureChange(float celsius, float fahrenheit) = 0;
    virtual ~TemperatureObserver() = default;
};

// Weather condition observer
class ConditionObserver {
public:
    virtual void onConditionChange(const string& condition) = 0;
    virtual ~ConditionObserver() = default;
};

// Observed Structure
class WeatherStation {
private:
    float temperatureC{};
    string condition = "clear";

    vector<TemperatureObserver*> tempObservers;
    vector<ConditionObserver*> condObservers;

public:
    // Observer management (Temperature)
    void addTemperatureObserver(TemperatureObserver* observer) {
        tempObservers.push_back(observer);
    }
    void removeTemperatureObserver(TemperatureObserver* observer) {
        tempObservers.erase(remove(tempObservers.begin(), tempObservers.end(), observer), tempObservers.end());
    }

    // Observer management (Conditions)
    void addConditionObserver(ConditionObserver* observer) {
        condObservers.push_back(observer);
    }
    void removeConditionObserver(ConditionObserver* observer) {
        condObservers.erase(remove(condObservers.begin(), condObservers.end(), observer), condObservers.end());
    }

    // Notify functions
    void notifyTemperatureObservers() {
        float fahrenheit = (temperatureC * 9.0f / 5.0f) + 32.0f; // changes Celsius to Farenheit 
        for (auto* obs : tempObservers) {
            obs->onTemperatureChange(temperatureC, fahrenheit);
        }
    }

    void notifyConditionObservers() {
        for (auto* obs : condObservers) {
            obs->onConditionChange(condition);
        }
    }

    // State changes
    void setTemperature(float newTempC) {
        cout << "\n[WeatherStation] New temperature: " << newTempC << "°C" << endl;
        temperatureC = newTempC;
        notifyTemperatureObservers();
    }

    void setCondition(const string& newCondition) {
        cout << "\n[WeatherStation] Weather condition changed to: " << newCondition << endl;
        condition = newCondition;
        notifyConditionObservers();
    }
};

// Concrete Observers
// Phone only shows temperature
class PhoneDisplay : public TemperatureObserver {
public:
    void onTemperatureChange(float celsius, float fahrenheit) override {
        cout << "[PhoneDisplay] Temp: " << celsius << "°C / " << fahrenheit << "°F" << endl;
    }
};

// Laptop shows both temperature and weather conditions
class LaptopDisplay : public TemperatureObserver, public ConditionObserver {
public:
    void onTemperatureChange(float celsius, float fahrenheit) override {
        cout << "[LaptopDisplay] Temp: " << celsius << "°C / " << fahrenheit << "°F" << endl;
    }

    void onConditionChange(const string& condition) override {
        cout << "[LaptopDisplay] Condition updated: " << condition << endl;
    }
};

// Alert system only reacts to weather condition changes
// gives ALERT when conditions are severe (heavy rain, snow and ice, or thunderstorms with hail)
class AlertSystem : public ConditionObserver {
public:
    void onConditionChange(const string& condition) override {
        if (condition == "Heavy Rain" || condition == "Snow and Ice" || condition == "Thuderstorms with Hail") {
            cout << "[AlertSystem] ALERT: Severe weather detected (" << condition << ")" << endl;
        } else {
            cout << "[AlertSystem] Conditions normal (" << condition << ")." << endl;
        }
    }
};

// Main
int main() {
    WeatherStation station;

    PhoneDisplay phone;
    LaptopDisplay laptop;
    AlertSystem alert;

    // Register observers
    station.addTemperatureObserver(&phone);
    station.addTemperatureObserver(&laptop);
    station.addConditionObserver(&laptop);
    station.addConditionObserver(&alert);

    // Simulate weather changes
    station.setTemperature(25.0f);
    station.setCondition("Clear");

    station.setTemperature(15.0f);
    station.setCondition("Heavy Rain");

    station.setTemperature(-5.0f);
    station.setCondition("Snow and Ice");

    station.setCondition("Clear");

    return 0;
}
