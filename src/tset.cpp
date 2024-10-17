// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
    
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.MaxPower)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf.GetLength())
{
    BitField = bf;
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    TBitField res(BitField);
    return res;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) throw "incorrect operation";
    if (BitField.GetBit(Elem) == 0) BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) throw "incorrect operation";
    if (BitField.GetBit(Elem) != 0) BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        BitField = s.BitField;
        MaxPower = s.MaxPower;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    int flag = 1;
    if (MaxPower != s.MaxPower) {
        flag = 0;
    }
    else {
        if (BitField != s.BitField) flag = 0;
    }
    return flag;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return ((*this == s) + 1) % 2;
}

TSet TSet::operator+(const TSet &s) // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet dop(MaxPower);
    dop.InsElem(Elem);
    return TSet(BitField | dop.BitField);
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    (*this).DelElem(Elem);
    return (*this);
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int tmp;
    char ch;
    cout << "{";
    while (ch != '}') {
        istr >> ch;
    }
    while (ch != '}') {
        istr >> tmp;
        s.InsElem(tmp);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    bool flag = false;
    int tmp = s.GetMaxPower();
    for (int i = 0; i < tmp; i++)
        if (s.IsMember(i))
            flag = true;
    if (flag) {
        ostr << "{";
        for (int i = 0; i < tmp; i++) {
            if (s.IsMember(i))
                ostr << i << ";";
        }
        ostr << "}";
    }
    else
        ostr << "empty" << endl;
    return ostr;
}
