/*-----------------------------------------------------------------------------

	SimpleDataList.h

	2000.9.16. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __SIMPLEDATALIST_H__
#define __SIMPLEDATALIST_H__

#include "BasicS.h"

//-----------------------------------------------------------------------------
// SimpleDataList
//
// template class
//
// data의 추가, 삭제, 검색을 수행한다.
// data는 어떠한 것이라도(NULL 조차..) 들어올 수 있다. 그러나 같은 것이 또 
// 들어올 수는 없다.
//-----------------------------------------------------------------------------
template <class Ta> 
class SimpleDataList
{
private:
	Ta	*			m_p_buf;
	int			m_size;

public:
	SimpleDataList();
	virtual ~SimpleDataList();
	
	bool	Add(Ta data);
	bool	Delete(Ta data);

	int	Size() const { return m_size; }

	bool	Find(Ta data) const;
	bool	Data(int index, Ta &data) const;
};

//-----------------------------------------------------------------------------
// SimplateDataList
//
// constructor.
//-----------------------------------------------------------------------------
template <class Ta>
SimpleDataList<Ta>::SimpleDataList()
{
	m_p_buf	= NULL;
	m_size	= 0;
}

//-----------------------------------------------------------------------------
// ~SimplateDataList
//
// destructor.
//-----------------------------------------------------------------------------
template <class Ta>
SimpleDataList<Ta>::~SimpleDataList()
{
	DeleteNewArray(m_p_buf);
}

//-----------------------------------------------------------------------------
// Find
//
// data가 존재하면 true를 반환한다.
//-----------------------------------------------------------------------------
template <class Ta>
bool SimpleDataList<Ta>::Find(Ta data) const
{
	for (int i=0; i < m_size; i++)
		if (m_p_buf[i] == data)
			return true;

	return false;
}

//-----------------------------------------------------------------------------
// Data
//
// 저장된 data를 반환한다.
//-----------------------------------------------------------------------------
template <class Ta>
bool SimpleDataList<Ta>::Data(int index, Ta &data) const
{
	if (index < 0 || index >= m_size)
		return false;

	data = m_p_buf[index];

	return true;
}

//-----------------------------------------------------------------------------
// Add
//
// 이미 data가 존재하면 실패이다.
//
// 성공하면 true, 실패하면 false를 반환한다.
//-----------------------------------------------------------------------------
template <class Ta>
bool SimpleDataList<Ta>::Add(Ta data)
{
	if (Find(data) == true)
		return false;

	Ta *		p_temp = NULL;
	int		prev_size = m_size;
	int		i;

	if (prev_size > 0) // backup prev-data
	{
		p_temp = new Ta[prev_size];
		CheckMemAlloc(p_temp);

		for (i=0; i < prev_size; i++)
			p_temp[i] = m_p_buf[i];
	}
	
	DeleteNewArray(m_p_buf);
	m_size++;
	m_p_buf = new Ta[m_size];
	CheckMemAlloc(m_p_buf);

	for (i=0; i < prev_size; i++) // restore backup data
		m_p_buf[i] = p_temp[i];

	m_p_buf[m_size-1] = data;

	DeleteNewArray(p_temp);

	return true;
}

//-----------------------------------------------------------------------------
// Delete
//
// 성공하면 true, 실패하면 false를 반환한다.
//-----------------------------------------------------------------------------
template <class Ta>
bool SimpleDataList<Ta>::Delete(Ta data)
{
	for (int i=0; i < m_size; i++) // find
	{
		if (m_p_buf[i] == data)
		{
			Ta *		p_temp = NULL;
			int		new_size = m_size-1;

			if (new_size > 0)
			{
				p_temp = new Ta[new_size];
				CheckMemAlloc(p_temp);

				for (int m=0, t=0; m < m_size; m++)
					if (m != i)
					{
						p_temp[t] = m_p_buf[m];
						t++;
					}
			}

			DeleteNewArray(m_p_buf);
			m_size--;
			m_p_buf = p_temp;

			return true;
		}
	}

	return false;
}

#endif