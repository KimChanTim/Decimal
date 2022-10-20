#include <iostream>
#include <cstring>

#define INT(c) (c - '0')
#define CHAR(c) (c + '0')

class Decimal {
    char *value;
    size_t length;
    size_t allocated;
    bool sign = false;

    Decimal& sum(const Decimal&);
    Decimal& diff(const Decimal&);

    void reverse();
public:
    Decimal();
    //Decimal(const Decimal&);
    Decimal(int);
    Decimal(std::string);
    ~Decimal();

    //bool operator>=(const Decimal&) const;

    //const Decimal abs() const;

    //friend std::ostream& operator<<(std::ostream& os, const Decimal& dec);
    Decimal& operator=(const Decimal&);
    Decimal& operator+=(const Decimal&);
    const Decimal operator+(const Decimal&);

    void PrintValue();
};

void Decimal::reverse() {
    for (int i = 0, j = length - 1; i < j; i++, j--) {
        char tmp = value[i];
        value[i] = value[j];
        value[j] = tmp;
    }
};

Decimal::Decimal() {
    allocated = 16;
    length = 1;
    sign = false;
    value = new char[allocated];
    value[0] = '0';
    value[1] = '\0';
}

Decimal::Decimal(int val) {
    allocated = 16;
    value = new char[allocated];
    if (val < 0) {
        sign = true;
        val = -val;
    }
    sprintf(value, "%d", val);
    length = strlen(value);
    reverse();
}

Decimal::Decimal(std::string str) {
    if (str[0] == '-') {
        sign = true;
        length = str.length() - 1;
        allocated = length;
        value = new char[allocated];
        strcpy(value, str.c_str() + 1);
        reverse();
        return;
    }
    length = str.length();
    allocated = length;
    value = new char[allocated];
    strcpy(value, str.c_str());
    reverse();
}

Decimal::~Decimal() {
    delete [] value;
}

Decimal& Decimal::operator=(const Decimal& val) {
    if (this == &val)
        return *this;
    delete value;
    value = new char[val.allocated];
    strcpy(value, val.value);
    allocated = val.allocated;
    length = val.length;
    sign = val.sign;
    return *this;
}

Decimal& Decimal::sum(const Decimal& val) {
    char toAdd = 0;
    size_t max_size = (allocated >= val.allocated ? allocated : val.allocated);
    size_t max_length = (length >= val.length ? length : val.length);

    for(int i = 0; i < max_length; i++) {
        int tmp = (i < length ? INT(value[i]) : 0) +
                (i < val.length ? INT(val.value[i]) : 0) + toAdd;
        toAdd = tmp / 10;
        value[i] = CHAR(tmp % 10);
    }
    if (toAdd) {
        if (max_size == max_length) {
            char* res = new char[max_length + 1];
            strcpy(res, value);
            res[max_length] = '1';
            delete value;
            value = res;
            return *this;
        }
        value[max_length] = '1';
        return *this;
    }
    return *this;
}

Decimal& Decimal::diff(const Decimal& val) {
    ;
}

Decimal& Decimal::operator+=(const Decimal& val) {
    if (sign == val.sign) {
        return sum(val);
    }
    else {
        return diff(val);
    }
}

const Decimal Decimal::operator+(const Decimal & val) {
    Decimal tmp;
    tmp = *this;
    tmp += val;
    return tmp;
}

void Decimal::PrintValue() {
    reverse();
    std::cout << value << std::endl;
}

/*
bool Decimal::operator>=(const Decimal &val) const {
    if (std::strcmp(value, val.value)) {
        return true;
    }
    int maxSize = allocated >= val.allocated ? allocated : val.allocated;

    for (int i = maxSize - 1; i >= 0; i++) {
        if (value[i] > val.value[i])
            return true;
        else if (value[i] != val.value[i])
            return false;
    }

    return false;
}

std::ostream &operator<<(std::ostream &os, const Decimal &dec) {
    return os << dec.value;
}

Decimal::Decimal(const Decimal & dec) {
    sign = dec.sign;
    allocated = dec.allocated;
    value = new char[allocated];
    strcpy(value, dec.value);
    length = dec.length;
}
*/

int main() {
    Decimal a("123");
    Decimal b("47");
    Decimal d;
    Decimal c(12);
    a = a + b;
    a.PrintValue();
    return 0;
}
