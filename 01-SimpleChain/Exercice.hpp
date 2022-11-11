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
  };

  template<typename _Ty>class SimpleChain {
    Node<_Ty>* m_head;
    Node<_Ty>* m_back;
    std::size_t m_size;

    Node<_Ty>* _findInArr(std::size_t index) {
      Node<_Ty>* buff = m_head;
      for (int i = 0; i < index; ++i)
        buff = buff->next;
      return buff;
    }

  public:
    using value_type = _Ty;

    SimpleChain() : m_size(0), m_head(nullptr), m_back(nullptr) {}

     //<summary>
     //Ajoute la donnée data à la fin de la liste
     //</summary>
     //<param name="data">Data to add</param>
     //<returns>The id of the newly added data</returns>
    std::size_t push_back(_Ty* data) {
      Node<_Ty> *newNode = new Node<_Ty>();
      newNode->data = data;

      if (!m_head) {
        m_head = newNode;
        m_back = m_head;
        return ++m_size;
      }

      m_back->next = newNode;
      m_back = m_back->next;
      return m_size++;
    }

    /// <summary>
    /// Supprime une donnée de la liste par son id
    /// </summary>
    /// <param name="index">id de la data a suppr</param>
    void remove(std::size_t index) {
      Node<_Ty>* buff = nullptr;
      if (index > m_size)
        return;

      buff = m_head;
      if(index == 0){
        m_head = m_head->next;
        delete buff;
        --m_size;
        return;
      }

      Node<_Ty>* prev = _findInArr(index-1);
      buff = prev->next;
      Node<_Ty>* next = buff->next;

      next = buff->next;
      prev->next = next;
      if (!next)
        m_back = prev;
      if(buff)
        delete buff;
      --m_size;
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
      if (index > m_size)
        return nullptr;
      if (index == 0)
        return m_head->data;
      if (index == m_size)
        return m_back->data;

      return _findInArr(index)->data;
    }

    /// <summary>
    /// Accès à la première donnée stockée dans la liste
    /// </summary>
    /// <note>
    /// Si la donnée n'existe pas, renvoi nullptr
    /// </note>
    /// <returns>ptr vers la donnée</returns>
    _Ty* first() { 
      return m_head->data;
    }

    /// <summary>
    /// Accès à la dernière donnée stockée dans la liste
    /// </summary>
    /// <note>
    /// Si la donnée n'existe pas, renvoi nullptr
    /// </note>
    /// <returns>ptr vers la donnée</returns>
    _Ty* last() {
      if (m_back)
        return m_back->data;
      return nullptr;
    }

    /// <summary>
    /// Renvoi le nb d'elements dans la liste
    /// </summary>
    /// <returns></returns>
    std::size_t size() { return m_size; }

    /// <summary>
    /// Supprime toutes les données de la liste
    /// </summary>
    void clear() {
      Node<_Ty>* buff = m_head;
      while (buff) {
        Node<_Ty>* d = buff;
        buff = buff->next;
        delete d;
      }

      m_head = nullptr;
      m_back = nullptr;
      m_size = 0;
    }
  };
}