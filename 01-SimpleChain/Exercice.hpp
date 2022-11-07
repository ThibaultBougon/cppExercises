#pragma once
#include <cstddef>

namespace exercice {
  /*
  * L'objectif est de faire une liste chainee capable de stocker
  * x donn�es de type _Ty, d'y acc�der et d'en supprimer.
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
     //Ajoute la donn�e data � la fin de la liste
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
    /// Supprime une donn�e de la liste par son id
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
    /// Acc�de � une donn�e via son id
    /// </summary>
    /// <note>
    /// Si la donn�e n'existe pas, renvoi nullptr
    /// </note>
    /// <param name="index">id de la donn�e a acc�der</param>
    /// <returns>ptr vers la donn�e</returns>
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
    /// Acc�s � la premi�re donn�e stock�e dans la liste
    /// </summary>
    /// <note>
    /// Si la donn�e n'existe pas, renvoi nullptr
    /// </note>
    /// <returns>ptr vers la donn�e</returns>
    _Ty* first() { 
      return _head->data;
    }

    /// <summary>
    /// Acc�s � la derni�re donn�e stock�e dans la liste
    /// </summary>
    /// <note>
    /// Si la donn�e n'existe pas, renvoi nullptr
    /// </note>
    /// <returns>ptr vers la donn�e</returns>
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
    /// Supprime toutes les donn�es de la liste
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