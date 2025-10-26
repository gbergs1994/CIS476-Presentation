// CIS 476 Presentation
// Weather Station Observer Pattern
// Gunnar Bergstrom

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// --- Observer Interfaces ---

// Temperature observer now receives both °C and °F
class TemperatureObserver {
public:
    virtual void onTemperatureChange(float celsius, float fahrenheit) = 0;
    virtual ~TemperatureObserver() = default;
};

// Weather condition observer
class ConditionObserver {
public:
    virtual void onConditionChange(const std::string& condition) = 0;
    virtual ~ConditionObserver() = default;
};

// --- Subject (Observable) ---
class WeatherStation {
private:
    float temperatureC{};
    std::string condition = "clear";

    std::vector<TemperatureObserver*> tempObservers;
    std::vector<ConditionObserver*> condObservers;

public:
    // Observer management
    void addTemperatureObserver(TemperatureObserver* observer) {
        tempObservers.push_back(observer);
    }
    void removeTemperatureObserver(TemperatureObserver* observer) {
        tempObservers.erase(std::remove(tempObservers.begin(), tempObservers.end(), observer), tempObservers.end());
    }

    void addConditionObserver(ConditionObserver* observer) {
        condObservers.push_back(observer);
    }
    void removeConditionObserver(ConditionObserver* observer) {
        condObservers.erase(std::remove(condObservers.begin(), condObservers.end(), observer), condObservers.end());
    }

    // Notify functions
    void notifyTemperatureObservers() {
        float fahrenheit = (temperatureC * 9.0f / 5.0f) + 32.0f;
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
        std::cout << "\n[WeatherStation] New temperature: " << newTempC << "°C\n";
        temperatureC = newTempC;
        notifyTemperatureObservers();
    }

    void setCondition(const std::string& newCondition) {
        std::cout << "\n[WeatherStation] Weather condition changed to: " << newCondition << "\n";
        condition = newCondition;
        notifyConditionObservers();
    }
};

// --- Concrete Observers ---

// Phone only shows temperature
class PhoneDisplay : public TemperatureObserver {
public:
    void onTemperatureChange(float celsius, float fahrenheit) override {
        std::cout << "[PhoneDisplay] Temp: " << celsius << "°C / " << fahrenheit << "°F\n";
    }
};

// Laptop shows both temperature and weather conditions
class LaptopDisplay : public TemperatureObserver, public ConditionObserver {
public:
    void onTemperatureChange(float celsius, float fahrenheit) override {
        std::cout << "[LaptopDisplay] Temp: " << celsius << "°C / " << fahrenheit << "°F\n";
    }

    void onConditionChange(const std::string& condition) override {
        std::cout << "[LaptopDisplay] Condition updated: " << condition << "\n";
    }
};

// Alert system only reacts to weather condition changes
class AlertSystem : public ConditionObserver {
public:
    void onConditionChange(const std::string& condition) override {
        if (condition == "Heavy Rain" || condition == "Snow and Ice" || condition == "Thuderstorms with Hail") {
            std::cout << "[AlertSystem] ALERT: Severe weather detected (" << condition << ")\n";
        } else {
            std::cout << "[AlertSystem] Conditions normal (" << condition << ").\n";
        }
    }
};

// --- Example usage ---
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
