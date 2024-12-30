#include "imsystem.h"

IMSystem::IMSystem()
{
    m_Client = new Client();
    m_RegisterInterface = new MainWindow();
    m_RegisterInterface->show();
}

IMSystem::~IMSystem()
{
    if(m_Client)
    {
        delete m_Client;
        m_Client = nullptr;
    }
    if(m_RegisterInterface)
    {
        delete m_RegisterInterface;
        m_RegisterInterface = nullptr;
    }
}
