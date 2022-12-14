#pragma once
#include "Utils.hpp"
#include <cstddef>
#include <iostream>

namespace excercice {
  /*
    Le but de la classe est de faire comme std::vector, un tableau dynamique
    capable de stocker un nombre ind?fini et variable d'objets. Dans notre
    cas, on n'a pas besoin de supprimer des objets sp?cifiques, on ne fait
    qu'en ajouter.
   
  * Pro Tip: pour le debugger VS dans les watch (espions en francais), tu peux
  * afficher les ptr sous forme de tableau :
  * Avec: _Ty* ptr;
  * Pour afficher ptr[0..9]: ptr,10
  * Pour afficher ptr[5..9]: ptr+5,5
  * https://docs.microsoft.com/fr-fr/visualstudio/debugger/format-specifiers-in-cpp?view=vs-2022
  */
  // _Ty correspond au type d'objet a stocker (comme les generic en C#)
  template<typename _Ty>class SimpleVector {
    std::size_t m_it;
    std::size_t m_dataIndex;
    static const std::size_t DEFAULTSIZE = 8;
    std::size_t m_arrSize;

    void _realloc(std::size_t newSize) {
      auto* tmp = _arr;
      _arr = new _Ty[newSize];
      if (tmp) {
        std::memcpy(_arr, tmp, m_arrSize * sizeof(_Ty));
        delete[] tmp;
      }
      m_arrSize = newSize;
    }
  public :
    using value_type = _Ty;
    _Ty* _arr;

    // Optionnel: pr?alloue le tableau pour un certain nombre de donn?es
    SimpleVector(std::size_t dsize) : m_arrSize(0), m_dataIndex(0), m_it(1), _arr(nullptr) {
      _realloc(dsize);
    }

    // Constructeur principal
    SimpleVector() : SimpleVector(DEFAULTSIZE) {
      
    }

    /// <summary>
    /// Ajoute un ?l?ment dans le tableau par copie
    /// </summary>
    /// <param name="elem">Une copie de l'?lement a ajouter</param>
    void push_back(_Ty elem) {
      if (m_dataIndex >= m_arrSize)
        _realloc(DEFAULTSIZE + m_arrSize);
      _arr[m_dataIndex] = elem;
      ++m_dataIndex;
    }

    /// <summary>
    /// Donne le nombre d'?l?ments actuellement pr?sents dans le tableau
    /// </summary>
    /// <returns>Le nb d'elems</returns>
    std::size_t size() {
      return m_dataIndex;
    }

    /// <summary>
    /// Permet d'acc?der ? un element particulier via un pointeur sans possibilit? de modifier l'?l?ment
    /// </summary>
    /// <param name="index">L'index de l'?l?ment ? acceder [0,+)</param>
    /// <returns>Un pointeur vers l'objet constant</returns>
    const _Ty* at(std::size_t index) {
      return &_arr[index];
    }

    /// <summary>
    /// Permet de vider le tableau.
    /// </summary>
    void clear() {
      std::memset(_arr, 0, m_arrSize);
      m_dataIndex = 0;
    }

    ~SimpleVector() {
      if (_arr) {
        delete[] _arr;
        m_arrSize = 0;
      }
    }
  };
}