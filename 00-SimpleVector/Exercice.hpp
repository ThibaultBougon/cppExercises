#pragma once
#include "Utils.hpp"
#include <cstddef>
#include <iostream>

namespace excercice {
  /*
    Le but de la classe est de faire comme std::vector, un tableau dynamique
    capable de stocker un nombre indéfini et variable d'objets. Dans notre
    cas, on n'a pas besoin de supprimer des objets spécifiques, on ne fait
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
  public :
    using value_type = _Ty;
    static const std::size_t DEFAULTSIZE = 8;
    std::size_t _it;
    std::size_t _arrSize;
    std::size_t _dataIndex;
    _Ty* _arr;

    // Optionnel: préalloue le tableau pour un certain nombre de données
    SimpleVector(std::size_t dsize) : _arrSize(0), _dataIndex(0), _it(1), _arr(nullptr) {
      _realloc(dsize);
    }

    // Constructeur principal
    SimpleVector() : SimpleVector(DEFAULTSIZE) {
      
    }

    void _realloc(std::size_t newSize) {
      auto* tmp = _arr;
      //_arr = nullptr;
      _arr = new _Ty[newSize];
      ++_it;
      if (tmp) {
        std::memcpy(_arr, tmp, _arrSize * sizeof(_Ty));
        delete[] tmp;
      }
      _arrSize = newSize;
    }

    /// <summary>
    /// Ajoute un élément dans le tableau par copie
    /// </summary>
    /// <param name="elem">Une copie de l'élement a ajouter</param>
    void push_back(_Ty elem) {
      if(_arrSize == _dataIndex)
        _realloc(DEFAULTSIZE * _it);
      _arr[_dataIndex] = elem;
      ++_dataIndex;
    }

    /// <summary>
    /// Donne le nombre d'éléments actuellement présents dans le tableau
    /// </summary>
    /// <returns>Le nb d'elems</returns>
    std::size_t size() {
      return _dataIndex;
    }

    /// <summary>
    /// Permet d'accéder à un element particulier via un pointeur sans possibilité de modifier l'élément
    /// </summary>
    /// <param name="index">L'index de l'élément à acceder [0,+)</param>
    /// <returns>Un pointeur vers l'objet constant</returns>
    const _Ty* at(std::size_t index) {
      return &_arr[index];
    }

    /// <summary>
    /// Permet de vider le tableau.
    /// </summary>
    void clear() {
      std::memset(_arr, 0, _arrSize);
      _dataIndex = 0;
    }

    ~SimpleVector() {
      if (_arr) {
        delete[] _arr;
        _arrSize = 0;
      }
    }
  };
}