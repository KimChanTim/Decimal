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
    Decimal& diff(const Decimal&, bool flag);

    void reverse();
public:
    Decimal();
    Decimal(int);
    Decimal(std::string);
    ~Decimal();

    bool operator==(const Decimal&) const;
    bool operator>=(const Decimal&) const;
    Decimal& operator=(const Decimal&);
    Decimal& operator+=(const Decimal&);
    Decimal& operator-=(const Decimal&);
    Decimal& operator*=(const Decimal&);
    //Decimal& operator/=(const Decimal&);
    const Decimal operator+(const Decimal&);
    const Decimal operator-(const Decimal&);
    const Decimal operator*(const Decimal&);
    //const Decimal operator/(const Decimal&);

    void PrintValue();
    void PrintSign();
    void PrintDecimal();
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

bool Decimal::operator==(const Decimal& val) const {
    if (!std::strcmp(value, val.value)) {
        return true;
    }
    return false;
}

bool Decimal::operator>=(const Decimal &val) const {
    if (!std::strcmp(value, val.value)) {
        return true;
    }
    int max_length = length >= val.length ? length : val.length;

    for (int i = max_length - 1; i >= 0; i--) {
        if (value[i] > val.value[i])
            return true;
        else if (value[i] != val.value[i])
            return false;
    }
    return false;
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
    char* res = new char[max_length];
    strcpy(res, value);
    delete [] value;
    value = res;
    for(int i = 0; i < max_length; i++) {
        int tmp = (i < length ? INT(value[i]) : 0) +
                  (i < val.length ? INT(val.value[i]) : 0) + toAdd;
        toAdd = tmp / 10;
        value[i] = CHAR(tmp % 10);
    }
    if (toAdd) {
        if (max_size == max_length) {
            char* _res = new char[max_length + 1];
            strcpy(_res, value);
            _res[max_length] = '1';
            delete [] value;
            value = _res;
            length++;
            allocated++;
            return *this;
        }
        value[max_length] = '1';
        length = max_length;
        allocated = max_length;
        return *this;
    }
    length = max_length;
    allocated = max_length;
    return *this;
}

Decimal& Decimal::diff(const Decimal& val, bool flag) {
    char toSub = 0;
    size_t max_length = (length >= val.length ? length : val.length);
    char *res = new char[max_length];
    bool key = false;
    if (*this >= val)
        key = true;
    for(int i = 0; i < max_length; i++) {
        int tmp = (key ? 1 : -1) * ((i < length ? INT(value[i]) : 0) -
                                    (i < val.length ? INT(val.value[i]) : 0)) - toSub;
        if (tmp < 0) {
            tmp += 10;
            toSub = 1;
        } else
            toSub = 0;
        res[i] = CHAR(tmp);
    }
    int i = max_length - 1;
    while (res[i] == '0' && i > 0) {
        max_length--;
        res[i] = '\0';
        i--;
    }
    char *_res = new char[max_length];
    strcpy(_res, res);
    delete [] res;
    delete [] value;
    value = _res;
    length = max_length;
    allocated = max_length;
    if (!key && flag)
        sign = val.sign;
    if (!key && !flag)
        sign = !val.sign;
    return *this;
}

Decimal& Decimal::operator+=(const Decimal& val) {
    if (sign == val.sign) {
        return sum(val);
    }
    else {
        return diff(val, true);
    }
}

const Decimal Decimal::operator+(const Decimal & val) {
    Decimal tmp;
    tmp = *this;
    tmp += val;
    return tmp;
}

Decimal& Decimal::operator-=(const Decimal& val) {
    if (sign != val.sign) {
        return sum(val);
    }
    else {
        return diff(val, false);
    }
}

const Decimal Decimal::operator-(const Decimal & val) {
    Decimal tmp;
    tmp = *this;
    tmp -= val;
    return tmp;
}

Decimal& Decimal::operator*=(const Decimal &val) {
    if((length == 1 && value[0] == '0') || (val.length == 1 && val.value[0] == '0')){
        *this = Decimal();
        return *this;
    }
    char *arr = new char[length + val.length];
    for (int i = 0; i < length; i++)
        for (int j = 0; j < val.length; j++){
            arr[i + j] += INT(value[i]) * INT(val.value[j]);
        }
    length += val.length;
    char *res = new char[length];
    delete [] value;
    value = res;
    for (int tmp, i = 0, t = 0; i < length; i++)
    {
        tmp = t + arr[i];
        arr[i] = tmp % 10;
        t = tmp / 10;
        value[i] = CHAR(arr[i]) ;
    }
    for (int i = length - 1; i >= 1 && !arr[i]; i--) {
        value[i] = '\0';
        length--;
    }
    char *_res = new char[length];
    strcpy(_res, value);
    delete [] value;
    value = _res;
    allocated = length;
    if (sign == val.sign)
        sign = false;
    else
        sign = true;
    return *this;
}

const Decimal Decimal::operator*(const Decimal & val) {
    Decimal tmp;
    tmp = *this;
    tmp *= val;
    return tmp;
}
/*
Decimal& Decimal::operator/=(const Decimal &val) {
    if (val.length == 1 && val.value[0] == '0')
        throw("Division By 0");
    if (!(*this >= val)) {
        *this = Decimal();
        return *this;
    }
    if(*this == val) {
        *this = Decimal(1);
        return *this;
    }
    int i, lgcat = 0, cc;
    char *res = new char[length];
    Decimal tmp;
    Decimal tmp2;
    Decimal tmp3;
    tmp3 = val;
    for (i = length - 1; tmp2 = tmp * 10, !(tmp2 + INT(value[i]) >= tmp3); i--) {
        tmp *= 10;
        tmp += INT(value[i]);
    }
    for (; i >= 0; i--) {
        tmp *= 10;
        tmp += INT(value[i]);
        for (cc = 9; !(tmp >= tmp3 * cc); cc--);
        tmp -= tmp3 * cc;
        res[lgcat++] = cc;
    }
    delete value;
    value = res;
    reverse();
    if (sign == val.sign)
        sign = false;
    else
        sign = true;
    return *this;
}

const Decimal Decimal::operator/(const Decimal & val) {
    Decimal tmp;
    tmp = *this;
    tmp /= val;
    return tmp;
}
*/
void Decimal::PrintValue() {
    value[length] = '\0';
    reverse();
    std::cout << value << std::endl;
    reverse();
}

void Decimal::PrintSign() {
    std::cout << (sign ? '-' : '+');
}

void Decimal::PrintDecimal() {
    PrintSign();
    PrintValue();
}

int main() {
    Decimal a("100");
    Decimal b("12");
    Decimal d;
    Decimal c(1200);
    a = d * c;
    a.PrintDecimal();
    return 0;
}
