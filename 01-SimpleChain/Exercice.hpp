#pragma once
#include <cstddef>

namespace exercice {
  /*
  * L'objectif est de faire une liste chainee capable de stocker
  * x données de type _Ty, d'y accéder et d'en supprimer.
  */
  template<typename _Ty>class Node {
  public:
    _Ty* data;
    Node* next;
    //Node(_Ty d) : data(d), next(nullptr) {}
  };

  template<typename _Ty>class SimpleChain {
  public:
    using value_type = _Ty;
    Node<_Ty> *_head;
    Node<_Ty> *_back;
    std::size_t _size;

    SimpleChain() : _size(0), _head(nullptr), _back(nullptr) {}

     //<summary>
     //Ajoute la donnée data à la fin de la liste
     //</summary>
     //<param name="data">Data to add</param>
     //<returns>The id of the newly added data</returns>
    std::size_t push_back(_Ty* data) {
      Node<_Ty> *newNode = new Node<_Ty>();
      newNode->data = data;

      if (!_head) {
        _head = newNode;
        _back = _head;
        return ++_size;
      }

      _back->next = newNode;
      _back = _back->next;
      return _size++;
    }

    /// <summary>
    /// Supprime une donnée de la liste par son id
    /// </summary>
    /// <param name="index">id de la data a suppr</param>
    void remove(std::size_t index) {
      Node<_Ty>* tmp = nullptr;
      if (index > _size)
        return;

      tmp = _head;
      if(index == 0){
        _head = _head->next;
        delete tmp;
        --_size;
        return;
      }

      Node<_Ty>* prev = nullptr;
      Node<_Ty>* next = nullptr;
      for (std::size_t i = 0; i < index; ++i) {
        prev = tmp;
        tmp = tmp->next;
      }
      next = tmp->next;
      prev->next = next;
      if (!next)
        _back = prev;
      if(tmp)
        delete tmp;
      --_size;
    }

    /// <summary>
    /// Accède à une donnée via son id
    /// </summary>
    /// <note>
    /// Si la donnée n'existe pas, renvoi nullptr
    /// </note>
    /// <param name="index">id de la donnée a accéder</param>
    /// <returns>ptr vers la donnée</returns>
    _Ty* at(std::size_t index) {
      if (index > _size)
        return nullptr;
      if (index == 0)
        return _head->data;
      if (index == _size)
        return _back->data;

      Node<_Ty>* tmp = _head;
      for (std::size_t i = 0; i < index; ++i)
        tmp = tmp->next;
      return tmp->data;
    }

    /// <summary>
    /// Accès à la première donnée stockée dans la liste
    /// </summary>
    /// <note>
    /// Si la donnée n'existe pas, renvoi nullptr
    /// </note>
    /// <returns>ptr vers la donnée</returns>
    _Ty* first() { 
      return _head->data;
    }

    /// <summary>
    /// Accès à la dernière donnée stockée dans la liste
    /// </summary>
    /// <note>
    /// Si la donnée n'existe pas, renvoi nullptr
    /// </note>
    /// <returns>ptr vers la donnée</returns>
    _Ty* last() {
      _back = _head->next;
      for (std::size_t i = 1; i < _size; ++i)
        _back = _back->next;
      if (_back)
        return _back->data;
      return nullptr;
    }

    /// <summary>
    /// Renvoi le nb d'elements dans la liste
    /// </summary>
    /// <returns></returns>
    std::size_t size() { return _size; }

    /// <summary>
    /// Supprime toutes les données de la liste
    /// </summary>
    void clear() {
      Node<_Ty>* tmp = _head;
      while (tmp) {
        Node<_Ty>* d = tmp;
        tmp = tmp->next;
        delete d;
      }

      _head = nullptr;
      _back = nullptr;
      _size = 0;
    }
  };
}