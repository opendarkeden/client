#include "Client_PCH.h"
#include "MCrashReportManager.h"

void	
MCrashReport::LoadFromFile(std::ifstream &file)
{
	m_ExecutableTime.LoadFromFile(file);
	file.read((char *)&m_Version, sizeof(WORD));
	m_Address.LoadFromFile(file);
	m_OS.LoadFromFile(file);
	m_CallStack.LoadFromFile(file);
	m_Message.LoadFromFile(file);
}

void	
MCrashReport::SaveToFile(std::ofstream &file)
{
	m_ExecutableTime.SaveToFile(file);
	file.write((const char *)&m_Version, sizeof(WORD));
	m_Address.SaveToFile(file);
	m_OS.SaveToFile(file);
	m_CallStack.SaveToFile(file);
	m_Message.SaveToFile(file);
}
