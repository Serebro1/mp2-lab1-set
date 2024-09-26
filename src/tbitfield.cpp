// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int _BitLen)
{
    if (_BitLen <= 0) throw - 1; // чтобы в гугл тест прошло нужно выкинуть любое исключение
    BitLen = _BitLen;
    MemLen = _BitLen / (sizeof(TELEM) * 8) + 1; // задача: как давать не лишнюю память?
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (!(n >= 0 && n < BitLen)) throw - 2;
    return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (!(n >= 0 && n < BitLen)) throw - 2;
    return 1 << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // 0 если заданный бит равен 0, любое другое число если нет
{
    // проверка
    return (pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        pMem = new TELEM[bf.MemLen];
        MemLen = bf.MemLen;
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return false;
    for (int i = 0; i < MemLen - 1; i++)
    {
        if (bf.pMem[i] != pMem[i]) return false;
    }
    for (int i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen; i++)
    {
        if (bf.GetBit(i) != GetBit(i)) return false;
    }
    return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField res = TBitField((BitLen > bf.BitLen) ? BitLen : bf.BitLen);
    int minLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    int count = 0; // счётчик до конца первого for
    for (int i = 0; i < minLen; i++)
    {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }
    if (bf.BitLen > BitLen)
        for (int i = minLen; i < res.MemLen; i++)
            res.pMem[i] = bf.pMem[i];
    else 
        for (int i = minLen; i < res.MemLen; i++)
            res.pMem[i] = pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    // Вариант реализации: результат максимальной длины
    TBitField res = TBitField((BitLen > bf.BitLen) ? BitLen : bf.BitLen);
    int minLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minLen; i++)
    {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    // Проблема: мусор в виде 0 может стать 1
    // (Memlen-1) * sizeof(TELEM)
    TBitField res = TBitField(BitLen);
    for (int i = 0; i < MemLen - 1; i++)
    {
        res.pMem[i] = ~pMem[i];
    }
    for (int i = (MemLen - 1) * sizeof(TELEM) * 8; i < BitLen; i++)
    {
        if (!GetBit(i)) res.SetBit(i);
    }
    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int bit;
    for (int i = 0; i < bf.BitLen; i++)
    {
        istr >> bit;
        if (bit) bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (bf.GetBit(i) == 0) ostr << 0;
        else ostr << 1;
    }
    return ostr;
}
