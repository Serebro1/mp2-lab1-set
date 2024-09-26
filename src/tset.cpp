// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);
//Нельзя 
TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
    /* объект бит.поля и при заходе в конструктор
    поля уже должны быть созданы
    У нас BitField не указатель!! Объёект хранится в TSet
    Поэтому создать можно только через список инициализации
    */
}


// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
    
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{

}
//Оператор преобразования типа

TSet::operator TBitField()
{
    // из set в TBitField
    return TBitField(BitField);
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
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this == &s) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{

    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet &s) // объединение
{

    return BitField | s.BitField;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res = TSet(*this);
    res.InsElem(Elem);
    return res;
}
TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet res = TSet(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    return BitField & s.BitField;
}

TSet TSet::operator~(void) // дополнение
{
    return ~this->BitField;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{// ввод до отрицательного числа
    int tmp = 0;
    while (tmp >= 0) {
        istr >> tmp;
        s.InsElem(tmp);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    // пройтись по всем массиву и проверить на 0
    for (int i = 0; i < s.MaxPower; i++)
    {
        if (s.BitField.GetBit(i)) ostr << i << " ";
    }
    return ostr;
}
