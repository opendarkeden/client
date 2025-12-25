/**
 * @file tarray.h
 * @brief Template Array class compatible with original Dark Eden TArray
 * 
 * Requirements: 4.6
 * 
 * This template provides a dynamic array with file I/O support.
 * The DataType class must implement:
 *   - bool SaveToFile(FILE* file)
 *   - bool LoadFromFile(FILE* file)
 */

#ifndef TARRAY_H
#define TARRAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * Template Array class
 * 
 * @tparam DataType The type of elements stored in the array
 * @tparam SizeType The type used for size and indexing (typically uint8_t, uint16_t, or uint32_t)
 */
template <typename DataType, typename SizeType>
class TArray {
public:
    /**
     * Constructor
     * @param size Initial size of the array (default 0)
     */
    TArray(SizeType size = 0);
    
    /**
     * Destructor - releases allocated memory
     */
    ~TArray();
    
    /**
     * Initialize array with given size
     * Releases any existing data before allocating new memory
     * @param size Number of elements to allocate
     */
    void Init(SizeType size);
    
    /**
     * Release allocated memory and reset size to 0
     */
    void Release();
    
    /**
     * Get the current size of the array
     * @return Number of elements in the array
     */
    SizeType GetSize() const { return m_Size; }
    
    /**
     * Array subscript operator (non-const)
     * @param n Index of element to access
     * @return Reference to element at index n
     */
    DataType& operator[](SizeType n) { return m_pData[n]; }
    
    /**
     * Array subscript operator (const)
     * @param n Index of element to access
     * @return Const reference to element at index n
     */
    const DataType& operator[](SizeType n) const { return m_pData[n]; }
    
    /**
     * Assignment operator
     * @param array Source array to copy from
     */
    void operator=(const TArray<DataType, SizeType>& array);
    
    /**
     * Addition assignment operator - appends another array
     * @param array Array to append
     */
    void operator+=(const TArray<DataType, SizeType>& array);
    
    /**
     * Save array to file
     * Writes size followed by each element's SaveToFile
     * @param file FILE pointer to write to
     * @return true on success, false on failure
     */
    bool SaveToFile(FILE* file);
    
    /**
     * Load array from file
     * Reads size followed by each element's LoadFromFile
     * @param file FILE pointer to read from
     * @return true on success, false on failure
     */
    bool LoadFromFile(FILE* file);

protected:
    SizeType m_Size;      /**< Number of elements in array */
    DataType* m_pData;    /**< Pointer to array data */
};

/* ============================================================================
 * Implementation
 * ============================================================================ */

template <typename DataType, typename SizeType>
TArray<DataType, SizeType>::TArray(SizeType size)
{
    m_Size = 0;
    m_pData = NULL;
    
    Init(size);
}

template <typename DataType, typename SizeType>
TArray<DataType, SizeType>::~TArray()
{
    Release();
}

template <typename DataType, typename SizeType>
void TArray<DataType, SizeType>::Init(SizeType size)
{
    /* Release existing data first */
    Release();
    
    if (size == 0) return;
    
    m_Size = size;
    m_pData = new DataType[m_Size];
}

template <typename DataType, typename SizeType>
void TArray<DataType, SizeType>::Release()
{
    if (m_pData != NULL) {
        delete[] m_pData;
        m_pData = NULL;
        m_Size = 0;
    }
}

template <typename DataType, typename SizeType>
void TArray<DataType, SizeType>::operator=(const TArray<DataType, SizeType>& array)
{
    /* Initialize with same size */
    Init(array.m_Size);
    
    /* Copy all elements */
    for (SizeType i = 0; i < m_Size; i++) {
        m_pData[i] = array.m_pData[i];
    }
}

template <typename DataType, typename SizeType>
void TArray<DataType, SizeType>::operator+=(const TArray<DataType, SizeType>& array)
{
    SizeType newSize = m_Size + array.m_Size;
    
    /* Allocate new memory for combined array */
    DataType* pTempData = new DataType[newSize];
    
    /* Copy existing data */
    SizeType k = 0;
    for (SizeType i = 0; i < m_Size; i++) {
        pTempData[k] = m_pData[i];
        k++;
    }
    
    /* Copy appended data */
    for (SizeType i = 0; i < array.m_Size; i++) {
        pTempData[k] = array.m_pData[i];
        k++;
    }
    
    /* Release old memory */
    Release();
    
    /* Update to new data */
    m_Size = newSize;
    m_pData = pTempData;
}

template <typename DataType, typename SizeType>
bool TArray<DataType, SizeType>::SaveToFile(FILE* file)
{
    if (file == NULL) return false;
    
    /* Write size (using sizeof(SizeType) bytes) */
    if (fwrite(&m_Size, sizeof(SizeType), 1, file) != 1) {
        return false;
    }
    
    /* Nothing to write if empty */
    if (m_pData == NULL || m_Size == 0) {
        return false;
    }
    
    /* Write each element */
    for (SizeType i = 0; i < m_Size; i++) {
        if (!m_pData[i].SaveToFile(file)) {
            return false;
        }
    }
    
    return true;
}

template <typename DataType, typename SizeType>
bool TArray<DataType, SizeType>::LoadFromFile(FILE* file)
{
    if (file == NULL) return false;
    
    /* Read size */
    if (fread(&m_Size, sizeof(SizeType), 1, file) != 1) {
        return false;
    }
    
    /* Size 0 is valid - just means empty array */
    if (m_Size == 0) {
        m_pData = NULL;
        return true;
    }
    
    /* Allocate memory */
    Init(m_Size);
    
    /* Read each element */
    for (SizeType i = 0; i < m_Size; i++) {
        if (!m_pData[i].LoadFromFile(file)) {
            return false;
        }
    }
    
    return true;
}

#endif /* TARRAY_H */
