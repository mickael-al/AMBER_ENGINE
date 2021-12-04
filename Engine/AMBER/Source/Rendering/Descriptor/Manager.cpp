#include "Manager.hpp"

namespace Ge
{
	Descriptor* Manager::getDescriptor()
	{
		return m_descriptor;
	}
	
	void Manager::destroyElement()
	{
		if (m_destroyElement)
		{
			updateDescriptor();
			m_destroyElement = false;
		}
	}

	bool Manager::getDestroyElement()
	{
		return m_destroyElement;
	}
}