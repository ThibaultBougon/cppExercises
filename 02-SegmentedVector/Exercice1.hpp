#pragma once
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <iostream>

namespace exercice1 {
  /*
  * Le but est de faire un tableau dynamique ayant globalement la même
  * interface que std::vector (ou SimpleVector) mais dont l'allocation en
  * interne se fait par segments.
  * Ainsi, au lieu d'avoir un seul gros tableau, on utilise une série
  * de plus petits tableaux. En pratique :
  *   On veut un tableau où foutre 100 éléments. Un SimpleVector fera une
  *   allocation de 100. SegmentedVector, lui, fera 4 allocations de 32.
  *
  * SimpleVector:
  * [0, 1, 2, ..., 99]
  * SegmentedVector (segment size = 32):
  * [0, 1, 2, ..., 31]
  * [32, 33, 34, ..., 63]
  * [64, 65, 66, ..., 95]
  * [96, 97, 98, ..., 127]
  *
  *
  * Ce design est parfois utile en pratique pour limiter les réallocations.
  * Ainsi, depuis ces 100 éléments, si nous voulons rajouter 20 éléments de plus,
  * aucune réallocation n'est nécessaire. Aussi, si nous voulons dépasser les 127
  * éléments, il suffit d'allouer un nouveau bloc au lieu de tout copier comme
  * dans le SimpleVector.
  *
  * Dans ce premier exercice, le nombre de segments et leur taille est fixe.
  * Aussi, nous n'avons pas besoin de la capacité à enlever des éléments des
  * tableaux (push-only).
  *
  * Optionnel 1:
  * Nous voulons afficher un message d'erreur compréhensible dans le cas où
  * l'utilisateur utilisait void comme type de donnée. Pour cette partie,
  * interdit d'utiliser std::enable_if(_t) ni les concepts ou require.
  * Le plus simple est le mieux !
  *
  * Optionnel 2:
  * Pour légèrement augmenter la difficulté, il est possible de faire cet exercice
  * sans stocker le nombre d'éléments actuels dans les segments. A la place, on peut
  * utiliser des pointeurs. Il est même possible d'implémenter size() même sans avoir
  * le nombre d'élements par segments grâce à ces mêmes pointeurs.
  *
  * Optionnel 3:
  * Au lieu d'utiliser une taille et un nombre de segments constant, on peut
  * faire un système dynamique capable de changer la géométrie de notre
  * tableau. Par exemple, modifier le nombre ou la taille des segments alors
  * que la donnée est deja présente dans le tableau.
  *
  */
  // opt 3 == reorganise => donne les new param et ca gere tout
  template<typename _Ty>class SegmentedVector {
    void _allocNewSegment() {
      auto* buff = new _Ty[SEGMENT_SIZE];
      m_arr[(m_dataIndex / SEGMENT_SIZE)] = buff;
      m_isLinFull = false;
    }

    void _fillArr(_Ty elem) {
      m_arr[m_dataIndex / SEGMENT_SIZE][m_dataIndex % SEGMENT_SIZE] = elem;
      ++m_dataIndex;
      if (m_dataIndex % SEGMENT_SIZE == 0)
        m_isLinFull = true;
    }

    void _copyArr(_Ty* arr, std::size_t destIndex, std::size_t sourceIndex, std::size_t size) {
      std::memcpy(arr + destIndex, m_arr[sourceIndex], size * sizeof(_Ty));
    }

    bool _reindex(std::size_t* index) {
      std::size_t seg = *index / SEGMENT_SIZE;
      for (int i{ 0 }; i <= seg; ++i) {
        *index += m_noData[i];
      }
      if (seg < (*index / SEGMENT_SIZE))
        *index += m_noData[seg+1];
      if (*index >= m_dataIndex)
        return false;
      return true;
    }

  public:
    using value_type = _Ty;
    // The maximum amount of segments
    const int SEGMENT_COUNT = 32;
    // The maximum number of elements in each segments
    const int SEGMENT_SIZE = 32;
    // Note: the maximum amount of elements in this
    // SegmentedVector is then COUNT*SIZE
    std::size_t m_dataIndex;
    bool  m_isLinFull;
    _Ty** m_arr;
    std::size_t* m_noData;
    static_assert(!std::is_void<_Ty>::value, "Can not be void type");

    SegmentedVector() : m_dataIndex(0), m_arr(nullptr), m_isLinFull(false), m_noData(nullptr) {
      m_noData = new std::size_t[SEGMENT_COUNT]{ 0 };
    }
    ~SegmentedVector() {
      clear();
    }

    //static_assert(std::is_void<_Ty>::value, "T must not be void");

    /// <summary>
    /// Add an element in the array
    /// </summary>
    /// <param name="elem">The elem to add</param>
    void push_back(_Ty elem) {
      if (!m_arr) {
        m_arr = new _Ty * [SEGMENT_COUNT];
        _allocNewSegment();
      }
      if (m_isLinFull)
        _allocNewSegment();
      _fillArr(elem);
    }

    /// <summary>
    /// Copy the content to another array
    /// </summary>
    /// <param name="arr">The array to copy to</param>
    /// <param name="max">The maximum number of elements to copy</param>
    void copy(_Ty* arr, std::size_t max = static_cast<std::size_t>(-1)) {
      if (max > m_dataIndex)
        max = m_dataIndex;
      for (int i{ 0 }; i <= (max / SEGMENT_SIZE); ++i) {
        _copyArr(arr, (i * SEGMENT_SIZE), i, ((max / SEGMENT_SIZE) != i) ? SEGMENT_SIZE : (max % SEGMENT_SIZE));
      }
    }

    void resize(int newSegSize, int newCountSize) {
      int oldSegSize = SEGMENT_SIZE;
      int* ptrSegSize = (int*)(&SEGMENT_SIZE);
      *ptrSegSize = newSegSize;
      int oldCountSize = SEGMENT_COUNT;
      int* ptrCountSize = (int*)(&SEGMENT_COUNT);
      *ptrCountSize = newCountSize;

      _Ty** oldArr = m_arr;
      _Ty** newArr = new _Ty * [SEGMENT_COUNT];
      _Ty* buff = nullptr;
      std::size_t oldDataIndex = m_dataIndex;
      m_dataIndex = 0;

      for (int i{ 0 }; (i < oldDataIndex) && ((i / SEGMENT_SIZE) <= SEGMENT_COUNT); ++i) {
        if (m_dataIndex % SEGMENT_SIZE == 0) {
          newArr[m_dataIndex / SEGMENT_SIZE] = new _Ty[SEGMENT_SIZE];
        }
        if (i % oldSegSize == 0) {
          buff = m_arr[i / oldSegSize];
          i += m_noData[i / oldCountSize];
          buff = &buff[m_noData[i / oldCountSize]];
        }
        newArr[m_dataIndex / SEGMENT_SIZE][m_dataIndex % SEGMENT_SIZE] = *buff++;
        ++m_dataIndex;
      }

      std::size_t* newNoDataArray = new std::size_t[SEGMENT_COUNT]{ 0 };
      auto oldNoDataArray = m_noData;
      m_noData = newNoDataArray;
      delete[] oldNoDataArray;
      for (int i{ 0 }; i < (m_dataIndex / oldSegSize); ++i) {
        delete[] oldArr[i];
      }
      delete[] oldArr;
      m_arr = newArr;
      return;
    }

    void remove(std::size_t index) {
      if (!_reindex(&index))
        return;
      std::size_t seg = index / SEGMENT_SIZE;
      _Ty buff = m_arr[seg][m_noData[seg]];
      m_arr[seg][m_noData[seg]] = NULL;
      ++m_noData[seg];
      for (std::size_t i{ m_noData[seg] }; (i < SEGMENT_SIZE) && (i <= (index%SEGMENT_SIZE)); ++i) {
        _Ty next = m_arr[seg][i];
        m_arr[seg][i] = buff;
        buff = next;
      }
    }

    void clear() {
      if (!m_arr)
        return;
      for (int i{ 0 }; i < (m_dataIndex / SEGMENT_SIZE); ++i) {
        delete[] m_arr[i];
      }
      delete[] m_arr;
      m_arr = nullptr;
      delete[] m_noData;
      m_noData = nullptr;
    }

    /// <summary>
    /// Retrieve an element from the array
    /// </summary>
    /// <param name="index">The index of the element to retrieve</param>
    /// <returns></returns>
    _Ty& at(std::size_t index) {
      if (!_reindex(&index)) {
        static _Ty remove_me;
        return remove_me;
      }
      //return nullptr;
      return m_arr[index / SEGMENT_SIZE][index % SEGMENT_SIZE];
    }

    // return false tant que le dernier segment
    // n'est pas rempli sinon true
    bool is_lineary_full() {
      return m_isLinFull;
    }

  };
}