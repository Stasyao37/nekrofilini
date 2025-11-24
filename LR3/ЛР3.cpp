#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>

class ProgramError : public std::exception {
public:
    const char* what() const noexcept override {
        return "Program error";
    }
};

class InsufficientPrivilegesError : public ProgramError {
public:
    const char* what() const noexcept override {
        return "Insufficient privileges";
    }
};

class ConversionError : public ProgramError {
public:
    const char* what() const noexcept override {
        return "Conversion error";
    }
};

class CannotConvertValueError : public ConversionError {
public:
    const char* what() const noexcept override {
        return "Cannot convert value";
    }
};

class CannotCastToInterfaceError : public ConversionError {
public:
    const char* what() const noexcept override {
        return "Cannot cast to interface";
    }
};

template<typename T>
class PointerArray {
private:
    std::vector<std::shared_ptr<T>> data;

public:
    void add(const std::shared_ptr<T>& ptr) {
        data.push_back(ptr);
    }
    std::shared_ptr<T>& operator[](size_t index) {
        if (index >= data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    size_t size() const {
        return data.size();
    }
};

double calculate_sqrt(double value) {
    if (value < 0) {
        throw std::domain_error("Square root of negative number is not defined");
    }
    return std::sqrt(value);
}

int main() {
    try {
        throw CannotConvertValueError();
    }
    catch (const ProgramError& e) {
        std::cout << "ProgramError: " << e.what() << std::endl;
    }


    PointerArray<ProgramError> ErrArray;
    ErrArray.add(std::make_shared<InsufficientPrivilegesError>(InsufficientPrivilegesError()));
    ErrArray.add(std::make_shared<CannotCastToInterfaceError>(CannotCastToInterfaceError()));
    std::cout << "ProgramError: " << ErrArray[0]->what() << std::endl;

    PointerArray<std::string> stringArray;
    stringArray.add(std::make_shared<std::string>("Hello"));
    stringArray.add(std::make_shared<std::string>("World"));
    std::cout << "stringArray[1] = " << *stringArray[1] << std::endl;

    try {
        double result = calculate_sqrt(-5);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::domain_error& e) {
        std::cout << "Domain error: " << e.what() << std::endl;
    }

    return 0;
}
