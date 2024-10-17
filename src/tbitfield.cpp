// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) throw "incorrect operation";
    BitLen =  len;
    MemLen = (len / (sizeof(TELEM)*8)) + int((len % (sizeof(TELEM) * 8) != 0));
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n >= BitLen || n < 0) throw "incorrect operation";
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n >= BitLen || n < 0) throw "incorrect operation";
    return TELEM(static_cast<TELEM>(1) << n);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n >= BitLen || n < 0) throw "incorrect operation";
    pMem[GetMemIndex(n)] |= (GetMemMask(n));
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= BitLen || n < 0) throw "incorrect operation";
    pMem[GetMemIndex(n)] &= (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n >= BitLen || n < 0) throw "incorrect operation";
    return ((pMem[GetMemIndex(n)]) & (GetMemMask(n))) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf) {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        delete[] pMem;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            (pMem[i] = bf.pMem[i]);
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int flag = 1;
    if (BitLen != bf.BitLen) {
        flag = 0;
    }
    else {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                flag = 0;
            }
        }
    }
    return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return ((*this == bf)+1) % 2;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int flag = 0;
    int maxBLen = max(bf.BitLen, BitLen);
    int minBLen = min(bf.BitLen, BitLen);
    if ((BitLen > bf.BitLen)) {
        flag = 1;
    }
    TBitField res(maxBLen);
    for (int i = 0; i  < minBLen; i++) {
        if (GetBit(i) == 1 || bf.GetBit(i) == 1) res.SetBit(i);
    }
    if (flag == 0) {
        for (int i = minBLen; i < maxBLen; i++) {
            if (bf.GetBit(i) == 1) res.SetBit(i);
        }
    }
    else {
        for (int i = minBLen; i < maxBLen; i++) {
            if (GetBit(i) == 1) res.SetBit(i);
        }
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = bf.BitLen;
    int minLen = BitLen;
    int flag = 0;
    if (BitLen != bf.BitLen) {
        if (BitLen > bf.BitLen) {
            maxLen = BitLen;
            minLen = bf.BitLen;
            flag = 1;
        }
    }
    TBitField res(maxLen);
    for (int i = 0; i < minLen; i++) {
        if (GetBit(i) == 1 && bf.GetBit(i) == 1) res.SetBit(i);
    }
    if (flag == 0) {
        for (int i = minLen; i < 0; i++) {
            res.ClrBit(i);
        }
    }
    else {
        for (int i = minLen; i < 0; i++) {
            res.ClrBit(i);
        }
    }
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(*this);
    for (int i = 0; i < BitLen; i++) {
        if ((*this).GetBit(i) == 0) res.SetBit(i);
        else res.ClrBit(i);
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string res;
    istr >> res;
    if (res.size() != bf.BitLen) {
        throw "incorrect operation";
    }
    for (int i = 0; i < bf.BitLen; ++i) {
        if (res[i] == '1') {
            bf.SetBit(i);
        }
        else if (res[i] != '0') throw "incorrect operation";
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; ++i) {
        ostr << (bf.GetBit(i));
    }
    return ostr;
}
